#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f4xx_hal.h"
#include "main.h" // 包含引脚定义

// 使用TIM2通道2（根据PA7的复用功能选择）
#define MOTOR1_PUL_TIM      TIM2
#define MOTOR1_PUL_CHANNEL  TIM_CHANNEL_2 // PA7对应TIM2_CH2

void Motor_Init(uint32_t default_pulses);
void Motor_Run(int32_t pulses);
#endif