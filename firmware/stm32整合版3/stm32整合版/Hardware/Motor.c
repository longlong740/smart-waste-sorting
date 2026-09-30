#include "stm32f10x.h"                  // Device header
#include "PWM.h"

// 定义电机编号
#define MOTOR_A 0
#define MOTOR_B 1
#define MOTOR_C 2
#define MOTOR_D 3

/**
  * 函    数：直流电机初始化
  * 参    数：无
  * 返 回 值：无
  */
void Motor_Init(void)
{
    /* 开启GPIOA和GPIOB时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    /* 初始化所有方向控制引脚为推挽输出 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // 电机A方向引脚：PB8(AIN2), PB9(AIN1)
    // 电机B方向引脚：PA8(BIN2), PB7(BIN1)
    // 电机C方向引脚：PB0(CIN2), PB1(CIN1)
    // 电机D方向引脚：PB10(DIN2), PB11(DIN1)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    PWM_Init_Moter(); // 初始化PWM通道
}

/**
  * 函    数：直流电机设置速度
  * 参    数：motor 电机编号（MOTOR_A~MOTOR_D）
  *         Speed 速度，范围：-100~100
  * 返 回 值：无
  */
void Motor_SetSpeed(uint8_t motor, int8_t Speed)
{
    switch(motor) {
        case MOTOR_A: // PWMA(PA0), AIN1(PB9), AIN2(PB8)
            GPIO_WriteBit(GPIOB, GPIO_Pin_9, Speed >= 0 ? Bit_SET : Bit_RESET);
            GPIO_WriteBit(GPIOB, GPIO_Pin_8, Speed >= 0 ? Bit_RESET : Bit_SET);
            PWM_SetCompare1_Moter(Speed >= 0 ? Speed : -Speed);
            break;
        case MOTOR_B: // PWMB(PA1), BIN1(PB7), BIN2(PA8)
            GPIO_WriteBit(GPIOB, GPIO_Pin_7, Speed >= 0 ? Bit_SET : Bit_RESET);
            GPIO_WriteBit(GPIOA, GPIO_Pin_8, Speed >= 0 ? Bit_RESET : Bit_SET);
            PWM_SetCompare2_Moter(Speed >= 0 ? Speed : -Speed);
            break;
        case MOTOR_C: // PWMC(PA2), CIN1(PB1), CIN2(PB0)
            GPIO_WriteBit(GPIOB, GPIO_Pin_1, Speed >= 0 ? Bit_SET : Bit_RESET);
            GPIO_WriteBit(GPIOB, GPIO_Pin_0, Speed >= 0 ? Bit_RESET : Bit_SET);
            PWM_SetCompare3_Moter(Speed >= 0 ? Speed : -Speed);
            break;
        case MOTOR_D: // PWMD(PA3), DIN1(PB11), DIN2(PB10)
            GPIO_WriteBit(GPIOB, GPIO_Pin_11, Speed >= 0 ? Bit_SET : Bit_RESET);
            GPIO_WriteBit(GPIOB, GPIO_Pin_10, Speed >= 0 ? Bit_RESET : Bit_SET);
            PWM_SetCompare4_Moter(Speed >= 0 ? Speed : -Speed);
            break;
    }
}

/**
  * 函    数：测试传送带电机代码
  * 参    数：KeyNum 条件；motor 电机编号；Speed 初始速度
  * 返 回 值：无
  */
void Motor_Test(int8_t KeyNum, uint8_t motor, int8_t Speed)
{
    if (KeyNum == 1) {
        Speed += 20;
        if (Speed > 100) Speed = -100;
    }
    Motor_SetSpeed(motor, Speed);
}
