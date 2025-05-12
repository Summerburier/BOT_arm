#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "tim.h"
#define COUNT_PWM_TIMER htim1
#define PWM_CHANNEL     TIM_CHANNEL_2

// prescaler 越小越快
#define MAX_PRESCALER   (100)
#define MIN_PRESCALER   (25)
#define MCU_FREQUENCY   (168000000.0f)

#define STEP_PULSE      (1600) // 一圈需要1600个脉冲
#define PARAM_A(x)      (4.0f * (MAX_PRESCALER - MIN_PRESCALER) / (x) / (x))

extern __IO uint8_t done_flag;

// void HAL_Count_PWM_Generator_Init(void);
void HAL_Count_PWM_Generate(uint16_t count);
void HAL_Count_PWM_Generator_Callback(TIM_HandleTypeDef *htim);
void HAL_ShelfTurn(uint8_t dir, uint16_t count);

#endif // !