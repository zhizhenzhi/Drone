#include "Int_joystick.h"

uint16_t adc_buff[4]={0};
/*
@brief 初始化摇杆
*/
void Int_Joystick_Init(void){
    //直接使用HAL中的函数 开启ADC
    HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adc_buff,4);
}
/*
@brief 读取ADC数据 保存到结构体地址中
*/
void Int_Joystick_Get_Value(Int_Joystick_t *joystick)
{
    //DMA不依赖与CPU的计算的 所以读取的数据是实时保存到ADC_BUFF中
    //顺序是自定义的
    joystick->thr = adc_buff[0];
    joystick->yaw = adc_buff[1];
    joystick->pit = adc_buff[2];
	joystick->rol = adc_buff[3];
}