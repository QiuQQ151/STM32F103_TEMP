#ifndef __MYIIC_H
#define __MYIIC_H
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

//IO��������
#define SDA_IN()  {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=8<<0;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=3<<0;}

// ���� I2C ���Ų�����
#define READ_SDA       GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)   // ��ȡ SDA ����״̬
#define IIC_SCL(x)    ((x) ? GPIO_SetBits(GPIOB, GPIO_Pin_9) : GPIO_ResetBits(GPIOB, GPIO_Pin_9)) // SCL ����
#define IIC_SDA(x)    ((x) ? GPIO_SetBits(GPIOB, GPIO_Pin_8) : GPIO_ResetBits(GPIOB, GPIO_Pin_8)) // SDA ����

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

u8   ReadOneByte(u8 DeviceAddr,u8 ReadAddr);
void WriteOneByte(u8 DeviceAddr,u8 WriteAddr,u8 DataToWrite);

#endif
















