#include "stm32f10x.h"

void GPIO_Configuration(void);
void USART3_Configuration(void);

int main(void)
{
    GPIO_Configuration();
    USART3_Configuration();

    while(1)
    {
        // 检测PA9输入状态
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == Bit_RESET)
        {
            // 发送字符串"yes\r\n"
            char *msg = "yes\r\n";
            while(*msg)
            {
                USART_SendData(USART3, *msg++);
                while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
            }
            // 添加延时防止重复发送
            Delay_ms(200);
        }
        Delay_ms(10);  // 主循环延时
    }
}

void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOA和GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // 配置PA9为浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 配置USART3 TX(PB10)为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void USART3_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;
    
    // 使能USART3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    
    // USART3参数配置
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    
    // 使能USART3
    USART_Cmd(USART3, ENABLE);
}



#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
    while(1);
}
#endif

