#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);
void Motor_SetSpeed(uint8_t motor, int8_t Speed);
void Motor_Test(int8_t KeyNum, uint8_t motor, int8_t Speed);
#endif
