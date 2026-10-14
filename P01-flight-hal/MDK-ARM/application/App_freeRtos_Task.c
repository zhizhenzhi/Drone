#include "App_freeRtos_Task.h"

//SRAM 20K 分配12K给操作系统
//内存管理 C语言结构体通常保存在堆中 不会自动垃圾回收 始终使用同一个结构体 不断循环使用

//LED结构体
led_t led1 = {.port=LED1_GPIO_Port, .pin=LED1_Pin};
led_t led2 = {.port=LED2_GPIO_Port, .pin=LED2_Pin};
led_t led3 = {.port=LED3_GPIO_Port, .pin=LED3_Pin};
led_t led4 = {.port=LED4_GPIO_Port, .pin=LED4_Pin};


//表示当前连接状态
Remote_State remote_state = REMOTE_CONNECTED;
//表示当前飞行状态
Flight_State flight_state = IDLE;
//扩展获取接收的遥控数据
Remote_data remote_data = {.thr=0,.yaw=0,.pit=0,.rol=0,.shutdown=0};

#define TASK1_STACK_SIZE 128
//任务优先级 数值越小优先级越小  最大4 不推荐使用最小优先级0
#define POWER_TASK_PRIORITY 4
#define FLIGHT_TASK_PRIORITY 3
#define LED_TASK_PRIORITY 1
#define COM_TASK_PRIORITY 3
//定义任务的周期
#define POWER_TASK_PERIOD 1000
#define FLIGHT_TASK_PERIOD 6
#define LED_TASK_PERIOD 100
#define COM_TASK_PERIOD 6

TaskHandle_t power_task_handle;
TaskHandle_t flight_task_handle;
TaskHandle_t led_task_handle;
TaskHandle_t com_task_handle;


void power_task(void *args);
void flight_task(void *args);
void led_task(void *args);
void com_task(void *args);
void App_freeRtos_Start(void)
{
    //1.创建任务
    xTaskCreate(power_task,"power_task",TASK1_STACK_SIZE,NULL,POWER_TASK_PRIORITY,&power_task_handle);
    //2.创建飞行控制任务
    xTaskCreate(flight_task,"flight_task",TASK1_STACK_SIZE,NULL,FLIGHT_TASK_PRIORITY,&flight_task_handle);
    //3.创建LED任务
    xTaskCreate(led_task,"led_task",TASK1_STACK_SIZE,NULL,LED_TASK_PRIORITY,&led_task_handle);
    //4.创建通信任务
    xTaskCreate(com_task,"com_task",TASK1_STACK_SIZE,NULL,COM_TASK_PRIORITY,&com_task_handle);
    //启动调度器
    vTaskStartScheduler();
}

void power_task(void *args)
{
    // //获取当前的基准时间
    // TickType_t last_wake_time = xTaskGetTickCount();
    // while(1)
    // {
        
    //     //每10s执行一次 启动电源避免自动关机
    //     vTaskDelayUntil(&last_wake_time,POWER_TASK_PERIOD);
    //     //启动电源
    //     IP5305T_start();
    // }

    //使用直接任务通知的接收方法实现10s处理一次
    //一直等通知 直到任务通知或超时
    uint32_t res =ulTaskNotifyTake(pdTRUE,POWER_TASK_PERIOD);
    if(res!=0)
    {
        //收到关机通知
        IP5305T_shutdown();
    }
    else
    {
        IP5305T_start();
    }
}

void flight_task(void *args)
{
     //获取当前的基准时间
    TickType_t last_wake_time = xTaskGetTickCount();
    App_flight_init();
    while(1)
    {
        // motor_start(&left_top_motor);
        // motor_start(&right_top_motor);
        // motor_start(&left_bottom_motor);
        // motor_start(&right_bottom_motor);
        //1.根据MPU6050测量的数据 姿态解算获取欧拉角
        App_flight_get_euler_angle();
        //2.根据姿态解算的欧拉角 计算PID输出值
        App_flight_pid_process();
        //3.根据PID计算的结果 对电机进行控制
        App_flight_motor_process();
        //每10s执行一次 启动电源避免自动关机
        vTaskDelayUntil(&last_wake_time,FLIGHT_TASK_PERIOD);

    }
}

void led_task(void *args)
{
     //获取当前的基准时间
    TickType_t last_wake_time = xTaskGetTickCount();
    uint8_t count = 0;
    while(1)
    {
        count++;
        //前两个灯表示连接状态
        if(remote_state==REMOTE_CONNECTED)
        {
            led_turn_on(&led1);
            led_turn_on(&led2);
        }
        else
        {
            led_turn_off(&led1);
            led_turn_off(&led2);
        }
        //后两个灯表示飞行状态
        if(flight_state==IDLE)
        {
            //灯慢闪 500ms亮 500ms灭
            if(count%5==0)
            {
                //循环5次 一次100ms 5次500ms
                led_toggle(&led3);
                led_toggle(&led4);
            }
        }
        else if(flight_state==NORMAL_FLIGHT)
        {
           //灯快闪 200ms亮 200ms灭
           if(count%2==0)
            {
                //循环2次 一次200ms 2次400ms
                led_toggle(&led3);
                led_toggle(&led4);
            }
        }
        else if(flight_state==FIX_HEIGHT)
        {
            led_turn_on(&led3);
            led_turn_on(&led4);
        }
        else if(flight_state==FAIL)
        {
            led_turn_off(&led3);
            led_turn_off(&led4);
        }
        //将count计数重置
        if(count==10)
        {
            count = 0;
        }
        //每10s执行一次 启动电源避免自动关机
        vTaskDelayUntil(&last_wake_time,LED_TASK_PERIOD);
    }
}

uint8_t com_data[TX_PLOAD_WIDTH + 1]={0};
void com_task(void *args)
{
    //获取当前的基准时间
    TickType_t last_wake_time = xTaskGetTickCount();
    while(1)
    {
       uint8_t res = App_receive_data();

       //根据接收数据的返回值 处理当前飞机的连接状态
       App_process_connect_state(res);

       //处理关机命令
       if(remote_data.shutdown == 1)
       {
            //不建议
            //IP5305T_shutdown();

            //使用直接任务通知 -》通知电源任务执行关机
            xTaskNotifyGive(power_task_handle);
       }

       //处理飞机飞行状态
       App_process_flight_state();
        //每6ms执行一次 启动电源避免自动关机
        vTaskDelayUntil(&last_wake_time,COM_TASK_PERIOD);
    }
}

