#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "Key.h"
#include "Serial.h"
#include "Motor.h"
#include "chaoshengbo.h"
#include "guangdian.h"
#include "tuigan.h"
#include "lvdai.h"
uint8_t flag;			
float Angle;			//定义角度变量
uint8_t RxData;			//定义用于接收串口数据的变量
float  juli;	//超声波测距的距离
uint8_t  fall;	//光电传感器测量物体是否掉落，1为是，0为否

int main(void)
{
	Serial_Init();	//串口初始化
	Servo_Init();		//舵机初始化
	Motor_Init();
//	Motor_Test(1,0);
	GD_Init();//光电初始化
	tuigan_Init();//推杆初始化	
	LVDAI_Init();//履带初始化
	
	//超声波初始化
	Tim4_Init();
	Hcsr05_1_Init();//有害
	Hcsr05_2_Init();//可回收
	Hcsr05_3_Init();//厨余
	Hcsr05_4_Init();//其他

//	Motor_SetSpeed(30);
//	//传送带传动！
	   RelayControl(0);  //履带开启       

	Delay_ms(2500);
	               
  Servo_SetAngle2(150);
	Servo_SetAngle1(30); //不能超过133

	

while(1)
{
								juli=Hcsr05GetLength(1);
//								if (juli == 1)
//								{
										Serial_SendString("man\r\n");  // 如果 juli 为 1，则发送 "满载"
				Serial_Printf("juli: %.3f cm\r\n", juli);
//								}
//								else
//								{
//								}


	
//	   RelayControl(1);  //履带开启   
////判断是否有物体掉落
//		while (1) 
//			{
//				fall = GD_Check_Obstacle();
//					if (fall == 1) 
//			{  
//						// 执行物体掉落时的操作，传送带停止
//					Motor_SetSpeed(0);
//					Serial_Printf("aaaaa\r\n");
//						break;		  // 跳出循环
//			}
//		 }
////舵机判断
//        while (Serial_GetRxFlag() == 0);  // 如果串口没有接收到数据，一直等待

//        // 确保接收到数据后，获取数据并清空接收标志位
//        RxData = Serial_GetRxData();  // 获取串口接收的数据
//        Serial_SendByte(RxData);  // 串口将收到的数据回传回去，用于测试

//        // 根据接收到的数据执行相应的操作
//        switch (RxData)
//        {
//            case '0':  // 有害垃圾
//                Servo_SetAngle2(20);
//                Servo_SetAngle1(150);
//                Delay_ms(800);
//                Servo_SetAngle1(90);
//                Servo_SetAngle2(90);
//						// 判断是否满载
//								juli=Hcsr05GetLength(1);
//								if (juli == 1)
//								{
//										Serial_Printf("man\r\n");  // 如果 juli 为 1，则发送 "满载"
//								}
//								else
//								{
//								}
//                break;

//            case '1':  // 可回收
//                Servo_SetAngle2(130);
//                Servo_SetAngle1(150);
//                Delay_ms(800);
//                Servo_SetAngle1(90);
//                Servo_SetAngle2(90);
//						
//												// 判断是否满载
//								juli=Hcsr05GetLength(2);
//								if (juli == 1)
//								{
//										Serial_Printf("man\r\n");  // 如果 juli 为 1，则发送 "满载"
//								}
//								else
//								{
//								}

////						 //推杆压缩
////								TG_ya_OPEA();//开压
////								Delay_s(10);// 10mm/s的速度，
////								TG_ya_NO();//达最大推杆长度						
////								TG_suo_OPEA();//开缩
////								Delay_s(10);
////								TG_suo_NO();//回初始状态
////							

//                break;

//            case '2':  // 厨余垃圾
//                Servo_SetAngle2(50);
//                Servo_SetAngle1(0);
//                Delay_ms(800);
//                Servo_SetAngle1(90);
//                Servo_SetAngle2(90);
//								// 判断是否满载
//								juli=Hcsr05GetLength(3);
//								if (juli == 1)
//								{
//										Serial_Printf("man\r\n");  // 如果 juli 为 1，则发送 "满载"
//								}
//								else
//								{
//								}
//                break;

//            case '3':  // 其他垃圾
//                Servo_SetAngle2(135);
//                Servo_SetAngle1(0);
//                Delay_ms(800);
//                Servo_SetAngle1(90);
//                Servo_SetAngle2(90);
//								// 判断是否满载
//								juli=Hcsr05GetLength(4);
//								if (juli == 1)
//								{
//										Serial_Printf("man\r\n");  // 如果 juli 为 1，则发送 "满载"
//								}
//								else
//								{
//								}
//                break;

//            default: break;
//        }
//					
//				
//				
//				Motor_SetSpeed(-20);
				
				//传送带动起来！
				
    }
		
	}	