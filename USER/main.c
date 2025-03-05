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
{   // 启动系统计时器 72
	SysTick_Init(72);	
	//将全部优先级设为抢占优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	// 初始化任务列表	
    Mytask_Init();	
	//开启任务调度器						
	vTaskStartScheduler();
}
 
 

