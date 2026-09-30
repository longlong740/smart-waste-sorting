#include "stm32f10x.h"                  // Device header

void LVDAI_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 1. 使能GPIOA和GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    // 2. 配置PA15为推挽输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; // 选择PA15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 输出速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIOA

    // 3. 配置PB4为推挽输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; // 选择PB4
    GPIO_Init(GPIOB, &GPIO_InitStructure); // 初始化GPIOB
}

// 同时控制继电器1和继电器2的函数
void RelayControl(uint8_t state)
{
    if (state == 1) 
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_15); // 设置PA15为低电平
        GPIO_ResetBits(GPIOB, GPIO_Pin_4);  // 设置PB4为低电平

    }
    else 
    {
       GPIO_SetBits(GPIOA, GPIO_Pin_15); // 设置PA15为高电平
        GPIO_SetBits(GPIOB, GPIO_Pin_4);  // 设置PB4为高电平
    }
}
