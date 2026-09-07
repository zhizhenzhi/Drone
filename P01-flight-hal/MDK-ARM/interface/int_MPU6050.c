#include "int_MPU6050.h"



/**
 * @brief 写入MPU6050传感器数据
 * @param reg 寄存器地址
 * @param data 数据
 */
void MPU6050_Write_Data(uint8_t reg,uint8_t data)
{
    //参数1：句柄I2C1 2：MPU6050地址 3：寄存器地址 4：寄存器地址长度 5：数据 6：数据长度 7：超时时间
    HAL_I2C_Mem_Write(&hi2c1,MPU6050_ADDR_WRITE,reg,I2C_MEMADD_SIZE_8BIT,&data,1,1000);
}

/**
 * @brief 读取MPU6050传感器数据
 */
void MPU6050_Read_Data(uint8_t reg,uint8_t *data)
{
    //参数 1：句柄I2C1 2：MPU6050地址 3：寄存器地址 4：寄存器地址长度 5：存放读取数据的地址 6：数据长度 7：超时时间
    HAL_I2C_Mem_Read(&hi2c1,MPU6050_ADDR_READ,reg,I2C_MEMADD_SIZE_8BIT,data,1,1000);
}

/**
 * @brief 初始化MPU6050传感器
 */
void MPU6050_Init(void)
{
    //1.重置所有寄存器的值
    MPU6050_Write_Data(0x6B,0x80);
    uint8_t data = 0;
    //重置完成之后 0x6b寄存器的值是0x40 表示当前是低功耗模式
    while(data != 0x40)
    {
        MPU6050_Read_Data(0x6B,&data);
    }
    //唤醒MPU6050 进入到正常工作状态
    MPU6050_Write_Data(0x6B,0x00);

    //2.选择合适的角速度量程2000
    MPU6050_Write_Data(0x1B,0x18);
    //选择加速度量程2g
    MPU6050_Write_Data(0x1C,0x00);
    //3.关闭中断使能
    MPU6050_Write_Data(0x38,0x00);
    //4.用户配置寄存器
    MPU6050_Write_Data(0x6A,0x00);
    //5.设置采样率为500Hz -》陀螺仪监控三轴加速度和三轴角速度
    MPU6050_Write_Data(0x19,0x01);
    //6.设置低通滤波为184HZ 188HZ
    MPU6050_Write_Data(0x1A,1);
    //7.配置使用的系统时钟为添加PLL
    MPU6050_Write_Data(0x6B,0x01);
    //8.使能加速度传感器和角速度传感器
    MPU6050_Write_Data(0x6C,0x00);
}