#include "servoMotor.h"
#include "stdio.h"
#include "string.h"
#include "main.h"
#include "usart.h"

char cmd[15]; // 定义一个字符数组，用于存储发送的命令
void setdata(const char* num, const char* pwm, const char* time)
{
    // 原始固定标记
    const char header = '#';
    const char markP = 'P';
    const char markT = 'T';
    const char tail = '!';
    
    // 构造最终字符串：格式：
    // "#" + num + markP + pwm + markT + time + tail
    // 请确保 bufSize 足够大，否则会发生溢出
    snprintf(cmd, 16, "%c%s%c%s%c%s%c",
             header, num, markP, pwm, markT, time, tail);
}
void servoMotor_init(void)
{
    setdata(servoMotor1,release,"0500");
    HAL_UART_Transmit(&huart2, (uint8_t*)cmd, 16, 100);
    setdata(servoMotor2,init,"0500");
    HAL_UART_Transmit(&huart2, (uint8_t*)cmd, 16,100);
}

void catchServoMotor(void)
{
    setdata(servoMotor1,catch,"0500");
    HAL_UART_Transmit(&huart2, (uint8_t*)cmd, 16,100);
}
void releaseServoMotor(void)
{
    setdata(servoMotor1,release,"0500");
    HAL_UART_Transmit(&huart2, (uint8_t*)cmd, 16,100);
}
void initServoMotor(void)
{
    setdata(servoMotor2,init,"0500");
    HAL_UART_Transmit(&huart2, (uint8_t*)cmd, 16,100);
}

void rotateServoMotor(void)
{
   setdata(servoMotor2,"2500","0500");
    HAL_UART_Transmit(&huart2, (uint8_t*)cmd, 16,100);
}