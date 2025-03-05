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
u8 system_background_time=5;   //后台节拍时间（s）
u8 system_background_flag=0;    //1后台时间已到，0未到时间
u8 system_time_count=0;  //后台时间计数
extern u8 sys_busy;
extern float  Battery_v1,Battery_v2; //电池电压
extern u8 LCD_light;      //显示屏亮度
extern u8 Auto_LCD_Led;
extern u8 DHT11_background_run;//1在后台记录，0无后台记录
extern u8 DHT11_warning_flag;  //1开启温湿度超标警告，0关闭警告
extern u8 DHT11_record[];
extern u8 DHT11_T_max;          //温湿度范围记录
extern u8 DHT11_T_min;
extern u8 DHT11_RH_max;
extern u8 DHT11_RH_min;

//通用定时器2中断初始化
//这里时钟选择为 APB1的2倍，而APB1为 36M
//arr：自动重装值。
//psc：时钟预分频数!
void TIM2_Int_Init(u16 arr,u16 psc)
{
    RCC->APB1ENR|=1<<0; //TIM2 时钟使能
		TIM2->ARR=arr; //设定计数器自动重装值//刚好 1ms
		TIM2->PSC=psc; //预分频器 7200,得到 10Khz 的计数时钟
		TIM2->DIER|=1<<0; //允许更新中断
	//	TIM2->CR1|=0x01; //使能定时器 2 ,//先不使能，在触摸中断中使能
		MY_NVIC_Init(1,3,TIM2_IRQn,2);//抢占 1，子优先级 3，组 2
}
//这里时钟选择为 APB1的2倍，而APB1为 36M
//arr：自动重装值。
//psc：时钟预分频数!
void TIM4_Int_Init(u16 arr,u16 psc)
{
    RCC->APB1ENR|=1<<2; //TIM2 时钟使能
		TIM4->ARR=arr; //设定计数器自动重装值//刚好 1ms
		TIM4->PSC=psc; //预分频器 7200,得到 10Khz 的计数时钟
		TIM4->DIER|=1<<0; //允许更新中断
		TIM4->CR1|=0x01; //使能定时器 2 ,//先不使能，在触摸中断中使能
		MY_NVIC_Init(1,3,TIM4_IRQn,2);//抢占 1，子优先级 3，组 2
}





void TIM2_IRQHandler(void)
{
    if(TIM2->SR&0X0001) 
		{ buzzer_volume=0;//溢出中断//关闭蜂鸣器声音
			TIM2->CR1&=0xFFFE; //停止计数
		}
    TIM2->SR&=~(1<<0);//清除中断标志位
}

void TIM4_IRQHandler(void)
{
    if(TIM4->SR&0X0001) 
		{  system_time_count+=1; //每次加1s
			 if( sys_busy==0&&(system_time_count>=system_background_time) )//
			 {    
				    system_background_flag=1; //后台执行中
				    system_time_count=0;
				    LED_work=0;
//				     POINT_COLOR=BLACK;
//           ShowNum(170,170,EXmem_perused(),5,16);
//						 //读取温湿度
//						if(DHT11_background_run==1)
//						{
//							 Touch_disable;
//							 DHT11_GET_RH(FI_DHT11);//更新温湿度
//							 Touch_enable;
//							 DHT11_SaveDataToEEPROM2();					
//						}
//					 //温湿度报警
//						if(DHT11_warning_flag==1)
//						{
//							if(DHT11_record[0]<=DHT11_T_min || DHT11_record[0]>=DHT11_T_max || DHT11_record[2]<=DHT11_RH_min || DHT11_record[2]>=DHT11_RH_max )
//							{ buzzer_volume=500;TIM2->CR1|=0x01;}
//							
//						}
//						 //读取电池电压
//						 ADC1->CR2|=(1<<0); //打开ADC1
//						 Battery_v1=Get_Adc_Average(1,20);								
//						 Battery_v2=Get_Adc_Average(2,20);			
//						 Battery_v1=(float)Battery_v1*(3.28/4096)*1000*2; //放大
//						 Battery_v2=(float)Battery_v2*(3.28/4096)*1000*2; //放大				 				  
//						 //屏幕自动亮度设定
//						   if(Auto_LCD_Led==1)
//						{ 	LCD_LED=LCD_light*3-1+Get_Adc_Average(8,2)*3;}
//						else 
//            {   LCD_LED=LCD_light*10-1;}
//						
//						ADC1->CR2&=0XFFFFFFFE; //停止ADC，进入掉电模式						
						
						
			      
			     LED_work=1;
			 }
     
		}
    TIM4->SR&=~(1<<0);//清除中断标志位
}


