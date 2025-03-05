#include "mytask.h"
// MYDRIVE
#include "adc.h"
#include "delay.h"
// BSP
#include "led.h"
#include "lcd.h"
#include "GUI.h"
#include "buzzer.h"
#include "DHT11.h"
#include "sram.h"
#include "24cxx.h"
// RTOS
#include "FreeRTOS.h"
#include "task.h"
// APP
#include "thermometer_app.h"
#include "test.h"


extern u8 DHT11_record[5];


void Mytask_Init(void)
{	

	// 任务句柄
	TaskHandle_t app_task1_handle = NULL;
    TaskHandle_t app_task2_handle = NULL;
    TaskHandle_t app_task3_handle = NULL;
    TaskHandle_t app_task4_handle = NULL;
    //创建任务
	xTaskCreate(  (TaskFunction_t)App_Task1,          //任务入口函数
	              (const char *)"system_init",          //任务名字
				  (uint16_t)configMINIMAL_STACK_SIZE, //任务栈大小
				  (void*)app_task1_handle,            //任务入口函数参数
				  (UBaseType_t)2,                     //任务优先级
				  (TaskHandle_t*)&app_task1_handle);  //任务句柄
	//创建任务
	xTaskCreate(  (TaskFunction_t)App_Task2,          //任务入口函数
	              (const char *)"App_Task2",          //任务名字
				  (uint16_t)configMINIMAL_STACK_SIZE, //任务栈大小
				  (void*)NULL,                        //任务入口函数参数
				  (UBaseType_t)2,                     //任务优先级
				  (TaskHandle_t*)&app_task2_handle);  //任务句柄
//    //创建任务
//	xTaskCreate(  (TaskFunction_t)App_Task3,          //任务入口函数
//	              (const char *)"App_Task3",          //任务名字
//				  (uint16_t)configMINIMAL_STACK_SIZE, //任务栈大小
//				  (void*)NULL,                        //任务入口函数参数
//				  (UBaseType_t)2,                     //任务优先级
//				  (TaskHandle_t*)&app_task3_handle);  //任务句柄
//	//创建任务
//	xTaskCreate(  (TaskFunction_t)App_Task4,          //任务入口函数
//	              (const char *)"App_Task4",          //任务名字
//				  (uint16_t)configMINIMAL_STACK_SIZE, //任务栈大小
//				  (void*)NULL,                        //任务入口函数参数
//				  (UBaseType_t)1,                     //任务优先级
//				  (TaskHandle_t*)&app_task4_handle);  //任务句柄
				 
	
}
		
/* 

硬件初始化

*/				  			  
void App_Task1(void *pvParameters)
{
	// 初始化硬件/驱动
	LED_Init();
	LCD_Init();
	AT24CXX_Init();
	Buzzer_Init(899,60);
	Adc_Init();	
	DHT11_init();  	
   for(;;)
  {
     thermometer_app();
  }
}
 
/* 
温湿度计
DHT11应用

*/
void App_Task2(void *pvParameters)
{

    for(;;)
	{
      
	}
}

void App_Task3(void *pvParameters)
{
  for(;;)
	{
//	 //buzzer_on;	
//     DHT11_GET_RH(EX_DHT11);
//	 //buzzer_off;
//	 LCD_ShowNum(16,16,DHT11_record[0],4,12);
//	 LCD_ShowNum(50,16,DHT11_record[1],4,12);
//     LCD_ShowNum(100,16,DHT11_record[2],4,12);
//     LCD_ShowNum(150,16,DHT11_record[3],4,12);
//     		
////     if( DHT11_record[4] == 0)
////	 { buzzer_arl;vTaskDelay(100);buzzer_off;}
//      vTaskDelay(1000);
     
	 
	}
}

void App_Task4(void *pvParameters)
{
   for(;;)
	{
//    main_test(); 		//测试主界面
 //  Test_Color();  		//简单刷屏填充测试
//	Test_FillRec();		//GUI矩形绘图测试
//	Test_Circle(); 		//GUI画圆测试
//	Test_Triangle();    //GUI三角形绘图测试
//	English_Font_test();//英文字体示例测试
//	Chinese_Font_test();//中文字体示例测试
//	Pic_test();			//图片显示示例测试
//    Rotate_Test();   //旋转显示测试
	}
}
 



