#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "chaoshenbo.h"
//uint8_t flag=0;				//用于记录中断信号是上升沿还是下降沿
//uint32_t number=0;			//记录定时器中断的次数
//uint32_t times=0;			//记录回响信号的持续时间
uint32_t time=0;
uint32_t distance;
int main(void){
    OLED_Init();
	Hcsr05_1_Init();
    //OLED_ShowString(1,1,"Hello World!!!");
    while(1){
        distance=0;
		time = Hcsr05GetLength(1);
        for(int i=0;i<10;++i){              //每次取10次测距数据,取平均值减少误差
            GPIO_SetBits(GPIOB,GPIO_Pin_12);
            Delay_us(15);                   //根据说明书,需要提供至少10us的高电平
            GPIO_ResetBits(GPIOB,GPIO_Pin_12);
            Delay_ms(65);                   //根据说明书,每个周期至少需要等待60ms
            distance+=(time/58);          //根据说明书提供的公式,获取单位为mm的距离
        }
        distance/=10;
        OLED_ShowNum(2,1,distance,4);
    }
}

////定时器中断函数
//void TIM4_IRQHandler(void){
//    if(SET==TIM_GetITStatus(TIM4,TIM_FLAG_Update)){
//        number++;                                   //每次中断将次数++
//        TIM_ClearITPendingBit(TIM4,TIM_FLAG_Update);
//    }
//}

//外部中断函数
//void EXTI15_10_IRQHandler(void){
//    if(SET==EXTI_GetITStatus(EXTI_Line13)){
//        if(flag==0){
//            //上升沿即回响电平开始,打开计数器
//            number=0;flag=1;
//            TIM_SetCounter(TIM4,0);
//            TIM_Cmd(TIM4,ENABLE);
//            
//        }else{
//            //下降沿即回响电平结束,统计高电平持续时长
//            TIM_Cmd(TIM4,DISABLE);
//            flag=0;
//            times=number*100+TIM_GetCounter(TIM4);  //得到回响的高电平持续的us
//        }
//        EXTI_ClearITPendingBit(EXTI_Line13);
//    }
//}

                                            

