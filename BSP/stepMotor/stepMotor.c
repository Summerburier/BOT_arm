#include "stepMotor.h"




/**
  * @brief    使能信号控制
  * @param    addr  ：电机地址
  * @param    state ：使能状态     ，true为使能电机，false为关闭电机
  * @param    snF   ：多机同步标志 ，false为不启用，true为启用
  * @retval   地址 + 功能码 + 命令状态 + 校验字节
  */
void stepMotorEn(uint8_t addr, bool state, bool snF)
{
  uint8_t cmd[16] = {0};
  
  // 装载命令
  cmd[0] =  addr;                       // 地址
  cmd[1] =  0xF3;                       // 功能码
  cmd[2] =  0xAB;                       // 辅助码
  cmd[3] =  (uint8_t)state;             // 使能状态
  cmd[4] =  snF;                        // 多机同步运动标志
  cmd[5] =  0x6B;                       // 校验字节
  
  // 发送命令
  HAL_UART_Transmit_DMA(&huart1, (uint8_t *)cmd, 6);
}



/**
  * @brief    位置模式
  * @param    addr：电机地址
  * @param    dir ：方向        ，0为CW，其余值为CCW
  * @param    vel ：速度(RPM)   ，范围0 - 5000RPM
  * @param    acc ：加速度      ，范围0 - 255，注意：0是直接启动
  * @param    clk ：脉冲数      ，范围0- (2^32 - 1)个
  * @param    raF ：相位/绝对标志，false为相对运动，true为绝对值运动
  * @param    snF ：多机同步标志 ，false为不启用，true为启用
  * @retval   地址 + 功能码 + 命令状态 + 校验字节
  */
void stepMotorRun(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, uint32_t clk, bool raF, bool snF)
{
  uint8_t cmd[16] = {0};

  // 装载命令
  cmd[0]  =  addr;                      // 地址
  cmd[1]  =  0xFD;                      // 功能码
  cmd[2]  =  dir;                       // 方向
  cmd[3]  =  (uint8_t)(vel >> 8);       // 速度(RPM)高8位字节
  cmd[4]  =  (uint8_t)(vel >> 0);       // 速度(RPM)低8位字节 
  cmd[5]  =  acc;                       // 加速度，注意：0是直接启动
  cmd[6]  =  (uint8_t)(clk >> 24);      // 脉冲数(bit24 - bit31)
  cmd[7]  =  (uint8_t)(clk >> 16);      // 脉冲数(bit16 - bit23)
  cmd[8]  =  (uint8_t)(clk >> 8);       // 脉冲数(bit8  - bit15)
  cmd[9]  =  (uint8_t)(clk >> 0);       // 脉冲数(bit0  - bit7 )
  cmd[10] =  raF;                       // 相位/绝对标志，false为相对运动，true为绝对值运动
  cmd[11] =  snF;                       // 多机同步运动标志，false为不启用，true为启用
  cmd[12] =  0x6B;                      // 校验字节
  
  // 发送命令
  HAL_UART_Transmit_DMA(&huart1, (uint8_t *)cmd, 13);
}

/**
  * @brief    立即停止（所有控制模式都通用）
  * @param    addr  ：电机地址
  * @param    snF   ：多机同步标志，false为不启用，true为启用
  * @retval   地址 + 功能码 + 命令状态 + 校验字节
  */
void stepMotorStop(uint8_t addr, bool snF)
{
  uint8_t cmd[16] = {0};
  
  // 装载命令
  cmd[0] =  addr;                       // 地址
  cmd[1] =  0xFE;                       // 功能码
  cmd[2] =  0x98;                       // 辅助码
  cmd[3] =  snF;                        // 多机同步运动标志
  cmd[4] =  0x6B;                       // 校验字节
  
  // 发送命令
  HAL_UART_Transmit_DMA(&huart1, (uint8_t *)cmd, 5);
}

/**
  * @brief    多机同步运动
  * @param    addr  ：电机地址
  * @retval   地址 + 功能码 + 命令状态 + 校验字节
  */
void stepMotorSync(uint8_t addr)
{
  uint8_t cmd[16] = {0};
  
  // 装载命令
  cmd[0] =  addr;                       // 地址
  cmd[1] =  0xFF;                       // 功能码
  cmd[2] =  0x66;                       // 辅助码
  cmd[3] =  0x6B;                       // 校验字节
  
  // 发送命令
  HAL_UART_Transmit_DMA(&huart1, (uint8_t *)cmd, 4);
}
