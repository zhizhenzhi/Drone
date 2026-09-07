#ifndef APP_PROCESS_DATA_H
#define APP_PROCESS_DATA_H

#include "Int_joystick.h"
#include "Int_key.h"
#include "com_debug.h"
#include "com_tool.h"
typedef struct 
{
    int16_t thr;
    int16_t yaw;    //偏航角 旋转
    int16_t pit;    //俯仰角 上下
    int16_t rol;    //横滚角 左右
    uint8_t shutdown;   //1 关机 0 不关机
    uint8_t fix_height; //1 切换定高和不定高 0 不切换
}Remote_data;

/*
 处理按键数据的函数声明 
*/
void App_process_key_data(void);
/*
 处理摇杆数据的函数声明 
*/
void App_process_joystick_data(void);

#endif

