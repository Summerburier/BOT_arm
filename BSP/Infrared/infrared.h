#ifndef __INFRARED_H__
#define __INFRARED_H__
#include "sys.h"
#include "main.h"
#include "delay.h"  // delay_ms 函数所在头文件
#include "stm32f4xx_hal.h"

#define INFRARED  HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_15) //INFRARED按键PG15
#define INFRARED_PRES  1

int infraredSensor(void);



#endif // !__INFRARED_H__
