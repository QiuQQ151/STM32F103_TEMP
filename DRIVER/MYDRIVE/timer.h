#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"

void TIM2_IRQHandler(void);
void TIM2_Int_Init(u16 arr,u16 psc);  //控制蜂鸣器响一次的时延
void TIM4_IRQHandler(void);
void TIM4_Int_Init(u16 arr,u16 psc);  //控制系统节拍

#endif
