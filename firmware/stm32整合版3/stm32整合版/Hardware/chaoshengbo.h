#ifndef __CHAOSHENGBO_H
#define __CHAOSHENGBO_H

#include "stm32f10x.h"

// 超声波模块的引脚定义
#define HCSR05_1_TRIG_PIN  GPIO_Pin_12
#define HCSR05_1_ECHO_PIN  GPIO_Pin_13
#define HCSR05_1_PORT      GPIOB

#define HCSR05_2_TRIG_PIN  GPIO_Pin_14
#define HCSR05_2_ECHO_PIN  GPIO_Pin_15
#define HCSR05_2_PORT      GPIOB

#define HCSR05_3_TRIG_PIN  GPIO_Pin_11
#define HCSR05_3_ECHO_PIN  GPIO_Pin_12
#define HCSR05_3_PORT      GPIOA

#define HCSR05_4_TRIG_PIN  GPIO_Pin_5
#define HCSR05_4_ECHO_PIN  GPIO_Pin_6
#define HCSR05_4_PORT      GPIOB

// 初始化函数
void Hcsr05_1_Init(void);//有害
void Hcsr05_2_Init(void);//可回收
void Hcsr05_3_Init(void);//厨余
void Hcsr05_4_Init(void);//其他

// 测距函数
float Hcsr05GetLength(uint8_t sensor);

void Tim4_Init(void);
static void OpenTimerForHc(void);
static void CloseTimerForHc(void);
uint32_t GetEchoTimer(void);

#endif
