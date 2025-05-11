/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MOTOR4_DIR_Pin GPIO_PIN_2
#define MOTOR4_DIR_GPIO_Port GPIOE
#define MOTOR4_ENA_Pin GPIO_PIN_3
#define MOTOR4_ENA_GPIO_Port GPIOE
#define KEY0_Pin GPIO_PIN_4
#define KEY0_GPIO_Port GPIOE
#define MOTOR4_PUL_Pin GPIO_PIN_5
#define MOTOR4_PUL_GPIO_Port GPIOE
#define COL1_Pin GPIO_PIN_0
#define COL1_GPIO_Port GPIOC
#define COL2_Pin GPIO_PIN_1
#define COL2_GPIO_Port GPIOC
#define KEY_UP_Pin GPIO_PIN_0
#define KEY_UP_GPIO_Port GPIOA
#define MOTOR3_PUL_Pin GPIO_PIN_1
#define MOTOR3_PUL_GPIO_Port GPIOA
#define MOTOR1_DIR_Pin GPIO_PIN_4
#define MOTOR1_DIR_GPIO_Port GPIOA
#define MOTOR1_ENA_Pin GPIO_PIN_5
#define MOTOR1_ENA_GPIO_Port GPIOA
#define MOTOR1_PUL_Pin GPIO_PIN_7
#define MOTOR1_PUL_GPIO_Port GPIOA
#define MOTOR3_DIR_Pin GPIO_PIN_1
#define MOTOR3_DIR_GPIO_Port GPIOB
#define MOTOR3_ENA_Pin GPIO_PIN_2
#define MOTOR3_ENA_GPIO_Port GPIOB
#define MOTOR0_DIR_Pin GPIO_PIN_10
#define MOTOR0_DIR_GPIO_Port GPIOE
#define MOTOR0_PUL_Pin GPIO_PIN_11
#define MOTOR0_PUL_GPIO_Port GPIOE
#define MOTOR0_ENA_Pin GPIO_PIN_12
#define MOTOR0_ENA_GPIO_Port GPIOE
#define MOTOR2_DIR_Pin GPIO_PIN_11
#define MOTOR2_DIR_GPIO_Port GPIOD
#define MOTOR2_ENA_Pin GPIO_PIN_12
#define MOTOR2_ENA_GPIO_Port GPIOD
#define MOTOR2_PUL_Pin GPIO_PIN_13
#define MOTOR2_PUL_GPIO_Port GPIOD
#define BELT_Pin GPIO_PIN_7
#define BELT_GPIO_Port GPIOC
#define KEY2_Pin GPIO_PIN_12
#define KEY2_GPIO_Port GPIOC
#define ROW1_Pin GPIO_PIN_11
#define ROW1_GPIO_Port GPIOG
#define ROW2_Pin GPIO_PIN_12
#define ROW2_GPIO_Port GPIOG
#define ROW3_Pin GPIO_PIN_13
#define ROW3_GPIO_Port GPIOG
#define ROW4_Pin GPIO_PIN_14
#define ROW4_GPIO_Port GPIOG
#define KEY1_Pin GPIO_PIN_15
#define KEY1_GPIO_Port GPIOG
#define COL3_Pin GPIO_PIN_0
#define COL3_GPIO_Port GPIOE
#define COL4_Pin GPIO_PIN_1
#define COL4_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
