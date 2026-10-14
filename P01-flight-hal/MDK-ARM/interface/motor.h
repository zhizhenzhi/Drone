#ifndef MOTOR_H
#define MOTOR_H

#include "tim.h"
#include "com_debug.h"

typedef struct {
    TIM_HandleTypeDef *tim;
    uint16_t channel;
    int16_t speed;
} motor_t;
//传入的参数是比较值 最大1000 默认200
void motor_set_speed(motor_t *motor);

void motor_start(motor_t *motor);

#endif 
