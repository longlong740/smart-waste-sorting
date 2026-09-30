#include "chaoshengbo.h"
#include "Delay.h"

uint16_t msHcCount = 0;

// 定时器4初始化
void Tim4_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);

    TIM_DeInit(TIM4);
    TIM_TimeBaseStructure.TIM_Period = (1000 - 1);
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM4, DISABLE);
}

// 定时器中断处理函数
void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        msHcCount++;
    }
}

// 打开定时器
static void OpenTimerForHc(void)
{
    TIM_SetCounter(TIM4, 0);
    msHcCount = 0;
    TIM_Cmd(TIM4, ENABLE);
}

// 关闭定时器
static void CloseTimerForHc(void)
{
    TIM_Cmd(TIM4, DISABLE);
}

// 获取定时器计数值
uint32_t GetEchoTimer(void)
{
    uint32_t t = msHcCount * 1000 + TIM_GetCounter(TIM4);
    TIM_SetCounter(TIM4, 0);
    return t;
}

//超声波1 有害垃圾
void Hcsr05_1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 配置TRIG引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin = HCSR05_1_TRIG_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HCSR05_1_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR05_1_PORT, HCSR05_1_TRIG_PIN); // 默认低电平

    // 配置ECHO引脚为浮空输入
    GPIO_InitStructure.GPIO_Pin = HCSR05_1_ECHO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(HCSR05_1_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR05_1_PORT, HCSR05_1_ECHO_PIN); // 默认低电平
}

//超声波2 可回收垃圾
void Hcsr05_2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 配置TRIG引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin = HCSR05_2_TRIG_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HCSR05_2_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR05_2_PORT, HCSR05_2_TRIG_PIN); // 默认低电平

    // 配置ECHO引脚为浮空输入
    GPIO_InitStructure.GPIO_Pin = HCSR05_2_ECHO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(HCSR05_2_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR05_2_PORT, HCSR05_2_ECHO_PIN); // 默认低电平
}
//超声波3 厨余垃圾
void Hcsr05_3_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置TRIG引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin = HCSR05_3_TRIG_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HCSR05_3_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR05_3_PORT, HCSR05_3_TRIG_PIN); // 默认低电平

    // 配置ECHO引脚为浮空输入
    GPIO_InitStructure.GPIO_Pin = HCSR05_3_ECHO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(HCSR05_3_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR05_3_PORT, HCSR05_3_ECHO_PIN); // 默认低电平
}


//超声波4 其他垃圾
void Hcsr05_4_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 配置TRIG引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin = HCSR05_4_TRIG_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HCSR05_4_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR05_4_PORT, HCSR05_4_TRIG_PIN); // 默认低电平

    // 配置ECHO引脚为浮空输入
    GPIO_InitStructure.GPIO_Pin = HCSR05_4_ECHO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(HCSR05_4_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR05_4_PORT, HCSR05_4_ECHO_PIN); // 默认低电平
}


/* 测距函数,需输入对应的垃圾型号从而选择对应的超声波
1：有害垃圾
2：可回收垃圾
3：厨余垃圾
4：其他垃圾
假如测出的距离lengthTemp小于9cm，则会返回满载信号1
*/
float Hcsr05GetLength(uint8_t sensor)
{
   u32 t = 0;
   int i = 0;
   float lengthTemp = 0;
   float sum = 0;
		int a=0;
    // 根据传感器编号选择对应的引脚
    GPIO_TypeDef* TRIG_PORT;
    GPIO_TypeDef* ECHO_PORT;
    uint16_t TRIG_PIN;
    uint16_t ECHO_PIN;

    switch (sensor)
    {
        case 1:
            TRIG_PORT = HCSR05_1_PORT;
            ECHO_PORT = HCSR05_1_PORT;
            TRIG_PIN = HCSR05_1_TRIG_PIN;
            ECHO_PIN = HCSR05_1_ECHO_PIN;
            break;
        case 2:
            TRIG_PORT = HCSR05_2_PORT;
            ECHO_PORT = HCSR05_2_PORT;
            TRIG_PIN = HCSR05_2_TRIG_PIN;
            ECHO_PIN = HCSR05_2_ECHO_PIN;
            break;
        case 3:
            TRIG_PORT = HCSR05_3_PORT;
            ECHO_PORT = HCSR05_3_PORT;
            TRIG_PIN = HCSR05_3_TRIG_PIN;
            ECHO_PIN = HCSR05_3_ECHO_PIN;
            break;
        case 4:
            TRIG_PORT = HCSR05_4_PORT;
            ECHO_PORT = HCSR05_4_PORT;
            TRIG_PIN = HCSR05_4_TRIG_PIN;
            ECHO_PIN = HCSR05_4_ECHO_PIN;
            break;
        default:
            return -1.0; // 无效的传感器编号
    }

			GPIO_SetBits(TRIG_PORT,TRIG_PIN); //拉高TRIG脚 发送超过10us的高电平
		  Delay_us(20);
			GPIO_ResetBits(TRIG_PORT,TRIG_PIN); //拉低TRIG
      i = i + 1;
      while(GPIO_ReadInputDataBit(TRIG_PORT,ECHO_PIN) == 0);  //等待ECHO脚高电平 跳出while循环
			OpenTimerForHc();         //打开定时器
			while(GPIO_ReadInputDataBit(TRIG_PORT,ECHO_PIN) == 1){} //等待ECHO脚高电平 跳出while循环
      CloseTimerForHc();         //关闭定时器
      t = GetEchoTimer();        //获取定时器计数的值 
      lengthTemp = ((float)t/58.0);//cm
      sum = lengthTemp + sum ;
        return lengthTemp;
//			 // 判断测量的距离是否小于9cm
//			 if (lengthTemp < 10.0)
//			 {
//					 return 1.0; // 满载
//			 }
//			 else
//			 {
//					 return 0.0; // 不满足条件返回0
			 }

