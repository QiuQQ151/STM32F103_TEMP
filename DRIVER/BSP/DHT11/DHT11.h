#ifndef __DHT11_H
#define __DHT11_H	
#include "stm32f10x.h"	
#include "delay.h"

//����PA10,A9ΪIO��
#define FI_DHT11_IO_IN()  {GPIOA->CRH&=0XFFFFF0FF;GPIOA->CRH|=(u32)8<<8;} //��������
#define FI_DHT11_IO_OUT() {GPIOA->CRH&=0XFFFFF0FF;GPIOA->CRH|=(u32)3<<8;} //�������
#define	FI_DHT11_DATE_OUT(x)  ((x) ? GPIO_SetBits(GPIOA, GPIO_Pin_10) : GPIO_ResetBits(GPIOA, GPIO_Pin_10)) //���ݶ˿�	���
#define	FI_DHT11_DATE_IN   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)  //���ݶ˿�	����

#define EX_DHT11_IO_IN()  {GPIOA->CRH&=0XFFFFFF0F;GPIOA->CRH|=(u32)8<<4;} //��������
#define EX_DHT11_IO_OUT() {GPIOA->CRH&=0XFFFFFF0F;GPIOA->CRH|=(u32)3<<4;} //�������
#define	EX_DHT11_DATE_OUT(x)  ((x) ? GPIO_SetBits(GPIOA, GPIO_Pin_9) : GPIO_ResetBits(GPIOA, GPIO_Pin_9)) //���ݶ˿�	���
#define	EX_DHT11_DATE_IN   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)  //���ݶ˿�	����


#define EX_DHT11 0
#define FI_DHT11 1

 u8  DHT11_init(void);     //��ʼ��
 u8  DHT11_COM(u8 DHT11_ID);
 u8  DHT11_GET_RH(u8 DHT11_ID);  //��ȡһ��RH
 u8  DHT11_check(u8 DHT11_ID); //����Ӧ���豸�Ƿ�������0������1�쳣

#endif
