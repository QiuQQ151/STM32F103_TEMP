// CMSIS/FWLIB
#include "stm32f10x.h"
// MYDRIVE
#include "delay.h"
// RTOS
#include "FreeRTOS.h"
#include "task.h"
// mytask
#include "mytask.h"





 
int main(void)
{   // ����ϵͳ��ʱ�� 72
	SysTick_Init(72);	
	//��ȫ�����ȼ���Ϊ��ռ���ȼ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	// ��ʼ�������б�	
    Mytask_Init();	
	//�������������						
	vTaskStartScheduler();
}
 
 

