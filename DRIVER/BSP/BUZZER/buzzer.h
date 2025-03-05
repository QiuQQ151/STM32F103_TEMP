#ifndef __BUZZER_H
#define __BUZZER_H
#include "stm32f10x.h"



//#define buzzer_off GPIO_ResetBits(GPIOA,GPIO_Pin_8);
//#define buzzer_on  GPIO_SetBits(GPIOA,GPIO_Pin_8);
#define buzzer_off  TIM1->CCR1=0;
#define buzzer_on   TIM1->CCR1=400;
#define buzzer_arl  TIM1->CCR1=20;

void Buzzer_Init(u16 arr,u16 buzzer);
//void Buzzer_on(u8 time);

#endif
