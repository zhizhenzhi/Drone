#include "com_tool.h"

/*
限幅函数
*/
int16_t com_limit(int16_t value,int16_t min,int16_t max)
{
    int16_t result =value;
    if(value>max)
    {
        return max;
    }
    else if(value<min)
    {
        return min;
    }
    else
    {
        return result;
    }
}
