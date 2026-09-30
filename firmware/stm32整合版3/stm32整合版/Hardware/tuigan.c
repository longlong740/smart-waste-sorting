#include "stm32f10x.h"                  // Device header
void tuigan_Init(void)
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_5);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
}
void TG_ya_OPEA(void){
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
}
void TG_ya_NO(void){
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}
void TG_suo_OPEA(void){
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	
}
void TG_suo_NO(void){
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}


