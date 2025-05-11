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
    //servoMotor_init();
    // 初始化LED
    //LED_Init();
    // 初始化按键
    KEY_Init();
    // 初始化延时函数
    delay_init(168);
   // initServoMotor();
    releaseServoMotor();
    stepMotorEn(1, true, false);
    delay_ms(100);
    stepMotorEn(2, true, false);
    delay_ms(100);
    stepMotorEn(3, true, false);
    delay_ms(100);

}   

int armToConveyer(int *result)  // 修正参数类型
{
    stepMotorRun(3, 0, 3000, 200, 16150,false,false);
    delay_ms(1000);
    
    belt_on();
    delay_ms(1000);
    belt_off();
    OpenMV_SendIdentifyCmd();
    if(OpenMV_GetResult((uint8_t*)result) == -1){ // 确保类型匹配
        stepMotorRun(2, 0, 3000, 200, 11200,false,false);
        delay_ms(2000);
        catchServoMotor();
        delay_ms(800);
        stepMotorRun(2, 1, 3000, 200, 11200,false,false);
        delay_ms(2000);
        //rotateServoMotor();
        stepMotorRun(3, 1, 3000, 200, 16150,false,false);
        return 1;
    }
    else{
        delay_ms(3000);
        stepMotorRun(3, 1, 3000, 200, 16000,false,false);
        return 0;
    }
}

void armToTable() // 改为void类型
{
    stepMotorRun(1, 1, 5000, 200, 125000,false,false);
    
    delay_ms(4000);
    stepMotorRun(2, 0, 3000, 200, 42500,false,false);
    //rotateServoMotor();
    delay_ms(3000);
    //stepMotorRun(0, 0, 3000, 200, 100000,false,true);
    releaseServoMotor();
    stepMotorRun(2, 1, 3000, 200, 42500,false,false);
    delay_ms(3000);
   // initServoMotor();
   //initServoMotor();
    stepMotorRun(1, 0, 5000, 200, 125000,false,false);
}

void arm_task(void)
{
    int result[2];
    //int now = 0;
    arm_init();
    servoMotor_init();
    // servoMotor_init();
    // delay_ms(1000);
    // rotateServoMotor();
    // stepMotorReset(0x01);
    // stepMotorReset(0x02);
    // delay_ms(1000);
   //stepMotorRun(2, 1, 3000, 200, 11200,false,false);
   armToConveyer(result);
    delay_ms(2000);
    armToTable();
//     delay_ms(2000);
// stepMotorReset(0x01);
//     stepMotorRun(0x01, 1, 3000, 200,5000,false,false);
    //delay_ms(2000);
    //stepMotorRun(3, 0, 3000, 200, 16150,false,false);
    // while(1){
    // if(armToConveyer(result) == 1){
    // armToTable(now,place[result[0]]);
    // now = place[result[0]];
    // }
    // else{
    // stepMotorRun(0, 0, 3000, 200, 100000,false,true);
    // }
    // }
}