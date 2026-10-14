#include "App_receive_data.h"

extern uint8_t back_buff[TX_PLOAD_WIDTH];
//按下定高之后的飞行高度
extern uint16_t fix_height;
//保存解析后的遥控量
extern Remote_data remote_data;
//飞行状态
extern Flight_State flight_state;
//遥控连接状态
extern Remote_State remote_state;
Thr_state thr_state = FREE;
//max的进入时间
uint32_t max_entry_time = 0;
uint8_t rxbuff[TX_PLOAD_WIDTH]={0};
//MIN状态的进入时间
uint32_t min_entry_time = 0;

//重试次数
uint8_t retry_count=0;

/**
 * @brief 接收遥控器发送的数据解析为结构体
 * @return 0：校验通过
 */
uint8_t App_receive_data(void)
{
    memset(rxbuff,0,TX_PLOAD_WIDTH);

    uint8_t res = SI24R1_RxPacket(rxbuff);
    if(res == 0)
    {
        //收到遥控数据 准备回传
        SI24R1_TX_Mode();
        while(SI24R1_TxPacket(back_buff) == 1)
        {
        }
        SI24R1_RX_Mode();
    }

    if(SI24R1_RxPacket(rxbuff) != 0)
    return 1;

    //帧头校验 确保唯一性
    if(rxbuff[0] != FRAME_HEADER_CHECK1 || rxbuff[1] != FRAME_HEADER_CHECK2 || rxbuff[2] != FRAME_HEADER_CHECK3)
    return 1;

    //帧尾校验 验证数据传输过程中没有出现错误
    uint32_t sum=0;
    uint32_t sum_receive=0;
    for(uint8_t i=0;i<13;i++)
    {
        sum += rxbuff[i];
    }

    sum_receive =rxbuff[13]<<24 |rxbuff[14]<<16 |rxbuff[15]<<8 |rxbuff[16];
    if(sum != sum_receive)
    return 1;

    //保存数据
    remote_data.thr=(rxbuff[3] <<8) |rxbuff[4];
    remote_data.yaw=(rxbuff[5] <<8) |rxbuff[6];
    remote_data.pit=(rxbuff[7] <<8) |rxbuff[8];
    remote_data.rol=(rxbuff[9] <<8) |rxbuff[10];
    remote_data.shutdown=rxbuff[11];
    remote_data.fix_height=rxbuff[12];

    return 0;
}


/**
 * @brief 处理连接状态
 * @param res 上一层接收数据的返回值
 */
void App_process_connect_state(uint8_t res)
{
    if(res ==0 )
    {
        //接收数据成功 此处使用的全局变量
        remote_state = REMOTE_CONNECTED;
    }
    else if(res ==1)
    {
        //接收数据失败
        retry_count++;
        if(retry_count >= MAX_RETRY_TIMES)
        {
            remote_state = REMOTE_DISCONNECTED;
            retry_count = 0;
        } 
    }
}

/**
 *  @brief 处理解锁逻辑
 *  @return uint8_t 0:解锁成功 1：解锁失败
 */
static uint8_t App_process_unlock(void)
{
    //考虑安全问题 -》解锁完成的最终状态应该是油门为0 
    switch(thr_state)
    {
        case FREE:
        if(remote_data.thr>=900)
        {
            //进入max
            thr_state= MAX;
            max_entry_time=xTaskGetTickCount();
        }
        break;
        case MAX:
        if(remote_data.thr<900)
        {
            if(xTaskGetTickCount()-max_entry_time>1000)
            {
                //油门保持最高状态超过1s-》进入leave-max状态
                thr_state = LEAVE_MAX;
            }
            else
            {
                //油门保持最高状态小于1s-》退回到free重新解锁
                thr_state = FREE;
            }
        }
        break;
        case LEAVE_MAX:
        if(remote_data.thr <= 100)
        {
            thr_state=MIN;
        }
        break;
        case MIN:
        //每次判断当前已经保持了多久
        if(xTaskGetTickCount()-min_entry_time<=1000)
        {
            //油门保持最低状态不够1s
            if(remote_data.thr>100)
            {
                thr_state=FREE;
            }
            else
            {
                thr_state=UNLOCK;
            }
        }
        break;
        case UNLOCK:
        break;
    }
    if(thr_state == UNLOCK)
    {
        return 0;
    }
   return 1;
}
/**
 * @brief 处理飞行状态
 */
void App_process_flight_state(void)
{
    //使用轮询调用当前所处的状态
    switch(flight_state)
    {
        case IDLE:
        if(App_process_unlock() == 0)
        {
            flight_state=NORMAL_FLIGHT;
            //每一次解锁成功 需要将解锁状态重置
            thr_state = FREE;
        }
        break;
        case NORMAL_FLIGHT:
        if(remote_data.fix_height == 1)
        {
            flight_state=FIX_HEIGHT;
            remote_data.fix_height=0;

            //记录下当前目标的高度
            fix_height = Int_VL53L1X_GetDistance();
        }
        if(remote_state==REMOTE_DISCONNECTED)
        {
            flight_state=FAIL;
        }
        break;
        case FIX_HEIGHT:
        //取消定高
        if(remote_data.fix_height==1)
        {
            flight_state=NORMAL_FLIGHT;
            remote_data.fix_height=0;
        }
        if(remote_state==REMOTE_DISCONNECTED)
        {
            flight_state=FAIL;
        }
        break;
        case FAIL:
        //处理失联故障 缓慢停止电机 一直等待 不会超时
        ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
        flight_state = IDLE;

        break;
        default:
        break;
    }
}
