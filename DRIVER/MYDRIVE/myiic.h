#ifndef __MYIIC_H
#define __MYIIC_H
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

//IO方向设置
#define SDA_IN()  {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=8<<0;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=3<<0;}

// 定义 I2C 引脚操作宏
#define READ_SDA       GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)   // 读取 SDA 引脚状态
#define IIC_SCL(x)    ((x) ? GPIO_SetBits(GPIOB, GPIO_Pin_9) : GPIO_ResetBits(GPIOB, GPIO_Pin_9)) // SCL 引脚
#define IIC_SDA(x)    ((x) ? GPIO_SetBits(GPIOB, GPIO_Pin_8) : GPIO_ResetBits(GPIOB, GPIO_Pin_8)) // SDA 引脚

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

u8   ReadOneByte(u8 DeviceAddr,u8 ReadAddr);
void WriteOneByte(u8 DeviceAddr,u8 WriteAddr,u8 DataToWrite);

#endif
















