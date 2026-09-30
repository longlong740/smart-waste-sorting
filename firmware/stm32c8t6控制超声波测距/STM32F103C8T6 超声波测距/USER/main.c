
/**************************************
 * 文件名  ：main.c
 * 描述    ：通过串口调试软件，向板子发送数据，板子接收到数据后，立即回传给电脑。         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 库版本  ：ST3.0.0  																										  

*********************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "time.h"
#include "led.h"

int main(void)
{  
	float length;
	SystemInit();	//配置系统时钟为 72M 
	USART1_Config(); //USART1 配置 	
//	LED_GPIO_Config();

	Hcsr05Init();
  while (1)
  {	 

		length = Hcsr05GetLength();
		printf("1");
printf("length: %.3f cm\r\n", length); 
		if(length<10)
		{GPIO_ResetBits(GPIOC, GPIO_Pin_13 );}
		else 
		{GPIO_SetBits(GPIOC, GPIO_Pin_13 );}

		delay_ms(100); 
  }
}




