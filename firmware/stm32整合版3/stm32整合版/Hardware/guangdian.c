#include "stm32f10x.h"                  // Device header


void GD_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // 配置PB7、PB8和PB9为浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
}


// 检测障碍物的函数
uint8_t GD_Check_Obstacle(void)
{
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == Bit_RESET || 
       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == Bit_RESET||GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == Bit_RESET)
    {
        // PB8或PB9、PB7检测到障碍物
        return 1;
    }
    else
    {
        // 没有检测到障碍物
        return 0;
    }
} 