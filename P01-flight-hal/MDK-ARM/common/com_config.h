#ifndef COM_CONFIG_H
#define COM_CONFIG_H

#include "main.h"
typedef enum{
    REMOTE_CONNECTED = 0,
    REMOTE_DISCONNECTED,
}Remote_State;

//飞行状态
typedef enum{
    IDLE=0,     //空闲状态
    NORMAL_FLIGHT,  //正常飞行状态
    FIX_HEIGHT,     //固定高度状态
    FAIL,           //失联状态
}Flight_State;

//油门解锁状态
typedef enum{
    FREE = 0,
    MAX,
    LEAVE_MAX,
    MIN,
    UNLOCK,
}Thr_state;
#endif 
