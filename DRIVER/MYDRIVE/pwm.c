#include "pwm.h"

void TIM3_PWM_init(u16 arr,u16 psc)  //驱动LCD_LED
{
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //关闭 JTAG,开启 SWD	
	    RCC->APB2ENR|=1<<0;      //使能AFIO
	    AFIO->MAPR&=0XFFFFF3FF;  //对应位清零
	    AFIO->MAPR|=1<<11;	     //tim3 重映射	10，tim3_ch1对应b4口   

   	 RCC->APB2ENR|=1<<4;  //GPIOC时钟使能
	   GPIOC->CRH&=0XFFFFFFFF; //初始化C9口
	   GPIOC->CRH|=0X000000B0; //9口复用推挽输出

    	RCC->APB1ENR|=1<<1;  //使能tim3时钟
			TIM3->ARR=arr;       //设定计数器自动重装值
			TIM3->PSC=psc;       //预分频器设置
			TIM3->CCMR1|=7<<4;   //CH1 PWM2 模式
			TIM3->CCMR1|=1<<3;   //CH1 预装载使能
			TIM3->CR1=0x0080;    //ARPE 重装载使能	
	    TIM3->CCER|=1<<1;    //设置极性
    	TIM3->CCER|=1<<0;    //OC1 输出使能	
			TIM3->CR1|=0x01;     //使能定时器3计数	
}

void TIM1_PWM_init(u16 arr,u16 psc)  //驱动buzzer
{
	   RCC->APB2ENR|=1<<2;  //GPIOA时钟使能
	   GPIOA->CRH&=0XFFFFFFF0; //初始化A8口
	   GPIOA->CRH|=0X0000000B; //复用推挽输出
	
	   RCC->APB2ENR|=1<<11;  //使能tim1时钟
		 TIM1->ARR=arr;        //设定计数器自动重装值
		 TIM1->PSC=psc;        //预分频器设置
		 TIM1->CCMR1|=7<<4;    //CH1 PWM2 模式
		 TIM1->CCMR1|=1<<3;    //CH1 预装载使能
	   TIM1->CCER|=1<<0;     //OC1 输出使能
	   TIM1->BDTR|=1<<15;    //MOE 主输出使能
		 TIM1->CR1=0x0080;     //ARPE 重装载使能	
	   TIM1->CCER|=1<<1;     //设置极性	
		 TIM1->CR1|=0x01;      //使能定时器计数	
	
}

