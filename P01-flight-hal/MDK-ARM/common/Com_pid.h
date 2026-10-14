#ifndef COM_PID_H
#define COM_PID_H

#define PID_PERIOD 0.006
#include "main.h"

typedef struct {
    float kp; //比例 值越大响应速度越快
    float ki; //积分 解决稳态误差
    float kd; //微分部分 值越大 抑制效果越好 
    float err; //误差值
    float desire; //目标值
    float measure; //测量值
    float last_err; //上一次的误差
    float integral; //积分累积
    float output; //输出结果
} PID_t;

/**
 * @brief 单次PID计算
 */
void Com_Pid_Calc(PID_t *pid);

/**
 * @brief 串级PID计算
 */
void Com_Pid_Calc_Chain(PID_t *out_pid,PID_t *in_pid);
/**
 * @brief 限制电机速度在正常的范围内
 */
int16_t Com_limit(int16_t speed,int16_t max_speed,int16_t min_speed);
#endif
