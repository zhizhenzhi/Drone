#ifndef APP_DISPALY_H
#define APP_DISPALY_H

#include "Inf_OLED.h"
#include "SI24R1.h"
#include "App_process_data.h"

#define LINE1_BEGIN 28
#define LINE2_BEGIN 5
#define LINE3_BEGIN 5

#define BAR1_BEGIN 35
#define BAR2_BEGIN 47

#define LINE3_BEGIN2 65
#define BAR1_BEGIN2 95
#define BAR2_BEGIN2 107

#define LINE4_BEGIN 5
#define LINE4_BEGIN2 65

#define Y0 0
#define Y1 14
#define Y2 26
#define Y3 38
/**
 * @brief 初始化显示模块
 */
void App_display_init(void);
/**
 * @brief 循环执行刷写屏幕
 */
void App_display_show(void);

#endif //APP_DISPALY_H
