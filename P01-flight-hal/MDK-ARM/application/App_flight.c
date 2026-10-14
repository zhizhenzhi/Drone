#include "App_flight.h"
#include "App_receive_data.h"   //Remote_data 类型定义在此

Gyro_Accel_struct gyro_accel_data={0};
Euler_struct euler_angle={0};
Gyro_struct last_gyro={0};

extern Remote_data remote_data;
extern Flight_State flight_state;

motor_t left_top_motor ={.tim=&htim3, .channel=TIM_CHANNEL_1, .speed=200};
motor_t right_top_motor ={.tim=&htim2, .channel=TIM_CHANNEL_2, .speed=200};
motor_t left_bottom_motor ={.tim=&htim4, .channel=TIM_CHANNEL_4, .speed=200};
motor_t right_bottom_motor ={.tim=&htim1, .channel=TIM_CHANNEL_3, .speed=200};


//PID的调参先调内环再调外环
//俯仰角PID结构体
PID_t pitch_pid={.kp=0.00,.ki=0.00,.kd=0.00};
//Y轴角速度结构体 对应俯仰角的内环
PID_t gyro_y_pid={.kp=0.00,.ki=0.00,.kd=0.00};

/**
 * @brief 飞控任务初始化 MPU6050初始化
 */
void App_flight_init(void)
{
    MPU6050_Init();

    motor_start(&left_top_motor);
    motor_start(&left_bottom_motor);
    motor_start(&right_top_motor);
    motor_start(&right_bottom_motor);
}
/**
 * @brief 根据陀螺仪测量的数据 计算出欧拉角
 */
void App_flight_get_euler_angle(void)
{
    //使用MPU6050的硬件接口 得到六轴数据
    MPU6050_Get_Data(&gyro_accel_data);

    //对角速度进行低通滤波 后续对采集数据的使用是及时性比较高的
    //对准确性要求没那么高 但要计算迅速
    //output =加权系数 * last_output +(1-加权系数)*本次的测量值
    gyro_accel_data.gyro.gyro_x = Common_Filter_LowPass(gyro_accel_data.gyro.gyro_x,last_gyro.gyro_x);
    gyro_accel_data.gyro.gyro_y = Common_Filter_LowPass(gyro_accel_data.gyro.gyro_y,last_gyro.gyro_y);
    gyro_accel_data.gyro.gyro_z = Common_Filter_LowPass(gyro_accel_data.gyro.gyro_z,last_gyro.gyro_z);

    last_gyro.gyro_x = gyro_accel_data.gyro.gyro_x;
    last_gyro.gyro_y = gyro_accel_data.gyro.gyro_y;
    last_gyro.gyro_z = gyro_accel_data.gyro.gyro_z;

        // debug_print("%f\n",gyro_accel_data.gyro.gyro_x);

    //对波动变化大的加速度 使用更高级的滤波方式 卡尔曼滤波
    gyro_accel_data.accel.accel_x = Common_Filter_KalmanFilter(&kfs[0],gyro_accel_data.accel.accel_x);
    gyro_accel_data.accel.accel_y = Common_Filter_KalmanFilter(&kfs[1],gyro_accel_data.accel.accel_y);
    gyro_accel_data.accel.accel_z = Common_Filter_KalmanFilter(&kfs[2],gyro_accel_data.accel.accel_z);

    //通过加速度和角速度来计算当前飞机切斜的角度 姿态解算
    Common_IMU_GetEulerAngle(&gyro_accel_data,&euler_angle,0.006);
}


/**
 * @brief 根据欧拉角 计算PID的目标值
 */
void App_flight_pid_process(void)
{
    //俯仰角
    //需要赋值目标值和测量值
    //外环的目标角度 如果是平稳飞行值为0 如果需要遥控飞行 目标角度就是遥控器的值
    //数值转换 控制范围在正负十度
    pitch_pid.desire = (remote_data.pit - 500)/50.0;
    //外环的测量值 就是当前的俯仰角
    pitch_pid.measure = euler_angle.pitch;
    //内环的测量值 当前的角速度
    gyro_y_pid.measure = gyro_accel_data.gyro.gyro_y *2000.0/32768.0;
    //计算PID
    Com_Pid_Calc_Chain(&pitch_pid,&gyro_y_pid);

    
}

/**
 * @brief 根据PID的输出值 控制电机
 */
void App_flight_motor_process(void)
{
    //首先判断当前飞机的飞行状态
    switch (flight_state)
    {
    case IDLE:
        //一旦进入加锁状态 需要将电机停止
        left_top_motor.speed = 0;
        left_bottom_motor.speed = 0;
        right_top_motor.speed = 0;
        right_bottom_motor.speed = 0;
        break;
    case NORMAL_FLIGHT:
        //向前飞有角速度 正误差 需要一个向后飞的反馈效果 前两个电机转的快 后两个电机转的慢
        left_top_motor.speed = remote_data.thr + gyro_y_pid.output;
        left_bottom_motor.speed = remote_data.thr + gyro_y_pid.output;
        right_top_motor.speed = remote_data.thr + gyro_y_pid.output;
        right_bottom_motor.speed = remote_data.thr + gyro_y_pid.output;
        break;
    case FIX_HEIGHT:
        break;
    case FAIL:
        break;
    
    default:
        break;
    }

    //限制电机速度的上限值
    left_top_motor.speed = Com_limit(left_top_motor.speed,600,0);
    left_bottom_motor.speed = Com_limit(left_bottom_motor.speed,600,0);
    right_top_motor.speed = Com_limit(right_top_motor.speed,600,0);
    right_bottom_motor.speed = Com_limit(right_bottom_motor.speed,600,0);   
    //设置电机速度
    motor_set_speed(&left_top_motor);
    motor_set_speed(&left_bottom_motor);
    motor_set_speed(&right_top_motor);
    motor_set_speed(&right_bottom_motor);
}

