#ifndef _INT_JOYSTICK_H_
#define _INT_JOYSTICK_H_

#include "adc.h"

typedef struct {
    int16_t thr;    //油门
    int16_t yaw;    //偏航角 顺时针逆时针旋转
    int16_t pit;    //俯仰角 上下旋转
    int16_t rol;    //横滚角 左右旋转
} Int_Joystick_t;
/*
@brief 初始化摇杆
*/
void Int_Joystick_Init(void);
/*
@brief 读取ADC数据 保存到结构体地址中
*/
void Int_Joystick_Get_Value(Int_Joystick_t *joystick);
#endif 
