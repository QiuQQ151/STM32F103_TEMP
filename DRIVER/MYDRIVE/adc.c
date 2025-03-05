#include "stm32f10x.h"

#include "adc.h"
#include "delay.h"
#include "led.h"

void Adc_Init(void)
{
	 //ADC�����Ϊ
     RCC->APB2ENR|=1<<2;      //ʹ�� PORTA ��ʱ��
     GPIOA->CRL&=0X0FFF000F;  //PA1\2\3 anolog ����
	 GPIOA->CRL|=0<<1;     //PA1\2\3\7 anolog ����

	
   RCC->APB2ENR|=1<<9;      //ADC1 ʱ��ʹ��
   RCC->APB2RSTR|=1<<9;     //ADC1 ��λ
   RCC->APB2RSTR&=~(1<<9);  //��λ����
   RCC->CFGR&=~(3<<14);     //��Ƶ��������
                            //SYSCLK/DIV2=12M ADC ʱ������Ϊ 12M,ADC ���ʱ�Ӳ��ܳ��� 14M!
                            //���򽫵��� ADC ׼ȷ���½�!
		RCC->CFGR|=2<<14;       //12Mhz
		ADC1->CR1&=0XF0FFFF;    //����ģʽ����
		ADC1->CR1|=0<<16;       //��������ģʽ
		ADC1->CR1&=~(1<<8);     //�ر�ɨ��ģʽ
		ADC1->CR2&=~(1<<1);     //����ת��ģʽ
	
		ADC1->CR2&=~(7<<17);    //����
		ADC1->CR2|=7<<17;       //�������ת��
		ADC1->CR2|=1<<20;       //ʹ�����ⲿ����(SWSTART)!!! ����ʹ��һ���¼�������
		ADC1->CR2&=~(1<<11);    //�Ҷ���
	
		ADC1->SQR1&=~(0XF<<20); //����
		ADC1->SQR1|=0<<20;      //1 ��ת���ڹ��������� Ҳ����ֻת���������� 1
		                    
	  //����ͨ�� 1\2\3�Ĳ���ʱ��
		ADC1->SMPR2&=~(0X1F<<3);  //ͨ�� ����ʱ�����
		ADC1->SMPR2|=0X1F<<3;     //ͨ�� 239.5 ����,��߲���ʱ�������߾�ȷ��
    
		ADC1->SMPR2&=~(0X7<<21);  //ͨ�� ����ʱ�����
		ADC1->SMPR2|=0X7<<21;     //ͨ�� 239.5 ����,��߲���ʱ�������߾�ȷ��		
		
		ADC1->SMPR1&=~(0X1F<<18);  //ͨ��16/17����ʱ�����
		ADC1->SMPR1|=0X1F<<18;     //ͨ�� 239.5 ����,��߲���ʱ�������߾�ȷ��		
    ADC1->CR2|=1<<23;          //�����ڲ��¶ȴ�������VREF����ʹ��
		
		ADC1->CR2|=1<<0;       //���� AD ת����
		ADC1->CR2|=1<<3;       //ʹ�ܸ�λУ׼
		while(ADC1->CR2&1<<3); //�ȴ���λУ׼����
		ADC1->CR2|=1<<2;       //���� AD У׼
		while(ADC1->CR2&1<<2); //�ȴ�У׼����
		
		//ADC1->CR2&=~(1<<0); //ֹͣADC���������ģʽ
}


//��� ADC ֵ
//ch:ͨ��ֵ 0~16
//����ֵ:ת�����
u16 Get_Adc(u8 ch)
{  
	
   //����ת������
    ADC1->SQR3&=0XFFFFFFE0;//�������� 1 ͨ�� ch
    ADC1->SQR3|=ch;
	
    ADC1->CR2|=1<<22;       //��������ת��ͨ��
    while(!(ADC1->SR&1<<1));//�ȴ�ת������
	  
    return ADC1->DR; //���� adc ֵ
}


//��ȡͨ�� ch ��ת��ֵ��ȡ times ��,Ȼ��ƽ��
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ�� ch �� times ��ת�����ƽ��ֵ
#define Lost_number 8
#define ADC_times   10
u16 Get_Adc_Average(u8 ch,u8 times)
{ 
    u32 temp_val=0; u8 t;
	 LED_on;
    for(t=0;t<times;t++)
    { temp_val+=Get_Adc(ch); }
	 LED_off;
    return temp_val/times;
}

