#include "Com_pid.h"

/**
 * @brief 单次PID计算
 */
void Com_Pid_Calc(PID_t *pid)
{
    // 计算误差 测量减去误差
    pid->err = pid->measure - pid->desire;
    //计算积分
    pid->integral += pid->err * PID_PERIOD;
    
    if(pid->last_err == 0)
    {
        pid->last_err = pid->err;
    }
    // 计算微分
    float der = pid->err - pid->last_err;
    //计算输出
    pid->output = pid->kp * pid->err + pid->ki * pid->integral + pid->kd * der/PID_PERIOD;

    //保存上一次误差
    pid->last_err = pid->err;
}

/**
 * @brief 串级PID计算
 */
void Com_Pid_Calc_Chain(PID_t *out_pid,PID_t *in_pid)
{
    //先计算外环
    Com_Pid_Calc(out_pid);
    //将外环的输出值作为内环的目标值
    in_pid->desire = out_pid->output;
    //计算内环
    Com_Pid_Calc(in_pid);
}
/**
 * @brief 限制电机速度在正常的范围内
 */
int16_t Com_limit(int16_t speed,int16_t max_speed,int16_t min_speed)
{
    if(speed>max_speed)
    {
        return max_speed;
    }
    else if(speed<min_speed)
    {
        return min_speed;
    }
    return speed;
}
    

