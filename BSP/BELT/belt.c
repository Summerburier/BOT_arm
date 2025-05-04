
#include "belt.h"

void belt_on(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); //PA1
}
void belt_off(void)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); //PA1
}