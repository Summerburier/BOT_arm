#ifndef __STEP_MOTOR_H__
#define __STEP_MOTOR_H__

#include "main.h"
#include "tim.h"
#include <stdbool.h>

// ==== 预留定时器和通道宏定义（请根据实际CubeMX配置填写） ====
#define MOTOR0_TIM      htim2
#define MOTOR0_CHANNEL  TIM_CHANNEL_2

#define MOTOR1_TIM      htim3
#define MOTOR1_CHANNEL  TIM_CHANNEL_2

#define MOTOR2_TIM      htim4
#define MOTOR2_CHANNEL  TIM_CHANNEL_2

#define MOTOR3_TIM      htim5
#define MOTOR3_CHANNEL  TIM_CHANNEL_2

// ==== 电机方向控制宏 ====
#define MOTOR0_DIR_CHANGE(x)  HAL_GPIO_WritePin(MOTOR0_DIR_GPIO_Port, MOTOR0_DIR_Pin, (x)?GPIO_PIN_SET:GPIO_PIN_RESET)
#define MOTOR1_DIR_CHANGE(x)  HAL_GPIO_WritePin(MOTOR1_DIR_GPIO_Port, MOTOR1_DIR_Pin, (x)?GPIO_PIN_SET:GPIO_PIN_RESET)
#define MOTOR2_DIR_CHANGE(x)  HAL_GPIO_WritePin(MOTOR2_DIR_GPIO_Port, MOTOR2_DIR_Pin, (x)?GPIO_PIN_SET:GPIO_PIN_RESET)
#define MOTOR3_DIR_CHANGE(x)  HAL_GPIO_WritePin(MOTOR3_DIR_GPIO_Port, MOTOR3_DIR_Pin, (x)?GPIO_PIN_SET:GPIO_PIN_RESET)

// ==== 电机使能控制宏 ====
#define MOTOR0_EN(x)  HAL_GPIO_WritePin(MOTOR0_ENA_GPIO_Port, MOTOR0_ENA_Pin, (x)?GPIO_PIN_SET:GPIO_PIN_RESET)
#define MOTOR1_EN(x)  HAL_GPIO_WritePin(MOTOR1_ENA_GPIO_Port, MOTOR1_ENA_Pin, (x)?GPIO_PIN_SET:GPIO_PIN_RESET)
#define MOTOR2_EN(x)  HAL_GPIO_WritePin(MOTOR2_ENA_GPIO_Port, MOTOR2_ENA_Pin, (x)?GPIO_PIN_SET:GPIO_PIN_RESET)
#define MOTOR3_EN(x)  HAL_GPIO_WritePin(MOTOR3_ENA_GPIO_Port, MOTOR3_ENA_Pin, (x)?GPIO_PIN_SET:GPIO_PIN_RESET)

// ==== 电机状态 ====
typedef enum {
    IDLE = 0,
    ACCEL,
    UNIFORM,
    DECEL,
    STOP
} MotorState;

typedef enum {
    FREE = 0,
    VELOCITY,
    POSITION
} MotorMode;

typedef enum {
    FORWARD = 0,
    BACKWARD = 1
} DIR;

// ==== 电机控制结构体 ====
typedef struct {
    uint8_t id; // 电机编号 0~3
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    // 运动参数
    int32_t v0, v1;
    float accTime, decTime;
    int32_t step, pos;
    float *accTab, *decTab;
    bool accTabMalloced, decTabMalloced;
    volatile MotorState state;
    MotorMode mode;
    DIR dir;
    // 方向引脚和使能引脚直接用main.h的宏，不再重复定义
    GPIO_TypeDef* dir_port;
    uint16_t dir_pin;
    GPIO_TypeDef* en_port;
    uint16_t en_pin;
} MotorControltype;

// ==== 4个电机实例 ====
extern MotorControltype motor0;
extern MotorControltype motor1;
extern MotorControltype motor2;
extern MotorControltype motor3;

// ==== 曲线加减速接口 ====
void motorCurveMove(MotorControltype *motor, int32_t v1, float accTime, float decTime, int32_t step);
void motorStateUpgrade(MotorControltype *motor);
// ==== 电机初始化接口 ====
void motorInit(void);


#endif
