#include "App_freeRtos_Task.h"



//创建电源管理任务
//SRAM 20K 分配12K给操作系统
void power_task(void *args);
#define POWER_TASK_STACK_SIZE 128
//任务优先级 数值越小优先级越小  最大4 不推荐使用最小优先级0
#define POWER_TASK_PRIORITY 4
TaskHandle_t power_task_handle;
#define POWER_TASK_PERIOD 10000

//通讯任务
void com_task(void *args);
TaskHandle_t com_task_handle;
#define COM_TASK_PRIORITY 3
#define COM_TASK_STACK_SIZE 128
#define COM_TASK_PERIOD 6

//按键任务
void key_task(void *args);
TaskHandle_t key_task_handle;
#define KEY_TASK_STACK_SIZE 128
#define KEY_TASK_PRIORITY 2
#define KEY_TASK_PERIOD 20

//摇杆任务
void joystick_task(void *args);
TaskHandle_t joystick_task_handle;
#define JOYSTICK_TASK_STACK_SIZE 128
#define JOYSTICK_TASK_PRIORITY 2
#define JOYSTICK_TASK_PERIOD 20

//屏幕任务
void oled_task(void *args);
TaskHandle_t oled_task_handle;
#define OLED_TASK_STACK_SIZE 128
#define OLED_TASK_PRIORITY 1
#define OLED_TASK_PERIOD 100
void App_freeRtos_Start(void)
{
    //创建任务
    xTaskCreate(power_task,"power_task",POWER_TASK_STACK_SIZE,NULL,POWER_TASK_PRIORITY,&power_task_handle);
    xTaskCreate(com_task,"com_task",COM_TASK_STACK_SIZE,NULL,COM_TASK_PRIORITY,&com_task_handle);
    xTaskCreate(key_task,"key_task",KEY_TASK_STACK_SIZE,NULL,KEY_TASK_PRIORITY,&key_task_handle);
    xTaskCreate(joystick_task,"joystick_task",JOYSTICK_TASK_STACK_SIZE,NULL,JOYSTICK_TASK_PRIORITY,&joystick_task_handle);
    xTaskCreate(oled_task,"oled_task",OLED_TASK_STACK_SIZE,NULL,OLED_TASK_PRIORITY,&oled_task_handle);
    //启动调度器
    vTaskStartScheduler();
}

void power_task(void *args)
{
    //获取当前的基准时间
    TickType_t last_wake_time = xTaskGetTickCount();
    while(1)
    {
        
        //每10s执行一次 启动电源避免自动关机
        vTaskDelayUntil(&last_wake_time,POWER_TASK_PERIOD);
        //启动电源
        IP5305T_start();
    }
}

void joystick_task(void *args)
{
    //获取当前的基准时间
    TickType_t last_wake_time = xTaskGetTickCount();
    //初始化摇杆ADC
    Int_Joystick_Init();
    while(1)
    {
        
        App_process_joystick_data();
        vTaskDelayUntil(&last_wake_time,POWER_TASK_PERIOD);
    }
}
void key_task(void *args)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    while(1)
    {
       
        App_process_key_data();
         vTaskDelayUntil(&last_wake_time,KEY_TASK_PERIOD);
    }
}

uint8_t com_buff[TX_PLOAD_WIDTH]={0};
void com_task(void *args)
{
    //获取当前的基准时间
    TickType_t last_wake_time = xTaskGetTickCount();
    while(1)
    {
        //将打包完成的数据发送给飞机
        app_transmit_data_process();
        //每6ms执行一次 启动电源避免自动关机
        vTaskDelayUntil(&last_wake_time,COM_TASK_PERIOD);
    }
}

void oled_task(void *args)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    while(1)
    {
        App_display_init();
        App_display_show();
        vTaskDelayUntil(&last_wake_time,OLED_TASK_PERIOD);
    }
}
