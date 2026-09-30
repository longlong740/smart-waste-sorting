#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "tuigan.h"
#include "key.h"

uint8_t KeyNum;

int main(void){
	
	tuigan_Init();
	
	KeyNum=2;
	if(KeyNum==1){
		TG_ya_OPEA();//开压
		Delay_s(10);// 10mm/s的速度，
		TG_ya_NO();//达最大推杆长度
	}
	if(KeyNum==2){
		TG_suo_OPEA();//开缩
		Delay_s(10);
		TG_suo_NO();//回初始状态
	}

}
