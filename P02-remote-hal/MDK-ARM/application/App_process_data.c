#include "App_process_data.h"


//摇杆数据结构体
Int_Joystick_t joystick ={0};
Remote_data remote_data={0};
//区分一下摇杆的控制值和按键的微调
int16_t key_pit_offset =0;
int16_t key_rol_offset =0;


//记录摇杆偏移量
int16_t thr_offset = 0;
int16_t yaw_offset = 0;
int16_t pit_offset = 0;
int16_t rol_offset = 0;
//校准摇杆函数
void App_calibrate_joystick(void){
    //零偏校准的逻辑就是减去零偏的值
    //首先清空按键的微调值
    key_pit_offset=0;
    key_rol_offset=0;
    //多次读写求平均值
    int16_t thr_sum=0;
    int16_t yaw_sum=0;
	int16_t rol_sum=0;
	int16_t pit_sum=0;
    for(int i=0;i<10;i++)
    {
        App_process_joystick_data();
        thr_sum += joystick.thr-0;
        yaw_sum += joystick.yaw-500;
        rol_sum += joystick.rol-500;
        pit_sum += joystick.pit-500;
        vTaskDelay(10);
    }
    //零偏校准的偏移值没有累加的效果 会造成两次校准退回的情况
    thr_offset+= thr_sum/10;
	yaw_offset+= yaw_sum/10;
	rol_offset+= rol_sum/10;
	pit_offset+= pit_sum/10;
}
/*
 处理按键数据的函数声明 
*/
void App_process_key_data(void)
{
    Key_type key = Key_Get();
    if(key == KEY_UP)
    {
        //向前飞微调 俯仰角+
        key_pit_offset +=10;
    }
    else if (key== KEY_DOWN)
    {
        //向后飞微调 俯仰角-
        key_pit_offset -=10;
    }
    else if (key== KEY_LEFT)
    {
        //向左飞微调 横滚角-
        key_rol_offset -=10;
    }
    else if (key== KEY_RIGHT)
    {
        //向右飞微调 横滚角+
        key_rol_offset +=10;
    }
    else if(key == KEY_LEFT_X)
    {
        remote_data.shutdown=1;
    }
    else if(key == KEY_RIGHT_X)
    {
        remote_data.fix_height=1;
    }
    else if(key == KEY_RIGHT_X_LONG)
    {
        //校准摇杆 触发校准之后 摇杆的值THR为0 其他为500 然后等待摇杆的值为0 再进行校准

        App_calibrate_joystick();
    }
}
/*
 处理摇杆数据的函数声明 
*/
void App_process_joystick_data(void)
{
    //解决freeRTOS的任务切换问题
    taskENTER_CRITICAL();
    //获取摇杆监控的ADC值
        Int_Joystick_Get_Value(&joystick);

        //2.处理范围和极性 想要使用的范围是0-1000 ADC的范围是0-4095
        joystick.thr = 1000-(joystick.thr*1000)/4095;
		joystick.yaw = 1000-(joystick.yaw*1000)/4095;
		joystick.rol = 1000-(joystick.rol*1000)/4095;
		joystick.pit = 1000-(joystick.pit*1000)/4095;
        
        //3.处理零偏校准
        joystick.thr -= thr_offset;
		joystick.yaw -= yaw_offset;
		joystick.rol -= rol_offset;
		joystick.pit -= pit_offset;

        //考虑按键的微调值
        joystick.pit += key_pit_offset;
		joystick.rol += key_rol_offset;
        
        //计算上偏移值可能超出范围 限制在0-1000
        joystick.thr=com_limit(joystick.thr,0,1000);
        joystick.yaw=com_limit(joystick.yaw,0,1000);
        joystick.pit=com_limit(joystick.pit,0,1000);
        joystick.rol=com_limit(joystick.rol,0,1000);

        //退出临界区
        taskEXIT_CRITICAL();
        //将处理完成的数据赋值给遥控数据结构体
        remote_data.thr=joystick.thr;
		remote_data.yaw=joystick.yaw;
		remote_data.rol=joystick.rol;
		remote_data.pit=joystick.pit;
        //debug_print("thr:%d,yaw:%d,rol:%d,pit:%d\r\n",joystick.thr,joystick.yaw,joystick.rol,joystick.pit);
}
