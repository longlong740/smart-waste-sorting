#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "guangdian.h"

int main(void)
{
	GD_Init();
	
	while (1)
	{

	Check_Obstacle();
	}
}
