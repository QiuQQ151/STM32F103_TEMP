#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

void TIM3_PWM_init(u16 arr,u16 psc);   //≥ı ºªØ
void TIM1_PWM_init(u16 arr,u16 psc);

#endif
