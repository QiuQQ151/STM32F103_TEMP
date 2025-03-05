#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"
#include "stdio.h"	 
 
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_UART4_RX 		  	0		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;            		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 pclk2,u32 bound);

void UART_SendBytes(u8 * BUFF ,u16 num);
void send_data_AN(u16 function,u16 A);

#endif	   
















