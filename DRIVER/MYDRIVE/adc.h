#ifndef __ADC_H
#define __ADC_H
#include "stm32f10x.h"

#define ADC_CH1 1 //通道 1
void Adc_Init(void); //ADC 通道初始化
u16 Get_Adc(u8 ch); //获得某个通道值
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道 10 次采样的平均值


#endif
