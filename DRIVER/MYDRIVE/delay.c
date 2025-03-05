#include "stm32f10x.h"
#include "delay.h"
// RTOS
#include "FreeRTOS.h"
#include "task.h"


u32 fac_us;
u32 fac_ms;


// 使用freertos提供的函数
void delay_us(u32 nus)
{
    u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;						//LOAD的值	    	 
	ticks=nus*(72000000/1000000); 	//需要的节拍数	  		 
	tcnt=0;
	vTaskSuspendAll();									//阻止OS调度，防止打断us延时
	told=SysTick->VAL;        					//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;						//时间超过/等于要延迟的时间,则退出.
		}  
	};
	xTaskResumeAll();										//恢复OS调度	
}
void delay_ms(u16 nms)
{
	vTaskDelay(nms);
}


void SysTick_Init(u8 SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SYSCLK/8;
	fac_ms = (u16)fac_us*1000;
}

//// us 延时 不要大于798915us
//void delay_us(u32 nus)
//{
//	u32 temp;
//	SysTick->LOAD = nus*fac_us;  // 加载计数值
//	SysTick->VAL = 0x00; //清空计数值
//	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开始倒计时
//	do{
//      temp = SysTick->CTRL;		
//	}while( ( temp&0x01)&&!(temp&(1<<16)));  // 等待计时
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;  // 关闭计时器
//	SysTick->VAL = 0x00; //清空计数值
//}

//// ms 延时 对72M 条件下,nms<=1864ms
//void delay_ms(u16 nms)
//{
//	u32 temp;
//	SysTick->LOAD = (u32)nms*fac_ms;  // 加载计数值
//	SysTick->VAL = 0x00; //清空计数值
//	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开始倒计时
//	do{
//      temp = SysTick->CTRL;		
//	}while( ( temp&0x01)&&!(temp&(1<<16)));  // 等待计时
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;  // 关闭计时器
//	SysTick->VAL = 0x00; //清空计数值
//}
