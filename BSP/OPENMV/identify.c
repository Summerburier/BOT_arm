#include "main.h"
#include <string.h>
#include <stdio.h>

// Declare the UART handle
extern UART_HandleTypeDef huart3;

// 定义OpenMV通信协议
#define OPENMV_CMD_START  0xA5
#define OPENMV_CMD_IDENTIFY 0x01

// 形状编号
#define SHAPE_NONE      0
#define SHAPE_TRIANGLE  1
#define SHAPE_CIRCLE    2
#define SHAPE_TRAPEZOID 3
#define SHAPE_RECTANGLE 4
#define SHAPE_HEXAGON   5

// 颜色编号
#define COLOR_NONE      0
#define COLOR_ORANGE    1
#define COLOR_BLUE      2

// 向OpenMV发送识别命令
void OpenMV_SendIdentifyCmd(void)
{
    uint8_t tx_buf[2];
    tx_buf[0] = OPENMV_CMD_START;
    tx_buf[1] = OPENMV_CMD_IDENTIFY;
    HAL_UART_Transmit(&huart3, tx_buf, 2, 100);
}

// 解析OpenMV回传数据，返回2字节数组 result[0]=形状 result[1]=颜色
// 假设回传格式：0xA5, 形状, 颜色
uint8_t rx_buf[5];

// 返回值：0成功，-1失败
int OpenMV_GetResult(uint8_t result[4])
{
    OpenMV_SendIdentifyCmd();
    if (HAL_UART_Receive(&huart3, rx_buf, 3, 500) == HAL_OK) {
        if (rx_buf[0] == OPENMV_CMD_START) {
            result[0] = rx_buf[1]; // 形状编号
            result[1] = rx_buf[2]; // 颜色编号
            result[2] = rx_buf[3]; // 形状中心X坐标
            result[3] = rx_buf[4]; // 形状中心Y坐标

            return 0;
        }
    }
    result[0] = SHAPE_NONE;
    result[1] = COLOR_NONE;
    return -1;
}