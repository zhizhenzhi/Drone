#ifndef _APP_FLIGHT_H_
#define _APP_FLIGHT_H_

#include "int_MPU6050.h"
#include "com_debug.h"
#include "Com_filter.h"
#include "Com_imu.h"
#include "Com_pid.h"
#include "motor.h"
#include "math.h"
/**
 * @brief 根据陀螺仪测量的数据 计算出欧拉角
 */
void App_flight_get_euler_angle(void);

/**
 * @brief 根据欧拉角 计算PID的目标值
 */
void App_flight_pid_process(void);

/**
 * @brief 根据PID的输出值 控制电机
 */
void App_flight_motor_process(void);
void App_flight_init(void);

#endif
