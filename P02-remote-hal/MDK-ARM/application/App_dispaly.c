#include "App_dispaly.h"
extern Remote_data remote_data;

void App_display_show_bar(uint8_t x,uint8_t y,uint8_t count)
{
    if(count <13)
    {
        OLED_Show_CH(x,y,12 +count,12,1);
    }
}
/**
 * @brief 初始化显示模块
 */
void App_display_init(void)
{
    OLED_Init();
}
/**
 * @brief 循环执行刷写屏幕
 */
void App_display_show(void)
{
    //X坐标 Y坐标 显示内容 字体大小 显示模式
    for(uint8_t i=0;i<6;i++)
    {
        OLED_Show_CH(LINE1_BEGIN+12*i,Y0,i,12,1);
    }

    //第二行
    uint8_t buff[3]={0};
    sprintf((char *)buff,"%03d",CHANNEL);
    OLED_ShowString(LINE2_BEGIN,Y1,buff,12,1);

    uint8_t count = 0;
    //第三行 展示遥控数据 THR ROL
    OLED_ShowString(LINE3_BEGIN,Y2,"THR:",12,1);
    if(remote_data.thr >500)
    {
        count = (remote_data.thr - 500)/41;
        //前一个汉字字符 涂满
        App_display_show_bar(BAR1_BEGIN,Y2,12);
        App_display_show_bar(BAR2_BEGIN,Y2,count);
    }
    else 
    {
        count = (500 - remote_data.thr)/41;
        //后一个汉字字符 涂满
        App_display_show_bar(BAR1_BEGIN,Y2,count);
        App_display_show_bar(BAR2_BEGIN,Y2,0);
    }

    OLED_ShowString(LINE3_BEGIN2,Y2,"ROL:",12,1);
    if(remote_data.rol >500)
    {
        count = (remote_data.rol - 500)/41;
        //前一个汉字字符 涂满
        App_display_show_bar(BAR1_BEGIN2,Y2,12);
        App_display_show_bar(BAR2_BEGIN2,Y2,count);
    }
    else 
    {
        count = (500 - remote_data.rol)/41;
        //后一个汉字字符 涂满
        App_display_show_bar(BAR1_BEGIN2,Y2,count);
        App_display_show_bar(BAR2_BEGIN2,Y2,0);
    }

    //第四行 YAW PIT
    OLED_ShowString(LINE4_BEGIN,Y3,"YAW:",12,1);
    if(remote_data.yaw >500)
    {
        count = (remote_data.yaw - 500)/41;
        //前一个汉字字符 涂满
        App_display_show_bar(BAR1_BEGIN,Y3,12);
        App_display_show_bar(BAR2_BEGIN,Y3,count);
    }
    else 
    {
        count = (500 - remote_data.yaw)/41;
        //后一个汉字字符 涂满
        App_display_show_bar(BAR1_BEGIN,Y3,count);
        App_display_show_bar(BAR2_BEGIN,Y3,0);
    }

    OLED_ShowString(LINE4_BEGIN2,Y3,"PIT:",12,1);
    if(remote_data.pit >500)
    {
        count = (remote_data.pit- 500)/41;
        //前一个汉字字符 涂满
        App_display_show_bar(BAR1_BEGIN2,Y3,12);
        App_display_show_bar(BAR2_BEGIN2,Y3,count);
    }
    else 
    {
        count = (500 - remote_data.pit)/41;
        //后一个汉字字符 涂满
        App_display_show_bar(BAR1_BEGIN2,Y3,count);
        App_display_show_bar(BAR2_BEGIN2,Y3,0);
    }
    OLED_Refresh_Gram();
}
