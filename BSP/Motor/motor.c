#include "motor.h"
#include "main.h"
#include "stdio.h"

__IO uint8_t done_flag = HAL_OK;
static uint16_t n = 0;
static uint16_t set_n = 0;
static float param_a = 0;

// void HAL_Count_PWM_Generator_Init(void) {
//   /*初始化主定时器,这个函数已经被mx_timx_init调用 */
//   // HAL_TIM_Base_Init(&COUNT_PWM_TIMER);
// }

void HAL_Count_PWM_Generate(uint16_t count) {
  // __HAL_TIM_SET_PRESCALER(&SLAVE_TIMER, 125 - 1);
  // // 设置从定时器的计数值，当计数值到达时，产生中断
  // __HAL_TIM_SET_AUTORELOAD(&SLAVE_TIMER, count - 1);
  __HAL_TIM_SET_COUNTER(&COUNT_PWM_TIMER, 0);              // 清零计数器
  __HAL_TIM_CLEAR_FLAG(&COUNT_PWM_TIMER, TIM_FLAG_UPDATE); // 清除更新中断标志
  n = 0;
  set_n = count - 1;
  done_flag = HAL_BUSY;
  param_a = PARAM_A(set_n);
  __HAL_TIM_SET_PRESCALER(&COUNT_PWM_TIMER, MAX_PRESCALER - 1);

  HAL_TIM_Base_Start_IT(&COUNT_PWM_TIMER);
  // HAL_TIM_PWM_Start_IT(&COUNT_PWM_TIMER, PWM_CHANNEL);
  HAL_TIM_PWM_Start(&COUNT_PWM_TIMER, PWM_CHANNEL);
}

/**
 * @brief 生成变化、特定数量的PWM波形的回调函数
 *
 * @param htim
 */
void HAL_Count_PWM_Generator_Callback(TIM_HandleTypeDef *htim) {
  // if (htim == &SLAVE_TIMER) {
  //   // 使用__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_CC2) != RESET
  //   // 来判断是否是这个通道产生的中断
  //   但是这里我们只使用一个通道，所以不需要判断
  //   HAL_TIM_PWM_Stop(&MASTER_TIMER, PWM_CHANNEL); // 关闭主定时器
  //   __HAL_TIM_CLEAR_FLAG(&SLAVE_TIMER,
  //                        TIM_FLAG_UPDATE); // 清除中断标志,用于下一次计数
  // }

  uint32_t prescaler = (uint32_t)(param_a * (n - set_n / 2) * (n - set_n / 2) +
                                  MIN_PRESCALER - 1);
  __HAL_TIM_SET_PRESCALER(&COUNT_PWM_TIMER, prescaler);
  n++;
  // time += (prescaler) * (1999) / MCU_FREQUENCY.0f;
  if (n >= set_n) {
    HAL_TIM_Base_Stop_IT(&COUNT_PWM_TIMER);
    // HAL_TIM_Base_Stop(&COUNT_PWM_TIMER);
    HAL_TIM_PWM_Stop(&COUNT_PWM_TIMER, PWM_CHANNEL);
    // HAL_TIM_PWM_Stop_IT(&COUNT_PWM_TIMER, PWM_CHANNEL);
    // MX_TIM3_Init();
    done_flag = HAL_OK;
  }
  // printf("%d,%d,%f,%f\n", n, prescaler,
  //        360.0f * MCU_FREQUENCY / (800 * prescaler * 1999), time);
}

void Turn(uint8_t dir, uint16_t count) {
  if (dir) {
    HAL_GPIO_WritePin(MOTOR0_DIR_GPIO_Port, MOTOR0_DIR_Pin, GPIO_PIN_SET);
  } else {
    HAL_GPIO_WritePin(MOTOR0_DIR_GPIO_Port,MOTOR0_DIR_Pin, GPIO_PIN_RESET);
  }
  if (count > 0) {
    HAL_Count_PWM_Generate(count);
  }
}