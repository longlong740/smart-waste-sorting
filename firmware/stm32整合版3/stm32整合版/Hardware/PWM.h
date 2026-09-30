#ifndef __PWM_H
#define __PWM_H

void PWM_Init_Servo(void);
void PWM_SetCompare1_Servo(uint16_t Compare);
void PWM_SetCompare2_Servo(uint16_t Compare);

void PWM_Init_Moter(void);
void PWM_SetCompare1_Moter(uint16_t Compare);
void PWM_SetCompare2_Moter(uint16_t Compare);
void PWM_SetCompare3_Moter(uint16_t Compare);
void PWM_SetCompare4_Moter(uint16_t Compare);

#endif
