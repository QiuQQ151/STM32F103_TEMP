#include "exti.h"
#include "stm32f10x.h"

#include "delay.h"
#include "lcd.h"
#include "buzzer.h"
#include "display.h"


//extern u8 Touch_enable; //触摸允许控制
extern u8 LCD_T_buzzer;
extern u8 sys_busy;

void EXTI_init(void)
{

    Ex_NVIC_Config(GPIO_B,1,FTIR); //下降沿触发 //触屏信号触发
    MY_NVIC_Init(2,2,EXTI1_IRQn,2); //抢占 2，子优先级 2，组2 //对应B1口
    
}

void EXTI1_IRQHandler(void)
{   
    //更新触控坐标
    TP_Scan(1); 
//	 Buzzer_on(100);
//	 ShowNum(10,200,Touch_x,4,16);
//	 ShowNum(10,220,Touch_y,4,16);
//	 ShowNum(80,200,tp_dev.xfac*1000,8,16);
//	 ShowNum(160,200,tp_dev.xoff,8,16);
//	 ShowNum(80,220,tp_dev.yfac*1000,8,16);
//	 ShowNum(160,220,tp_dev.yoff,8,16);	
	 if(LCD_T_buzzer)  //启动蜂鸣器输出定时中断
	 { buzzer_volume=500;TIM2->CR1|=0x01; }
    EXTI->PR=1<<1; //清除 LINE1 上的中断标志位
}