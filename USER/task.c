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
    //motorInit();
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
    stepMotorRun(0, 0, 3000, 200, 100000,false,true);
    stepMotorRun(1, 0, 3000, 200, 100000,false,true);
    stepMotorRun(2, 0, 3000, 200, 100000,false,true);
}   

int armToConveyer(int *result)  // 修正参数类型
{
    stepMotorRun(0, 0, 3000, 200, 100000,false,true);
    belt_on();
    delay_ms(1000);
    belt_off();
    OpenMV_SendIdentifyCmd();
    if(OpenMV_GetResult((uint8_t*)result) == 0){ // 确保类型匹配
        stepMotorRun(0, 0, 3000, 200, 100000,false,true);
        catchServoMotor();
        delay_ms(1000);
        stepMotorRun(0, 1, 3000, 200, 100000,false,true);
        return 1;
    }
    else{
        stepMotorRun(0, 0, 3000, 200, 100000,false,true);
        return 0;
    }
}

void armToTable(int now, int place) // 改为void类型
{
    stepMotorRun(0, 0, 3000, 200, 100000,false,true);
    rotateServoMotor();
    stepMotorRun(0, 0, 3000, 200, 100000,false,true);
    releaseServoMotor();
    initServoMotor();
    stepMotorRun(0, 0, 3000, 200, 100000,false,true);
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
            stepMotorRun(0, 0, 3000, 200, 100000,false,true);
        }
    }
}