#ifndef APP_RECEIVE_DATA_H
#define APP_RECEIVE_DATA_H

#include "SI24R1.h"
#include "com_config.h"
#include <string.h>

#define FRAME_HEADER_CHECK1 'j'
#define FRAME_HEADER_CHECK2 'y'
#define FRAME_HEADER_CHECK3 'f'

//最大重试次数
#define MAX_RETRY_TIMES 5

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
接收遥控器发送的数据解析为结构体
@return 0:校验通过
*/
uint8_t App_receive_data(void);

/*
@brief 处理连接状态
@param 上一次接收数据的返回值
*/
void App_process_connect_state(uint8_t res);

/*
处理飞机飞行状态
*/
void App_process_flight_state(void);

#endif
