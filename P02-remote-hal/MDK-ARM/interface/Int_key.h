#ifndef _INT_KEY_H_
#define _INT_KEY_H_

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
typedef enum
{
    KEY_NONE=0,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_LEFT_X,
    KEY_RIGHT_X,
    KEY_RIGHT_X_LONG,
}Key_type;

/*
@brief 获取当前按键是否被按下
@return NONE：没有按键被按下 其他都是对应被按键的标记
*/
Key_type Key_Get(void);
#endif
