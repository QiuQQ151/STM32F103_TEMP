#include "stm32f10x.h"
#include "buzzer.h"
#include "pwm.h"
//#include "24cxx.h"


//extern u8 LCD_T_buzzer; //0关闭触摸音，1开启
void Buzzer_Init(u16 arr,u16 buzzer)
{
//	LCD_T_buzzer=AT24CXX_ReadOneByte(EEPROM1,35); //读取保存的设置
	TIM1_PWM_init(arr,buzzer);
	TIM1->CCR1=0;
	
//	GPIO_InitTypeDef GPIO_InitStr;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	
//	GPIO_InitStr.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStr.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStr.GPIO_Speed = GPIO_Speed_50MHz;
//	
//	GPIO_Init(GPIOA, &GPIO_InitStr);

//	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}


//void Buzzer_on(u8 time)
//{
//	
//	buzzer_volume = time;
//	TIM2->CR1|=0x01;
//}
