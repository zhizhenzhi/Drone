#include "App_transmit_data.h"

extern Remote_data remote_data;
uint8_t transmit_buff[TX_PLOAD_WIDTH]={0};

/**
 * @brief 发送数据给飞机
 */
void app_transmit_data_process(void)
{
    //切换到发送模式
    SI24R1_TX_Mode();
    //发送数据 
    //帧头校验指定发送给对应的设备 
    //发送数据的结尾添加校验和（将发送的数据累加得到的值添加到数据的结尾
    
    uint32_t sum=0;
    //数据本身10字节 帧头校验设置为3字节 校验和设置为4字节
    transmit_buff[0]=FRAME_HEADER_CHECK1;
    transmit_buff[1]=FRAME_HEADER_CHECK2;
    transmit_buff[2]=FRAME_HEADER_CHECK3;

    //高八位在前
    transmit_buff[3]=(remote_data.thr >> 8) & 0xFF;
    transmit_buff[4]=remote_data.thr;
    
    transmit_buff[5]=(remote_data.yaw >> 8) & 0xFF;
    transmit_buff[6]=remote_data.yaw;

    transmit_buff[7]=(remote_data.pit >> 8) & 0xFF;
    transmit_buff[8]=remote_data.pit;

    transmit_buff[9]=(remote_data.rol >> 8) & 0xFF;
    transmit_buff[10]=remote_data.rol;

    taskENTER_CRITICAL();
    //此时的关机和定高只会有0或1
    transmit_buff[11]=remote_data.shutdown;
    remote_data.shutdown=0;
    transmit_buff[12]=remote_data.fix_height;
    remote_data.fix_height=0;
    taskEXIT_CRITICAL();
    for(uint8_t i=0;i<12;i++)
    {
        sum+=transmit_buff[i];
    }
    //高位在前
    transmit_buff[13]=(sum>>24)&0xFF;
    transmit_buff[14]=(sum>>16)&0xFF;
    transmit_buff[15]=(sum>>8)&0xFF;
    transmit_buff[16]=sum&0xFF;

    SI24R1_TxPacket(transmit_buff);

    //切换回接收模式
    SI24R1_RX_Mode();
}
