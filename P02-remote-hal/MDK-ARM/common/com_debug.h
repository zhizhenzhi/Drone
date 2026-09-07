#ifndef _COM_DEBUG_H
#define _COM_DEBUG_H

#include "usart.h"
#include "stdio.h"
#include "stdarg.h"

//通过输出打印在CPU上运行非常占用资源-》打印10字节左右大概需要1ms，非常影响飞机的飞行，所以后续飞机正常飞行时候，需要关闭打印功能
#define DEBUG_LOG_ENABLE  1
#ifdef DEBUG_LOG_ENABLE
//使用宏定义的方式能实现打印日志之前 先添加文件名和行号
#define debug_print(fm,...) printf( "[%s:%d] " fm , __FILE__, __LINE__, ##__VA_ARGS__)
#define debug_printf(fm,...) printf( "[%s:%d] " fm , __FILE__, __LINE__, ##__VA_ARGS__)

#else
#define debug_print(fm,...)
#define debug_printf(fm,...)
#endif

#endif
