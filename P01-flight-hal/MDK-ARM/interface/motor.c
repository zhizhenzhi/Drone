#include "motor.h"
#include "stm32f1xx_hal_tim.h"

/**
 * @brief 设置电机速度
 * @param motor 电机结构体指针
 */
void motor_set_speed(motor_t *motor)
{
    if(motor->speed>1000)
    {
        debug_printf("motor speed is too high\n");
        return;
    }
    __HAL_TIM_SET_COMPARE(motor->tim,motor->channel,motor->speed);
}

/**
 * @brief 启动电机
 * @param motor 电机结构体指针
 */
void motor_start(motor_t *motor)
{
    HAL_TIM_PWM_Start(motor->tim,motor->channel);
}
