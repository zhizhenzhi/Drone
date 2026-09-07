#ifndef APP_TRANSMIT_DATA_H
#define APP_TRANSMIT_DATA_H

#include "SI24R1.h"
#include "App_process_data.h"
#include "FreeRTOS.h"
#include "task.h"
//定义帧头校验的值
#define FRAME_HEADER_CHECK1 'j'
#define FRAME_HEADER_CHECK2 'y'
#define FRAME_HEADER_CHECK3 'f'
/*
自动切换SI24R1的模式 将采集完成的数据发送给飞机
*/
void app_transmit_data_process(void);

#endif
