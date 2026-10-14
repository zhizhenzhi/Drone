#ifndef _APP_FREERTOS_TASK_H_
#define _APP_FREERTOS_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "com_debug.h"
#include "int_IP5305T.h"
#include "motor.h"
#include "led.h"
#include "com_config.h"
#include "SI24R1.h"
#include "App_receive_data.h"
#include "App_flight.h"
#include "int_bat_ADC.h"
/*
启动freeRTOS操作系统
*/
void App_freeRtos_Start(void);
#endif
