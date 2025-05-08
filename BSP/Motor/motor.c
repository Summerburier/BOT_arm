#include "motor.h"
#include "tim.h"
#include "stdlib.h"
static TIM_HandleTypeDef *htim_motor = &htim2;

void Motor_Init(uint32_t default_pulses) {
    // 配置TIM2通道2为翻转模式
    TIM_OC_InitTypeDef sConfigOC = {0};
    
    htim_motor->Instance->ARR = default_pulses * 2; // 翻转模式需要双倍计数值
    htim_motor->Instance->CCR2 = 0;                 // 初始比较值
    
    sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_OC_ConfigChannel(htim_motor, &sConfigOC, MOTOR1_PUL_CHANNEL);
    
    // 初始化GPIO状态
    HAL_GPIO_WritePin(MOTOR1_DIR_GPIO_Port, MOTOR1_DIR_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR1_ENA_GPIO_Port, MOTOR1_ENA_Pin, GPIO_PIN_RESET);
}

void Motor_Run(int32_t pulses) {
    // 设置方向（根据脉冲符号）
    GPIO_PinState dir = (pulses >= 0) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    HAL_GPIO_WritePin(MOTOR1_DIR_GPIO_Port, MOTOR1_DIR_Pin, dir);
    
    // 计算实际脉冲数
    uint32_t pulse_count = abs(pulses) * 2; // 每个脉冲需要两次翻转
    
    // 更新定时器参数
    __HAL_TIM_SET_AUTORELOAD(htim_motor, pulse_count);
    __HAL_TIM_SET_COMPARE(htim_motor, MOTOR1_PUL_CHANNEL, 0);
    
    // 使能电机并启动定时器
    HAL_GPIO_WritePin(MOTOR1_ENA_GPIO_Port, MOTOR1_ENA_Pin, GPIO_PIN_SET);
    HAL_TIM_OC_Start_IT(htim_motor, MOTOR1_PUL_CHANNEL);
    
    // 等待脉冲完成
    while(!__HAL_TIM_GET_FLAG(htim_motor, TIM_FLAG_CC2));
    __HAL_TIM_CLEAR_FLAG(htim_motor, TIM_FLAG_CC2);
    
    // 停止输出
    HAL_TIM_OC_Stop_IT(htim_motor, MOTOR1_PUL_CHANNEL);
    HAL_GPIO_WritePin(MOTOR1_ENA_GPIO_Port, MOTOR1_ENA_Pin, GPIO_PIN_RESET);
}