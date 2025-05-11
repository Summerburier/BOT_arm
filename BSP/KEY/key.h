#ifndef _KEY_H
#define _KEY_H
#include "sys.h"
#include "main.h"
#include "delay.h"  // delay_ms 函数所在头文件
#include "stm32f4xx_hal.h"

#define KEY0        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)  //KEY0按键PE4
//#define KEY1        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3)  //KEY1按键PE3
//#define KEY2        HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2) 	//KEY2按键PE2
#define WK_UP       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)  //WKUP按键PA0
#define KEY1       HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_15)  //KEY2按键PE2
#define KEY2        HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12) 




#define KEY0_PRES 	1
#define KEY1_PRES	2
#define KEY2_PRES	3
#define WKUP_PRES   4

void KEY_Init(void);
uint8_t KEY_Scan(uint8_t mode);
int mk_scan(void);



#endif
