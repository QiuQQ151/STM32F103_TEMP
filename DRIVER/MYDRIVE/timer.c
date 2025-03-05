#include "timer.h"
#include "stm32f10x.h"
#include "buzzer.h"
#include "led.h"
#include "dht11.h"
#include "adc.h"
#include "display.h"
#include "lcd.h"
#include "gui.h"
#include "malloc.h"
u8 system_background_time=5;   //��̨����ʱ�䣨s��
u8 system_background_flag=0;    //1��̨ʱ���ѵ���0δ��ʱ��
u8 system_time_count=0;  //��̨ʱ�����
extern u8 sys_busy;
extern float  Battery_v1,Battery_v2; //��ص�ѹ
extern u8 LCD_light;      //��ʾ������
extern u8 Auto_LCD_Led;
extern u8 DHT11_background_run;//1�ں�̨��¼��0�޺�̨��¼
extern u8 DHT11_warning_flag;  //1������ʪ�ȳ��꾯�棬0�رվ���
extern u8 DHT11_record[];
extern u8 DHT11_T_max;          //��ʪ�ȷ�Χ��¼
extern u8 DHT11_T_min;
extern u8 DHT11_RH_max;
extern u8 DHT11_RH_min;

//ͨ�ö�ʱ��2�жϳ�ʼ��
//����ʱ��ѡ��Ϊ APB1��2������APB1Ϊ 36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��!
void TIM2_Int_Init(u16 arr,u16 psc)
{
    RCC->APB1ENR|=1<<0; //TIM2 ʱ��ʹ��
		TIM2->ARR=arr; //�趨�������Զ���װֵ//�պ� 1ms
		TIM2->PSC=psc; //Ԥ��Ƶ�� 7200,�õ� 10Khz �ļ���ʱ��
		TIM2->DIER|=1<<0; //��������ж�
	//	TIM2->CR1|=0x01; //ʹ�ܶ�ʱ�� 2 ,//�Ȳ�ʹ�ܣ��ڴ����ж���ʹ��
		MY_NVIC_Init(1,3,TIM2_IRQn,2);//��ռ 1�������ȼ� 3���� 2
}
//����ʱ��ѡ��Ϊ APB1��2������APB1Ϊ 36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��!
void TIM4_Int_Init(u16 arr,u16 psc)
{
    RCC->APB1ENR|=1<<2; //TIM2 ʱ��ʹ��
		TIM4->ARR=arr; //�趨�������Զ���װֵ//�պ� 1ms
		TIM4->PSC=psc; //Ԥ��Ƶ�� 7200,�õ� 10Khz �ļ���ʱ��
		TIM4->DIER|=1<<0; //��������ж�
		TIM4->CR1|=0x01; //ʹ�ܶ�ʱ�� 2 ,//�Ȳ�ʹ�ܣ��ڴ����ж���ʹ��
		MY_NVIC_Init(1,3,TIM4_IRQn,2);//��ռ 1�������ȼ� 3���� 2
}





void TIM2_IRQHandler(void)
{
    if(TIM2->SR&0X0001) 
		{ buzzer_volume=0;//����ж�//�رշ���������
			TIM2->CR1&=0xFFFE; //ֹͣ����
		}
    TIM2->SR&=~(1<<0);//����жϱ�־λ
}

void TIM4_IRQHandler(void)
{
    if(TIM4->SR&0X0001) 
		{  system_time_count+=1; //ÿ�μ�1s
			 if( sys_busy==0&&(system_time_count>=system_background_time) )//
			 {    
				    system_background_flag=1; //��ִ̨����
				    system_time_count=0;
				    LED_work=0;
//				     POINT_COLOR=BLACK;
//           ShowNum(170,170,EXmem_perused(),5,16);
//						 //��ȡ��ʪ��
//						if(DHT11_background_run==1)
//						{
//							 Touch_disable;
//							 DHT11_GET_RH(FI_DHT11);//������ʪ��
//							 Touch_enable;
//							 DHT11_SaveDataToEEPROM2();					
//						}
//					 //��ʪ�ȱ���
//						if(DHT11_warning_flag==1)
//						{
//							if(DHT11_record[0]<=DHT11_T_min || DHT11_record[0]>=DHT11_T_max || DHT11_record[2]<=DHT11_RH_min || DHT11_record[2]>=DHT11_RH_max )
//							{ buzzer_volume=500;TIM2->CR1|=0x01;}
//							
//						}
//						 //��ȡ��ص�ѹ
//						 ADC1->CR2|=(1<<0); //��ADC1
//						 Battery_v1=Get_Adc_Average(1,20);								
//						 Battery_v2=Get_Adc_Average(2,20);			
//						 Battery_v1=(float)Battery_v1*(3.28/4096)*1000*2; //�Ŵ�
//						 Battery_v2=(float)Battery_v2*(3.28/4096)*1000*2; //�Ŵ�				 				  
//						 //��Ļ�Զ������趨
//						   if(Auto_LCD_Led==1)
//						{ 	LCD_LED=LCD_light*3-1+Get_Adc_Average(8,2)*3;}
//						else 
//            {   LCD_LED=LCD_light*10-1;}
//						
//						ADC1->CR2&=0XFFFFFFFE; //ֹͣADC���������ģʽ						
						
						
			      
			     LED_work=1;
			 }
     
		}
    TIM4->SR&=~(1<<0);//����жϱ�־λ
}


