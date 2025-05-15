#include "key.h"
#include "delay.h"  // delay_ms 函数所在头文件

//按键初始化函数
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();           //开启GPIOE时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();           //开启GPIOG时钟

    
    GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
	GPIO_Initure.Pin=GPIO_PIN_4; //PE2,3,4
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);

    GPIO_Initure.Pin=GPIO_PIN_12; //PC12
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);

    GPIO_Initure.Pin=GPIO_PIN_15; //PG15
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
    
    
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，WKUP按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP!!
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;     //按键松开标志
    if(mode==1)key_up=1;    //支持连按
    if(key_up&&(KEY0==1||WK_UP==1||KEY1==0||KEY2==0))
    {
        delay_ms(10);
        key_up=0;
        if(KEY0==1)       return KEY0_PRES;
        else if(WK_UP==1) return WKUP_PRES;   
        else if(KEY1==0) return KEY1_PRES;
        else if(KEY2==0) return KEY2_PRES;       
    }else if(KEY0==0&&WK_UP==0)key_up=1;
    return 0;   //无按键按下
}

int mk_scan(void)
{
    int key = 0;
    // 先将所有列置高
    HAL_GPIO_WritePin(COL1_GPIO_Port, COL1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL2_GPIO_Port, COL2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL3_GPIO_Port, COL3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL4_GPIO_Port, COL4_Pin, GPIO_PIN_SET);

    // 扫描每一列
    for (int col = 0; col < 4; col++) {
        // 再次将所有列置高
        HAL_GPIO_WritePin(COL1_GPIO_Port, COL1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL2_GPIO_Port, COL2_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL3_GPIO_Port, COL3_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(COL4_GPIO_Port, COL4_Pin, GPIO_PIN_SET);
        // 将当前列置低激活按键
        switch(col)
        {
            case 0: HAL_GPIO_WritePin(COL1_GPIO_Port, COL1_Pin, GPIO_PIN_RESET); break;
            case 1: HAL_GPIO_WritePin(COL2_GPIO_Port, COL2_Pin, GPIO_PIN_RESET); break;
            case 2: HAL_GPIO_WritePin(COL3_GPIO_Port, COL3_Pin, GPIO_PIN_RESET); break;
            case 3: HAL_GPIO_WritePin(COL4_GPIO_Port, COL4_Pin, GPIO_PIN_RESET); break;
        }
        delay_ms(10);  // 消抖延时

        // 读取每一行
        for (int row = 0; row < 4; row++) {
            uint8_t pressed = 0;
            switch(row)
            {
                case 0: pressed = (HAL_GPIO_ReadPin(ROW1_GPIO_Port, ROW1_Pin) == GPIO_PIN_RESET); break;
                case 1: pressed = (HAL_GPIO_ReadPin(ROW2_GPIO_Port, ROW2_Pin) == GPIO_PIN_RESET); break;
                case 2: pressed = (HAL_GPIO_ReadPin(ROW3_GPIO_Port, ROW3_Pin) == GPIO_PIN_RESET); break;
                case 3: pressed = (HAL_GPIO_ReadPin(ROW4_GPIO_Port, ROW4_Pin) == GPIO_PIN_RESET); break;
            }
            if (pressed) {
                // 根据行、列计算按键编号,例如编号 = row * 4 + col + 1
                key = row * 4 + col + 1;
                // 等待按键释放，避免重复检测
                while(1)
                {
                    uint8_t stillPressed = 0;
                    switch(row)
                    {
                        case 0: stillPressed = (HAL_GPIO_ReadPin(ROW1_GPIO_Port, ROW1_Pin) == GPIO_PIN_RESET); break;
                        case 1: stillPressed = (HAL_GPIO_ReadPin(ROW2_GPIO_Port, ROW2_Pin) == GPIO_PIN_RESET); break;
                        case 2: stillPressed = (HAL_GPIO_ReadPin(ROW3_GPIO_Port, ROW3_Pin) == GPIO_PIN_RESET); break;
                        case 3: stillPressed = (HAL_GPIO_ReadPin(ROW4_GPIO_Port, ROW4_Pin) == GPIO_PIN_RESET); break;
                    }
                    if (!stillPressed)
                        break;
                    delay_ms(10);
                }
                return key;
            }
        }
    }
    return 0;  // 无按键按下则返回0
}
