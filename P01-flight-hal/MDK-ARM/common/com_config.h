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

//陀螺仪数据
typedef struct{
    uint16_t gyro_x;  //往右飞为正 表示横滚角
    uint16_t gyro_y;  //向前飞为正 表示俯仰角
    uint16_t gyro_z;  //逆时针为正 表示偏航角
}Gyro_struct;

typedef struct
{
    uint16_t accel_x; //向前为正
    uint16_t accel_y; //向左为正
    uint16_t accel_z; //朝上加速度为正
}Accel_struct;

typedef struct 
{
    Gyro_struct gyro;
    Accel_struct accel;
}Gyro_Accel_struct;
//解算得到的欧拉角
typedef  struct 
{
    float yaw;
    float pitch;
    float roll;
}Euler_struct;

#endif 
