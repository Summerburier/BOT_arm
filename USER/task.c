#include "main.h"
#include "identify.h"
#include "belt.h"
#include "stepMotor.h"
#include "servoMotor.h"
#include "usart.h"
#include "delay.h"

int place[4]= {0,0,0,0}; // 记录每个物品的位置
void arm_init(void)
{
    // 初始化步进电机
    motorInit();
    // 初始化舵机
    servoMotor_init();
    // 初始化LED
    LED_Init();
    // 初始化按键
    KEY_Init();
    // 初始化延时函数
    delay_init(168);
    initServoMotor();
    releaseServoMotor();
    motorCurveMove(&motor0, 1000, 0.1, 0.1, 1000);
    motorCurveMove(&motor1, 1000, 0.1, 0.1, 1000);
    motorCurveMove(&motor2, 1000, 0.1, 0.1, 1000);
}   

int armToConveyer(int *result)  // 修正参数类型
{
    motorCurveMove(&motor0, 1000, 0.1, 0.1, 1000);
    belt_on();
    delay_ms(1000);
    belt_off();
    OpenMV_SendIdentifyCmd();
    if(OpenMV_GetResult((uint8_t*)result) == 0){ // 确保类型匹配
        motorCurveMove(&motor1, 1000, 0.1, 0.1, 1000);
        catchServoMotor();
        delay_ms(1000);
        motorCurveMove(&motor1, 1000, 0.1, 0.1, -1000);
        return 1;
    }
    else{
        motorCurveMove(&motor0, 1000, 0.1, 0.1, -1000);
        return 0;
    }
}

void armToTable(int now, int place) // 改为void类型
{
    motorCurveMove(&motor2, 1000, 0.1, 0.1, 1000);
    rotateServoMotor();
    motorCurveMove(&motor3, 1000, 0.1, 0.1, now-place);
    motorCurveMove(&motor0, 1000, 0.1, 0.1, 1000);
    motorCurveMove(&motor1, 1000, 0.1, 0.1, 1000);
    releaseServoMotor();
    initServoMotor();
    motorCurveMove(&motor0, 1000, 0.1, 0.1, -1000);
    motorCurveMove(&motor1, 1000, 0.1, 0.1, -1000);
}

void arm_task(void)
{
    int result[2];
    int now = 0;
    arm_init();
    while(1){
        if(armToConveyer(result) == 1){
            armToTable(now,place[result[0]]);
            now = place[result[0]];
        }
        else{
            motorCurveMove(&motor0, 1000, 0.1, 0.1, -1000);
            motorCurveMove(&motor1, 1000, 0.1, 0.1, -1000);
        }
    }
}