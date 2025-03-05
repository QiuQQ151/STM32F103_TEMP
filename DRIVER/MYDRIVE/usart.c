#include "usart.h"	  
#include "string.h"
 	  
 
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定向fputc函数
//printf的输出，指向fputc，由fputc输出到串口
//这里使用串口1(USART1)输出printf信息
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//等待上一次串口数据发送完成  
	USART3->DR = (u8) ch;      	//写DR,串口1将发送数据
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////



#if EN_USART3_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void USART4_IRQHandler(void)
{
	u8 res;	
	
	if(UART4->SR&(1<<5))	//接收到数据
	{	 
		res=UART4->DR;  //读出数据
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}else //还没收到0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}  		 									     
	}
} 
#endif	


//初始化IO 串口3
//pclk:PCLK1时钟频率(Mhz)
//bound:波特率 
void uart_init(u32 pclk,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
  
	
	//重映射USART到PC10、11
	RCC->APB2ENR|=1<<0; //启动AFIO时钟
	AFIO->MAPR&=~(0x1<<4);
	AFIO->MAPR|=0x1<<4;
	
	RCC->APB1ENR|=1<<18;  //使能串口3时钟 
	
	RCC->APB2ENR|=1<<4;   //使能PORTC口时钟  
	GPIOC->CRH&=0XFFFF00FF;//IO状态设置
	GPIOC->CRH|=0X00008B00;//IO状态设置 
	
	RCC->APB1RSTR|=1<<18;   //复位串口3
	RCC->APB1RSTR&=~(1<<18);//停止复位	   	   
	
	//波特率设置
 	USART3->BRR=mantissa; // 波特率设置	 
	USART3->CR1|=0X200C;  //1位停止,无校验位.
#if EN_USART3_RX		  //如果使能了接收
	//使能接收中断 
	UART4->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级 
#endif
}

void UART_SendBytes(u8 * BUFF ,u16 num)
{
	u32 i;
	for( i=0;i<num;i++)
	{
	while((USART3->SR&0X40)==0);//等待上一次串口数据发送完成  
	USART3->DR = *(BUFF+i);     //写DR,串口3将发送数据					
	}
}




//匿名上位机发送数据
void send_data_AN(u16 function,u16 A)
{
	u8 i;
	u8 sumcheck=0;
	u8 addcheck=0;
	u8 cnt=0;
	u8 BUFF[8]; //总长
	
	BUFF[0]=0XAA;    
	cnt++;
	BUFF[1]=0xFF;
	cnt++;
	BUFF[2]=function;
	cnt++;
	BUFF[3]=0x02;//数据长度
	cnt++;  
	BUFF[4]=A&0x00FF;
	cnt++;
	BUFF[5]=A>>8;
	cnt++;

for(i=0; i < (BUFF[3]+4); i++)
{
   sumcheck +=BUFF[i]; //从帧头开始，对每一字节进行求和，直到DATA区结束
   addcheck +=sumcheck; //每一字节的求和操作，进行一次sumcheck的累加
}
	BUFF[6]=sumcheck ;
  cnt++;
	BUFF[7]=addcheck ;
  cnt++;
	UART_SendBytes(BUFF ,8);
}






