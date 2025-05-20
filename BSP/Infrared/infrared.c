#include "infrared.h"
#include "delay.h"  // delay_ms 函数所在头文件




u8 Scan(u8 mode)
{
    static u8 key_up=1;     //按键松开标志
    if(mode==1)key_up=1;    //支持连按
    if(key_up&&(INFRARED==0))
    {
        delay_ms(10);
        key_up=0;
        if(INFRARED==1)       return INFRARED_PRES;    
    }
    return 0;   //无按键按下
}
