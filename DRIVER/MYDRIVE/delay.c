#include "stm32f10x.h"
#include "delay.h"
// RTOS
#include "FreeRTOS.h"
#include "task.h"


u32 fac_us;
u32 fac_ms;


// ʹ��freertos�ṩ�ĺ���
void delay_us(u32 nus)
{
    u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;						//LOAD��ֵ	    	 
	ticks=nus*(72000000/1000000); 	//��Ҫ�Ľ�����	  		 
	tcnt=0;
	vTaskSuspendAll();									//��ֹOS���ȣ���ֹ���us��ʱ
	told=SysTick->VAL;        					//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;						//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
	xTaskResumeAll();										//�ָ�OS����	
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

//// us ��ʱ ��Ҫ����798915us
//void delay_us(u32 nus)
//{
//	u32 temp;
//	SysTick->LOAD = nus*fac_us;  // ���ؼ���ֵ
//	SysTick->VAL = 0x00; //��ռ���ֵ
//	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //��ʼ����ʱ
//	do{
//      temp = SysTick->CTRL;		
//	}while( ( temp&0x01)&&!(temp&(1<<16)));  // �ȴ���ʱ
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;  // �رռ�ʱ��
//	SysTick->VAL = 0x00; //��ռ���ֵ
//}

//// ms ��ʱ ��72M ������,nms<=1864ms
//void delay_ms(u16 nms)
//{
//	u32 temp;
//	SysTick->LOAD = (u32)nms*fac_ms;  // ���ؼ���ֵ
//	SysTick->VAL = 0x00; //��ռ���ֵ
//	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //��ʼ����ʱ
//	do{
//      temp = SysTick->CTRL;		
//	}while( ( temp&0x01)&&!(temp&(1<<16)));  // �ȴ���ʱ
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;  // �رռ�ʱ��
//	SysTick->VAL = 0x00; //��ռ���ֵ
//}
