#include "usart.h"	  
#include "string.h"
 	  
 
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc����
//printf�������ָ��fputc����fputc���������
//����ʹ�ô���1(USART1)���printf��Ϣ
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//�ȴ���һ�δ������ݷ������  
	USART3->DR = (u8) ch;      	//дDR,����1����������
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////



#if EN_USART3_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void USART4_IRQHandler(void)
{
	u8 res;	
	
	if(UART4->SR&(1<<5))	//���յ�����
	{	 
		res=UART4->DR;  //��������
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}else //��û�յ�0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}  		 									     
	}
} 
#endif	


//��ʼ��IO ����3
//pclk:PCLK1ʱ��Ƶ��(Mhz)
//bound:������ 
void uart_init(u32 pclk,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
  
	
	//��ӳ��USART��PC10��11
	RCC->APB2ENR|=1<<0; //����AFIOʱ��
	AFIO->MAPR&=~(0x1<<4);
	AFIO->MAPR|=0x1<<4;
	
	RCC->APB1ENR|=1<<18;  //ʹ�ܴ���3ʱ�� 
	
	RCC->APB2ENR|=1<<4;   //ʹ��PORTC��ʱ��  
	GPIOC->CRH&=0XFFFF00FF;//IO״̬����
	GPIOC->CRH|=0X00008B00;//IO״̬���� 
	
	RCC->APB1RSTR|=1<<18;   //��λ����3
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ	   	   
	
	//����������
 	USART3->BRR=mantissa; // ����������	 
	USART3->CR1|=0X200C;  //1λֹͣ,��У��λ.
#if EN_USART3_RX		  //���ʹ���˽���
	//ʹ�ܽ����ж� 
	UART4->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//��2��������ȼ� 
#endif
}

void UART_SendBytes(u8 * BUFF ,u16 num)
{
	u32 i;
	for( i=0;i<num;i++)
	{
	while((USART3->SR&0X40)==0);//�ȴ���һ�δ������ݷ������  
	USART3->DR = *(BUFF+i);     //дDR,����3����������					
	}
}




//������λ����������
void send_data_AN(u16 function,u16 A)
{
	u8 i;
	u8 sumcheck=0;
	u8 addcheck=0;
	u8 cnt=0;
	u8 BUFF[8]; //�ܳ�
	
	BUFF[0]=0XAA;    
	cnt++;
	BUFF[1]=0xFF;
	cnt++;
	BUFF[2]=function;
	cnt++;
	BUFF[3]=0x02;//���ݳ���
	cnt++;  
	BUFF[4]=A&0x00FF;
	cnt++;
	BUFF[5]=A>>8;
	cnt++;

for(i=0; i < (BUFF[3]+4); i++)
{
   sumcheck +=BUFF[i]; //��֡ͷ��ʼ����ÿһ�ֽڽ�����ͣ�ֱ��DATA������
   addcheck +=sumcheck; //ÿһ�ֽڵ���Ͳ���������һ��sumcheck���ۼ�
}
	BUFF[6]=sumcheck ;
  cnt++;
	BUFF[7]=addcheck ;
  cnt++;
	UART_SendBytes(BUFF ,8);
}






