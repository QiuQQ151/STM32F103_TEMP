#include "pwm.h"

void TIM3_PWM_init(u16 arr,u16 psc)  //����LCD_LED
{
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //�ر� JTAG,���� SWD	
	    RCC->APB2ENR|=1<<0;      //ʹ��AFIO
	    AFIO->MAPR&=0XFFFFF3FF;  //��Ӧλ����
	    AFIO->MAPR|=1<<11;	     //tim3 ��ӳ��	10��tim3_ch1��Ӧb4��   

   	 RCC->APB2ENR|=1<<4;  //GPIOCʱ��ʹ��
	   GPIOC->CRH&=0XFFFFFFFF; //��ʼ��C9��
	   GPIOC->CRH|=0X000000B0; //9�ڸ����������

    	RCC->APB1ENR|=1<<1;  //ʹ��tim3ʱ��
			TIM3->ARR=arr;       //�趨�������Զ���װֵ
			TIM3->PSC=psc;       //Ԥ��Ƶ������
			TIM3->CCMR1|=7<<4;   //CH1 PWM2 ģʽ
			TIM3->CCMR1|=1<<3;   //CH1 Ԥװ��ʹ��
			TIM3->CR1=0x0080;    //ARPE ��װ��ʹ��	
	    TIM3->CCER|=1<<1;    //���ü���
    	TIM3->CCER|=1<<0;    //OC1 ���ʹ��	
			TIM3->CR1|=0x01;     //ʹ�ܶ�ʱ��3����	
}

void TIM1_PWM_init(u16 arr,u16 psc)  //����buzzer
{
	   RCC->APB2ENR|=1<<2;  //GPIOAʱ��ʹ��
	   GPIOA->CRH&=0XFFFFFFF0; //��ʼ��A8��
	   GPIOA->CRH|=0X0000000B; //�����������
	
	   RCC->APB2ENR|=1<<11;  //ʹ��tim1ʱ��
		 TIM1->ARR=arr;        //�趨�������Զ���װֵ
		 TIM1->PSC=psc;        //Ԥ��Ƶ������
		 TIM1->CCMR1|=7<<4;    //CH1 PWM2 ģʽ
		 TIM1->CCMR1|=1<<3;    //CH1 Ԥװ��ʹ��
	   TIM1->CCER|=1<<0;     //OC1 ���ʹ��
	   TIM1->BDTR|=1<<15;    //MOE �����ʹ��
		 TIM1->CR1=0x0080;     //ARPE ��װ��ʹ��	
	   TIM1->CCER|=1<<1;     //���ü���	
		 TIM1->CR1|=0x01;      //ʹ�ܶ�ʱ������	
	
}

