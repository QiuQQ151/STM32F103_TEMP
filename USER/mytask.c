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

	// ������
	TaskHandle_t app_task1_handle = NULL;
    TaskHandle_t app_task2_handle = NULL;
    TaskHandle_t app_task3_handle = NULL;
    TaskHandle_t app_task4_handle = NULL;
    //��������
	xTaskCreate(  (TaskFunction_t)App_Task1,          //������ں���
	              (const char *)"system_init",          //��������
				  (uint16_t)configMINIMAL_STACK_SIZE, //����ջ��С
				  (void*)app_task1_handle,            //������ں�������
				  (UBaseType_t)2,                     //�������ȼ�
				  (TaskHandle_t*)&app_task1_handle);  //������
	//��������
	xTaskCreate(  (TaskFunction_t)App_Task2,          //������ں���
	              (const char *)"App_Task2",          //��������
				  (uint16_t)configMINIMAL_STACK_SIZE, //����ջ��С
				  (void*)NULL,                        //������ں�������
				  (UBaseType_t)2,                     //�������ȼ�
				  (TaskHandle_t*)&app_task2_handle);  //������
//    //��������
//	xTaskCreate(  (TaskFunction_t)App_Task3,          //������ں���
//	              (const char *)"App_Task3",          //��������
//				  (uint16_t)configMINIMAL_STACK_SIZE, //����ջ��С
//				  (void*)NULL,                        //������ں�������
//				  (UBaseType_t)2,                     //�������ȼ�
//				  (TaskHandle_t*)&app_task3_handle);  //������
//	//��������
//	xTaskCreate(  (TaskFunction_t)App_Task4,          //������ں���
//	              (const char *)"App_Task4",          //��������
//				  (uint16_t)configMINIMAL_STACK_SIZE, //����ջ��С
//				  (void*)NULL,                        //������ں�������
//				  (UBaseType_t)1,                     //�������ȼ�
//				  (TaskHandle_t*)&app_task4_handle);  //������
				 
	
}
		
/* 

Ӳ����ʼ��

*/				  			  
void App_Task1(void *pvParameters)
{
	// ��ʼ��Ӳ��/����
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
��ʪ�ȼ�
DHT11Ӧ��

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
//    main_test(); 		//����������
 //  Test_Color();  		//��ˢ��������
//	Test_FillRec();		//GUI���λ�ͼ����
//	Test_Circle(); 		//GUI��Բ����
//	Test_Triangle();    //GUI�����λ�ͼ����
//	English_Font_test();//Ӣ������ʾ������
//	Chinese_Font_test();//��������ʾ������
//	Pic_test();			//ͼƬ��ʾʾ������
//    Rotate_Test();   //��ת��ʾ����
	}
}
 



