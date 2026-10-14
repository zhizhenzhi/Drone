#include "int_bat_ADC.h"

void Int_Bat_ADC_Init(void)
{
    //打开使能引脚
    HAL_GPIO_WritePin(BAT_ADC_EN_GPIO_Port,BAT_ADC_EN_Pin,GPIO_PIN_RESET);
    //启动ADC
    HAL_ADC_Start(&hadc1);
}

/**
 * @brief 获取电池电压
 * 
 * @param None
 */
float Int_bat_ADC_Get(void)
{
    uint32_t adc_value=0;
    adc_value=HAL_ADC_GetValue(&hadc1);
    //计算电压
    float voltage=adc_value*3.3/4095 *2;
    return voltage;
}