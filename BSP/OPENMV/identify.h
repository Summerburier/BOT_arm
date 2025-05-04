#ifndef __IDENTIFY_H__
#define __IDENTIFY_H__

#include "main.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"

int OpenMV_GetResult(uint8_t result[2]);
void OpenMV_SendIdentifyCmd(void);

#endif

