#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"
#include "stdio.h"	 
 
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_UART4_RX 		  	0		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;            		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 pclk2,u32 bound);

void UART_SendBytes(u8 * BUFF ,u16 num);
void send_data_AN(u16 function,u16 A);

#endif	   
















