#include "int_MPU6050.h"

//保存偏移量的值
int32_t acc_x_offset = 0;
int32_t acc_y_offset = 0;
int32_t acc_z_offset = 0;
int32_t gyro_x_offset = 0;
int32_t gyro_y_offset = 0;
int32_t gyro_z_offset = 0;

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
 * @brief 在初始化MPU6050完成之后 计算出加速度传感器的零偏值 
 */
void MPU6050_calculate_offset(void)
{
    //等待飞机停放平稳 前后两次加速度的值差值小于200 连续100次
    Accel_struct current_accel={0};
    Accel_struct last_accel={0};
    uint8_t count =0;
    MPU6050_Get_Accel(&last_accel);

    while(count <100)
    {
        MPU6050_Get_Accel(&current_accel);
        if(abs(current_accel.accel_x - last_accel.accel_x) < 200 &&
           abs(current_accel.accel_y - last_accel.accel_y) < 200 &&
           abs(current_accel.accel_z - last_accel.accel_z) < 200)
        {
            count++;
        }
        else
        {
            count = 0;
        }
        last_accel = current_accel;
        vTaskDelay(6);
    }

    //已经平稳 开始进行零偏校准
    Gyro_Accel_struct gyro_accel_data={0};
    int32_t acc_x_offset_sum = 0;
    int32_t acc_y_offset_sum = 0;
    int32_t acc_z_offset_sum = 0;
    int32_t gyro_x_offset_sum = 0;
    int32_t gyro_y_offset_sum = 0;
    int32_t gyro_z_offset_sum = 0;
    for(uint8_t i=0;i<100;i++)
    {
        //重新读取加速度和角速度
        MPU6050_Get_Data(&gyro_accel_data);
        acc_x_offset_sum += (gyro_accel_data.accel.accel_x -0);
        acc_y_offset_sum += (gyro_accel_data.accel.accel_y -0);
        //z轴的零偏值是16384 因为MPU6050的加速度量程是2g 所以16384就是2g的值
        acc_z_offset_sum += (gyro_accel_data.accel.accel_z -16384);

        gyro_x_offset_sum += (gyro_accel_data.gyro.gyro_x - 0);
        gyro_y_offset_sum += (gyro_accel_data.gyro.gyro_y - 0);
        gyro_z_offset_sum += (gyro_accel_data.gyro.gyro_z - 0);
    }
    acc_x_offset = acc_x_offset_sum / 100;
    acc_y_offset = acc_y_offset_sum / 100;
    acc_z_offset = acc_z_offset_sum / 100;
    gyro_x_offset = gyro_x_offset_sum / 100;
    gyro_y_offset = gyro_y_offset_sum / 100;
    gyro_z_offset = gyro_z_offset_sum / 100;

    vTaskDelay(6);
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
    //9.进行零偏校准
    MPU6050_calculate_offset();
}



/**
 * @brief 读取三轴加速度 需要进行零偏校准
 */
void MPU6050_Get_Accel(Accel_struct *accel)
{
    uint8_t high =0;
    uint8_t low =0;
    //X轴
    MPU6050_Read_Data(MPU_ACCEL_XOUTH_REG,&high);
    MPU6050_Read_Data(MPU_ACCEL_XOUTL_REG,&low);
    accel->accel_x = (high << 8 | low);

    //Y轴
    MPU6050_Read_Data(MPU_ACCEL_YOUTH_REG,&high);
    MPU6050_Read_Data(MPU_ACCEL_YOUTL_REG,&low);
    accel->accel_y = (high << 8 | low);

    MPU6050_Read_Data(MPU_ACCEL_ZOUTH_REG,&high);
    MPU6050_Read_Data(MPU_ACCEL_ZOUTL_REG,&low);
    accel->accel_z = (high << 8 | low);
}
/**
 * @brief 读取三轴角速度
 */
void MPU6050_Get_Gyro(Gyro_struct *gyro)
{
    //存储角速度的寄存器地址从0x43开始 高八位在前 xyz的顺序
    uint8_t high =0 ;
    uint8_t low =0;
    MPU6050_Read_Data(MPU_GYRO_XOUTH_REG,&high);
    MPU6050_Read_Data(MPU_GYRO_XOUTL_REG,&low);
    gyro->gyro_x = (high << 8 | low);

    MPU6050_Read_Data(MPU_GYRO_YOUTH_REG,&high);
    MPU6050_Read_Data(MPU_GYRO_YOUTL_REG,&low);
    gyro->gyro_y = (high << 8 | low);

    MPU6050_Read_Data(MPU_GYRO_ZOUTH_REG,&high);
    MPU6050_Read_Data(MPU_GYRO_ZOUTL_REG,&low);
    gyro->gyro_z = (high << 8 | low);
}
/**
/**
 * @brief 读取六轴数据
 */
void MPU6050_Get_Data(Gyro_Accel_struct *data)
{
    MPU6050_Get_Accel(&data->accel);
    MPU6050_Get_Gyro(&data->gyro);
}
