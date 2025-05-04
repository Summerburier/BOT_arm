#ifndef _SERVO_MOTOR_H
#define _SERVO_MOTOR_H
#include "main.h"
#include "usart.h"


#define catch "2500"
#define release "1500"
#define init "0500"
#define servoMotor1 "000"
#define servoMotor2 "001"

void servoMotor_init(void);
void catchServoMotor(void);
void releaseServoMotor(void);
void initServoMotor(void);
void rotateServoMotor(void);


#endif
