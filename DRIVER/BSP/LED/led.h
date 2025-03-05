#ifndef __LED_H
#define __LED_H

#include "stm32f10x_gpio.h"

#define LED_off  GPIO_SetBits( GPIOB, GPIO_Pin_10);
#define LED_on GPIO_ResetBits( GPIOB, GPIO_Pin_10);

void LED_Init(void); //led≥ı ºªØ


#endif
