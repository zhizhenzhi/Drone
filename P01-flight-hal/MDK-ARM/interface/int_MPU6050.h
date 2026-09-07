#ifndef _INT_MPU6050_H_
#define _INT_MPU6050_H_

#include "i2c.h"
//从设备地址
#define MPU6050_ADDR 0x68
//读写地址
#define MPU6050_ADDR_WRITE 0xD0
#define MPU6050_ADDR_READ  0xD1

/**
 * @brief 初始化MPU6050传感器
 */
void MPU6050_Init(void);
void MPU6050_Read_Data(uint8_t reg,uint8_t *data);
void MPU6050_Write_Data(uint8_t reg,uint8_t data);
#endif // _INT_MPU6050_H_

