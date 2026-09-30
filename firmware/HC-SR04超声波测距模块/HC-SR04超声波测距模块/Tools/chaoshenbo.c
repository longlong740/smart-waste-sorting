#include "stm32f10x.h"                  // Device header
uint8_t flag=0;				//用于记录中断信号是上升沿还是下降沿
uint32_t number=0;			//记录定时器中断的次数
uint32_t times=0;			//记录回响信号的持续时间
void Hcsr05_1_Init(void){

//	//初始化GPIO口,Trig使用推挽输出,Echo使用浮空输入
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);        //使能GPIOA的外设时钟
//    GPIO_InitTypeDef itd;
//    itd.GPIO_Mode=GPIO_Mode_Out_PP;                             //选择推挽输出模式
//    itd.GPIO_Pin=GPIO_Pin_12;                                    //选择GPIO_Pin_12
//    itd.GPIO_Speed=GPIO_Speed_50MHz;                            //默认选择50MHz
//    GPIO_Init(GPIOB,&itd);
//    
//    itd.GPIO_Mode=GPIO_Mode_IN_FLOATING;                        //选择浮空输入模式
//    itd.GPIO_Pin=GPIO_Pin_13;                                    //选择GPIO_Pin_13
//    GPIO_Init(GPIOB,&itd);
//    
//    //AFIO映射中断引脚
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);         //使能AFIO的外设时针
//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);  //选择外部中断源和中断通道
//    
//    //EXTI中断配置
//    EXTI_InitTypeDef itd1;
//    itd1.EXTI_Line=EXTI_Line13;                                  //echo使用的端口13,因此选择13号中断线
//    itd1.EXTI_LineCmd=ENABLE;
//    itd1.EXTI_Mode=EXTI_Mode_Interrupt;
//    itd1.EXTI_Trigger=EXTI_Trigger_Rising_Falling;              //上升沿和下降沿都触发中断
//    EXTI_Init(&itd1);
//    
//    //NVIC分配外部中断的中断优先级
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             //指定中断分组
//    NVIC_InitTypeDef itd2;
//    itd2.NVIC_IRQChannel=EXTI15_10_IRQn;                          //使用的端口13,因此选择这个参数
//    itd2.NVIC_IRQChannelCmd=ENABLE;
//    itd2.NVIC_IRQChannelPreemptionPriority=2;                   //抢占优先级
//    itd2.NVIC_IRQChannelSubPriority=2;                          //响应优先级 
//    NVIC_Init(&itd2);
//    
//    //配置定时器
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
//    TIM_TimeBaseInitTypeDef itd3;
//    itd3.TIM_ClockDivision=TIM_CKD_DIV1;                        //使用时钟分频1
//    itd3.TIM_CounterMode=TIM_CounterMode_Up;                    //向上计数
//    //72MHz/72/100=1000,每秒定时器计数1000个,因此每个计数为100us
//    itd3.TIM_Period=72-1;                                       //预分频系数
//    itd3.TIM_Prescaler=100-1;                                   //自动重装器
//    itd3.TIM_RepetitionCounter=0;                               //该参数仅给高级定时器使用
//    TIM_TimeBaseInit(TIM4,&itd3);
//    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);                    //使能中断输出信号
//    TIM_InternalClockConfig(TIM4);                              //选择内部时钟    
//    
//    //NVIC分配定时器的中断优先级
//    NVIC_InitTypeDef itd4;
//    itd4.NVIC_IRQChannel=TIM4_IRQn;                             //指定Tim2的中断通道
//    itd4.NVIC_IRQChannelCmd=ENABLE;
//    itd4.NVIC_IRQChannelPreemptionPriority=1;                   //抢占优先级
//    itd4.NVIC_IRQChannelSubPriority=1;                          //响应优先级
//    NVIC_Init(&itd4);
//    // 初始化GPIO口，Trig使用推挽输出，Echo使用浮空输入
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能GPIOB的外设时钟
    GPIO_InitTypeDef GPIO_InitStructure;

    // 配置Trig引脚为推挽输出模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                     // 推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                           // 选择GPIO_Pin_12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                    // 设置速度为50MHz
    //GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 配置Echo引脚为浮空输入模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                // 浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;                           // 选择GPIO_Pin_13
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // AFIO映射中断引脚
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // 使能AFIO的外设时钟
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13); // 将GPIOB的Pin13映射到外部中断线

    // EXTI中断配置
    EXTI_InitTypeDef itd1;
    itd1.EXTI_Line = EXTI_Line13;                         // 选择13号中断线
    itd1.EXTI_LineCmd = ENABLE;                          // 使能中断线
    itd1.EXTI_Mode = EXTI_Mode_Interrupt;                // 配置为中断模式
    itd1.EXTI_Trigger = EXTI_Trigger_Rising_Falling;     // 上升沿和下降沿都触发中断
    EXTI_Init(&itd1);

    // NVIC分配外部中断的中断优先级
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);      // 设置中断优先级分组
    NVIC_InitTypeDef itd2;
    itd2.NVIC_IRQChannel = EXTI15_10_IRQn;               // 使用EXTI15_10中断通道
    itd2.NVIC_IRQChannelCmd = ENABLE;                    // 使能中断通道
    itd2.NVIC_IRQChannelPreemptionPriority = 2;          // 抢占优先级为2
    itd2.NVIC_IRQChannelSubPriority = 2;                 // 响应优先级为2
    NVIC_Init(&itd2);

    // 配置定时器TIM4
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // 使能TIM4外设时钟
    TIM_TimeBaseInitTypeDef itd3;
    itd3.TIM_ClockDivision = TIM_CKD_DIV1;               // 时钟分频1
    itd3.TIM_CounterMode = TIM_CounterMode_Up;           // 向上计数模式
    itd3.TIM_Period = 72 - 1;                            // 预分频系数
    itd3.TIM_Prescaler = 100 - 1;                        // 自动重装器
    itd3.TIM_RepetitionCounter = 0;                      // 重复计数器（仅高级定时器使用）
    TIM_TimeBaseInit(TIM4, &itd3);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);           // 使能定时器更新中断
    TIM_InternalClockConfig(TIM4);                       // 选择内部时钟

    // NVIC分配定时器的中断优先级
    NVIC_InitTypeDef itd4;
    itd4.NVIC_IRQChannel = TIM4_IRQn;                    // 使用TIM4中断通道
    itd4.NVIC_IRQChannelCmd = ENABLE;                    // 使能中断通道
    itd4.NVIC_IRQChannelPreemptionPriority = 1;          // 抢占优先级为1
    itd4.NVIC_IRQChannelSubPriority = 1;                 // 响应优先级为1
    NVIC_Init(&itd4);
}
float Hcsr05GetLength(uint8_t sensor){
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 1){
            //上升沿即回响电平开始,打开计数器
            number=0;flag=1;
            TIM_SetCounter(TIM4,0);
            TIM_Cmd(TIM4,ENABLE);
            
        }else if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == 0){
            //下降沿即回响电平结束,统计高电平持续时长
            TIM_Cmd(TIM4,DISABLE);
            flag=0;
            times=number*100+TIM_GetCounter(TIM4);  //得到回响的高电平持续的us
        }
}
//定时器中断函数
void TIM4_IRQHandler(void){
    if(SET==TIM_GetITStatus(TIM4,TIM_FLAG_Update)){
        number++;                                   //每次中断将次数++
        TIM_ClearITPendingBit(TIM4,TIM_FLAG_Update);
    }
}
