#ifndef _APP_FREERTOS_TASK_H_
#define _APP_FREERTOS_TASK_H_

#include "FreeRTOS.h"
#include "task.h"
#include "com_debug.h"
#include "int_IP5305T.h"
#include "SI24R1.h"
#include "App_process_data.h"
#include "App_transmit_data.h"
/*
启动freeRTOS操作系统
*/
void App_freeRtos_Start(void);
#endif
