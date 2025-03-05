#ifndef __DHT11_H
#define __DHT11_H	
#include "stm32f10x.h"	
#include "delay.h"

//定义PA10,A9为IO口
#define FI_DHT11_IO_IN()  {GPIOA->CRH&=0XFFFFF0FF;GPIOA->CRH|=(u32)8<<8;} //上拉输入
#define FI_DHT11_IO_OUT() {GPIOA->CRH&=0XFFFFF0FF;GPIOA->CRH|=(u32)3<<8;} //推挽输出
#define	FI_DHT11_DATE_OUT(x)  ((x) ? GPIO_SetBits(GPIOA, GPIO_Pin_10) : GPIO_ResetBits(GPIOA, GPIO_Pin_10)) //数据端口	输出
#define	FI_DHT11_DATE_IN   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)  //数据端口	输入

#define EX_DHT11_IO_IN()  {GPIOA->CRH&=0XFFFFFF0F;GPIOA->CRH|=(u32)8<<4;} //上拉输入
#define EX_DHT11_IO_OUT() {GPIOA->CRH&=0XFFFFFF0F;GPIOA->CRH|=(u32)3<<4;} //推挽输出
#define	EX_DHT11_DATE_OUT(x)  ((x) ? GPIO_SetBits(GPIOA, GPIO_Pin_9) : GPIO_ResetBits(GPIOA, GPIO_Pin_9)) //数据端口	输出
#define	EX_DHT11_DATE_IN   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)  //数据端口	输入


#define EX_DHT11 0
#define FI_DHT11 1

 u8  DHT11_init(void);     //初始化
 u8  DHT11_COM(u8 DHT11_ID);
 u8  DHT11_GET_RH(u8 DHT11_ID);  //获取一次RH
 u8  DHT11_check(u8 DHT11_ID); //检查对应的设备是否正常，0正常，1异常

#endif
