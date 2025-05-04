#include "stepMotor.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ==== 4个电机实例 ====
MotorControltype motor0;
MotorControltype motor1;
MotorControltype motor2;
MotorControltype motor3;

// ==== 一次性初始化4个电机，全部信息用main.h宏 ====
void motorInit(void)
{
    motor0.id = 0;
    motor0.htim = &MOTOR0_TIM;
    motor0.channel = MOTOR0_CHANNEL;
    motor0.dir_port = MOTOR0_DIR_GPIO_Port;
    motor0.dir_pin = MOTOR0_DIR_Pin;
    motor0.en_port = MOTOR0_ENA_GPIO_Port;
    motor0.en_pin = MOTOR0_ENA_Pin;
    motor0.state = IDLE;
    motor0.mode = FREE;
    motor0.dir = FORWARD;
    motor0.accTab = NULL;
    motor0.decTab = NULL;
    motor0.accTabMalloced = false;
    motor0.decTabMalloced = false;

    motor1.id = 1;
    motor1.htim = &MOTOR1_TIM;
    motor1.channel = MOTOR1_CHANNEL;
    motor1.dir_port = MOTOR1_DIR_GPIO_Port;
    motor1.dir_pin = MOTOR1_DIR_Pin;
    motor1.en_port = MOTOR1_ENA_GPIO_Port;
    motor1.en_pin = MOTOR1_ENA_Pin;
    motor1.state = IDLE;
    motor1.mode = FREE;
    motor1.dir = FORWARD;
    motor1.accTab = NULL;
    motor1.decTab = NULL;
    motor1.accTabMalloced = false;
    motor1.decTabMalloced = false;

    motor2.id = 2;
    motor2.htim = &MOTOR2_TIM;
    motor2.channel = MOTOR2_CHANNEL;
    motor2.dir_port = MOTOR2_DIR_GPIO_Port;
    motor2.dir_pin = MOTOR2_DIR_Pin;
    motor2.en_port = MOTOR2_ENA_GPIO_Port;
    motor2.en_pin = MOTOR2_ENA_Pin;
    motor2.state = IDLE;
    motor2.mode = FREE;
    motor2.dir = FORWARD;
    motor2.accTab = NULL;
    motor2.decTab = NULL;
    motor2.accTabMalloced = false;
    motor2.decTabMalloced = false;

    motor3.id = 3;
    motor3.htim = &MOTOR3_TIM;
    motor3.channel = MOTOR3_CHANNEL;
    motor3.dir_port = MOTOR3_DIR_GPIO_Port;
    motor3.dir_pin = MOTOR3_DIR_Pin;
    motor3.en_port = MOTOR3_ENA_GPIO_Port;
    motor3.en_pin = MOTOR3_ENA_Pin;
    motor3.state = IDLE;
    motor3.mode = FREE;
    motor3.dir = FORWARD;
    motor3.accTab = NULL;
    motor3.decTab = NULL;
    motor3.accTabMalloced = false;
    motor3.decTabMalloced = false;
}

// ==== 曲线加减速运动接口 ====
// step为正表示正转，为负表示反转
void motorCurveMove(MotorControltype *motor, int32_t v1, float accTime, float decTime, int32_t step)
{
    // 1. 设置运动参数
    motor->v0 = 0;
    motor->v1 = v1;
    motor->accTime = accTime;
    motor->decTime = decTime;
    motor->step = abs(step); // 步数取绝对值
    motor->pos = 0;
    motor->state = ACCEL;
    motor->mode = POSITION;

    // 2. 设置方向
    if (step >= 0) {
        motor->dir = FORWARD;
    } else {
        motor->dir = BACKWARD;
    }
    // 控制方向引脚
    HAL_GPIO_WritePin(motor->dir_port, motor->dir_pin, (motor->dir) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    // 3. 生成加速/减速速度表（以S型曲线为例）
    int32_t acc_steps = motor->step / 3;
    int32_t uniform_steps = motor->step / 3;
    int32_t dec_steps = motor->step - acc_steps - uniform_steps;

    if (motor->accTabMalloced && motor->accTab) { free(motor->accTab); motor->accTab = NULL; }
    if (motor->decTabMalloced && motor->decTab) { free(motor->decTab); motor->decTab = NULL; }
    motor->accTab = (float*)malloc(sizeof(float) * acc_steps);
    motor->decTab = (float*)malloc(sizeof(float) * dec_steps);
    motor->accTabMalloced = motor->decTabMalloced = 1;

    float k = 10.0f / acc_steps;
    float x0 = acc_steps / 2.0f;
    for (int i = 0; i < acc_steps; i++) {
        float sigma = 1.0f / (1.0f + expf(-k * (i - x0)));
        motor->accTab[i] = motor->v1 * sigma;
    }
    for (int i = 0; i < dec_steps; i++) {
        float sigma = 1.0f / (1.0f + expf(-k * ((dec_steps - 1 - i) - x0)));
        motor->decTab[i] = motor->v1 * sigma;
    }

    // 4. 启动定时器OC中断
    __HAL_TIM_SET_COUNTER(motor->htim, 0);
    __HAL_TIM_SET_COMPARE(motor->htim, motor->channel, (uint32_t)motor->accTab[0]);
    HAL_TIM_OC_Start_IT(motor->htim, motor->channel);

    // 记录各阶段步数
    motor->accTime = acc_steps;
    motor->decTime = dec_steps;
}

// ==== 状态机（在定时器OC中断里调用） ====
void motorStateUpgrade(MotorControltype *motor)
{
    static int acc_idx = 0, uniform_idx = 0, dec_idx = 0;
    static int acc_steps = 0, uniform_steps = 0, dec_steps = 0;
    static int total_steps = 0;

    // 初始化阶段步数
    if (motor->pos == 0) {
        acc_steps = motor->accTime;
        dec_steps = motor->decTime;
        uniform_steps = motor->step - acc_steps - dec_steps;
        acc_idx = uniform_idx = dec_idx = 0;
        total_steps = 0;
    }

    switch (motor->state)
    {
    case ACCEL:
        if (acc_idx < acc_steps) {
            __HAL_TIM_SET_COMPARE(motor->htim, motor->channel, __HAL_TIM_GET_COUNTER(motor->htim) + (uint32_t)motor->accTab[acc_idx]);
            acc_idx++;
            motor->pos++;
            total_steps++;
        }
        if (acc_idx >= acc_steps) {
            motor->state = UNIFORM;
        }
        break;
    case UNIFORM:
        if (uniform_idx < uniform_steps) {
            __HAL_TIM_SET_COMPARE(motor->htim, motor->channel, __HAL_TIM_GET_COUNTER(motor->htim) + (uint32_t)motor->v1);
            uniform_idx++;
            motor->pos++;
            total_steps++;
        }
        if (uniform_idx >= uniform_steps) {
            motor->state = DECEL;
        }
        break;
    case DECEL:
        if (dec_idx < dec_steps) {
            __HAL_TIM_SET_COMPARE(motor->htim, motor->channel, __HAL_TIM_GET_COUNTER(motor->htim) + (uint32_t)motor->decTab[dec_idx]);
            dec_idx++;
            motor->pos++;
            total_steps++;
        }
        if (dec_idx >= dec_steps || total_steps >= motor->step) {
            motor->state = STOP;
        }
        break;
    case STOP:
        HAL_TIM_OC_Stop_IT(motor->htim, motor->channel);
        motor->state = IDLE;
        if (motor->accTabMalloced && motor->accTab) { free(motor->accTab); motor->accTab = NULL; motor->accTabMalloced = 0; }
        if (motor->decTabMalloced && motor->decTab) { free(motor->decTab); motor->decTab = NULL; motor->decTabMalloced = 0; }
        break;
    default:
        break;
    }
}

// ==== 定时器OC中断回调 ====
// 在 stm32f4xx_it.c 或 HAL库回调中调用
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &MOTOR0_TIM) {
        // MOTOR0_STEP_TOGGLE();
        motorStateUpgrade(&motor0);
    }
    if (htim == &MOTOR1_TIM) {
        // MOTOR1_STEP_TOGGLE();
        motorStateUpgrade(&motor1);
    }
    if (htim == &MOTOR2_TIM) {
        // MOTOR2_STEP_TOGGLE();
        motorStateUpgrade(&motor2);
    }
    if (htim == &MOTOR3_TIM) {
        // MOTOR3_STEP_TOGGLE();
        motorStateUpgrade(&motor3);
    }
}
