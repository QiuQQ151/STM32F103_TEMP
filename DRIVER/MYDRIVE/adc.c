#include "stm32f10x.h"

#include "adc.h"
#include "delay.h"
#include "led.h"

void Adc_Init(void)
{
	 //ADC输入口为
     RCC->APB2ENR|=1<<2;      //使能 PORTA 口时钟
     GPIOA->CRL&=0X0FFF000F;  //PA1\2\3 anolog 输入
	 GPIOA->CRL|=0<<1;     //PA1\2\3\7 anolog 输入

	
   RCC->APB2ENR|=1<<9;      //ADC1 时钟使能
   RCC->APB2RSTR|=1<<9;     //ADC1 复位
   RCC->APB2RSTR&=~(1<<9);  //复位结束
   RCC->CFGR&=~(3<<14);     //分频因子清零
                            //SYSCLK/DIV2=12M ADC 时钟设置为 12M,ADC 最大时钟不能超过 14M!
                            //否则将导致 ADC 准确度下降!
		RCC->CFGR|=2<<14;       //12Mhz
		ADC1->CR1&=0XF0FFFF;    //工作模式清零
		ADC1->CR1|=0<<16;       //独立工作模式
		ADC1->CR1&=~(1<<8);     //关闭扫描模式
		ADC1->CR2&=~(1<<1);     //单次转换模式
	
		ADC1->CR2&=~(7<<17);    //清零
		ADC1->CR2|=7<<17;       //软件控制转换
		ADC1->CR2|=1<<20;       //使用用外部触发(SWSTART)!!! 必须使用一个事件来触发
		ADC1->CR2&=~(1<<11);    //右对齐
	
		ADC1->SQR1&=~(0XF<<20); //清零
		ADC1->SQR1|=0<<20;      //1 个转换在规则序列中 也就是只转换规则序列 1
		                    
	  //设置通道 1\2\3的采样时间
		ADC1->SMPR2&=~(0X1F<<3);  //通道 采样时间清空
		ADC1->SMPR2|=0X1F<<3;     //通道 239.5 周期,提高采样时间可以提高精确度
    
		ADC1->SMPR2&=~(0X7<<21);  //通道 采样时间清空
		ADC1->SMPR2|=0X7<<21;     //通道 239.5 周期,提高采样时间可以提高精确度		
		
		ADC1->SMPR1&=~(0X1F<<18);  //通道16/17采样时间清空
		ADC1->SMPR1|=0X1F<<18;     //通道 239.5 周期,提高采样时间可以提高精确度		
    ADC1->CR2|=1<<23;          //允许内部温度传感器和VREF采样使能
		
		ADC1->CR2|=1<<0;       //开启 AD 转换器
		ADC1->CR2|=1<<3;       //使能复位校准
		while(ADC1->CR2&1<<3); //等待复位校准结束
		ADC1->CR2|=1<<2;       //开启 AD 校准
		while(ADC1->CR2&1<<2); //等待校准结束
		
		//ADC1->CR2&=~(1<<0); //停止ADC，进入掉电模式
}


//获得 ADC 值
//ch:通道值 0~16
//返回值:转换结果
u16 Get_Adc(u8 ch)
{  
	
   //设置转换序列
    ADC1->SQR3&=0XFFFFFFE0;//规则序列 1 通道 ch
    ADC1->SQR3|=ch;
	
    ADC1->CR2|=1<<22;       //启动规则转换通道
    while(!(ADC1->SR&1<<1));//等待转换结束
	  
    return ADC1->DR; //返回 adc 值
}


//获取通道 ch 的转换值，取 times 次,然后平均
//ch:通道编号
//times:获取次数
//返回值:通道 ch 的 times 次转换结果平均值
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

