#ifndef __COMMON_FILTER_H
#define __COMMON_FILTER_H
#include "Com_debug.h"


/*
均值和期望：求平均值
方差和标准差：一组数据在平均值附近波动的幅度 方差越大 波动越强
准度：测量值与真实值的偏差 偏差越小 准度越高
精度：一系列测量值和相对同一个真值的偏差分布情况
卡尔曼滤波实现：初始化 记录一组初始化状态值
测量 获取当前一次加速度 方差
使用初始化的K值 得到当前一次的输出值 记录结果的方差
下一次测量之前 更新状态 重新计算K增益系数
测量 
*/
/* 卡尔曼滤波器结构体 */
typedef struct
{
    float LastP; // 上一时刻的状态方差（或协方差）
    float Now_P; // 当前时刻的状态方差（或协方差）
    float out;   // 滤波器的输出值，即估计的状态
    float Kg;    // 卡尔曼增益，用于调节预测值和测量值之间的权重
    float Q;     // 过程噪声的方差，反映系统模型的不确定性
    float R;     // 测量噪声的方差，反映测量过程的不确定性
} KalmanFilter_Struct;

extern KalmanFilter_Struct kfs[3];
int16_t Common_Filter_LowPass(int16_t newValue, int16_t preFilteredValue);

double Common_Filter_KalmanFilter(KalmanFilter_Struct *kf, double input);

#endif
