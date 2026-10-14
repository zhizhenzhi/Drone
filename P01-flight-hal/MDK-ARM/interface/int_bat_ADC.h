#ifndef __INT_BAT_ADC_H__
#define __INT_BAT_ADC_H__

#include "adc.h"

/**
 * @brief 初始化ADC
 * 
 * @param None
 */
void Int_bat_ADC_Init(void);

/**
 * @brief 获取电池电压
 * 
 */
float Int_bat_ADC_Get(void);

#endif
