#include "DHT11.h"
#include "stm32f10x.h"
#include "delay.h"
// RTOS
#include "FreeRTOS.h"
#include "task.h"


/* 用户数据 */
u8 DHT11_record[5]={0,0,0,0,0};//记录温湿度,温度整数+温度小数+湿度整数+湿度小数+1(数据有效）（0无效）
float TH_RH[3] = {0,0,0};//记录温度-湿度-有效性（0有效，1无效）
/* 内部*/
u8 U8FLAG;  //防卡死
u8 U8temp;
u8 U8DHT11_COMdata;   //总线数据接收

/* 自定义 */
//返回正常0，1，2
u8 DHT11_init(void) 
{
	u8 sta=0;
	RCC->APB2ENR|=1<<2;      //开启A口时钟
	GPIOA->CRH&=0XFFFFF00F;  //板载加外部探头
	GPIOA->CRH|=(u32)3<<4;
	GPIOA->ODR|=1<<9;	
	GPIOA->CRH|=(u32)3<<8;
	GPIOA->ODR|=1<<10;	
	return sta;
}

 
 /*  内部使用  */
u8 DHT11_COM(u8 DHT11_ID)
{
     
	u8 i;
    u8 time_D=1;
    u8 time_U=1;	
   for(i=0;i<8;i++)	   
	 {   
		   if(DHT11_ID==FI_DHT11)  //板载
			 {    
				    U8FLAG=2;
						while( (!FI_DHT11_DATE_IN)&&U8FLAG++ )
						{ time_D+=1;delay_us(1);}
						U8FLAG=2;
						while(FI_DHT11_DATE_IN &&U8FLAG++)
						{ time_U+=1;delay_us(1);}	
			 }
		   else if(DHT11_ID==EX_DHT11)  //外接
			 {
				    U8FLAG=2;
						while((!EX_DHT11_DATE_IN)&&U8FLAG++ )
						{ time_D+=1;delay_us(1);}
						U8FLAG=2;
						while(EX_DHT11_DATE_IN&&U8FLAG++ )
						{ time_U+=1;delay_us(1);}					 
			 }

			U8DHT11_COMdata<<=1; //数据左移一位，再末尾插入
			if(time_D>=time_U) //0 
			{ U8DHT11_COMdata|=0;}
			else
			{ U8DHT11_COMdata|=1;}	 
			time_D=1;
			time_U=1;
	 }
	return 0;   
}

	
	
	//--------------------------------
	//-----湿度读取子程序 ------------
u8 DHT11_GET_RH(u8 DHT11_ID)
{ 
     u8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp; //温湿度临时记录	   
	 DHT11_record[4]=0;	
	TH_RH[2] = 1;
    if(DHT11_ID==EX_DHT11)	
	{
		
	    EX_DHT11_IO_OUT();  //输出模式
        EX_DHT11_DATE_OUT(0);//主机拉低18ms
	    delay_ms(18);  
		vTaskSuspendAll();
	    //主机设为输入 判断从机响应信号 
		EX_DHT11_IO_IN();//输入模式
		U8FLAG=2;
		while(EX_DHT11_DATE_IN&&U8FLAG++); //等待DHT11低电平响应或超时退出	
		
        if(!EX_DHT11_DATE_IN)	//低电平(DHT11响应后)进入下一步，非低电平不进入	  
	    {
			U8FLAG=2;
			while((!EX_DHT11_DATE_IN)&&U8FLAG++);//等待从机低电平结束	
            U8FLAG=2;				
			while(EX_DHT11_DATE_IN&&U8FLAG++);//等待从机高电平结束
			//进入数据接收
			 			 DHT11_COM(DHT11_ID);
						 U8RH_data_H_temp=U8DHT11_COMdata;
						 DHT11_COM(DHT11_ID);
						 U8RH_data_L_temp=U8DHT11_COMdata;
						 DHT11_COM(DHT11_ID);
						 U8T_data_H_temp=U8DHT11_COMdata;
						 DHT11_COM(DHT11_ID);
						 U8T_data_L_temp=U8DHT11_COMdata;
						 DHT11_COM(DHT11_ID);
						 U8checkdata_temp=U8DHT11_COMdata;	 
						 EX_DHT11_IO_OUT();  //输出模式，恢复高电平
						 EX_DHT11_DATE_OUT(1);		

					   //数据校验 		 
						 U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
						 if(U8temp==U8checkdata_temp)
						 {  
								DHT11_record[2]=U8RH_data_H_temp;
								DHT11_record[3]=U8RH_data_L_temp;
							    DHT11_record[0]=U8T_data_H_temp;
								DHT11_record[1]=U8T_data_L_temp;
                                DHT11_record[4]=1;	

							 TH_RH[0] = ( (float)U8T_data_H_temp + (float)(U8T_data_L_temp&0x7f)/10)*( U8T_data_L_temp&0x80 ? -1 : 1);
							 TH_RH[1] = (float)U8RH_data_H_temp + (float)U8RH_data_L_temp/10;
							 TH_RH[2] = 0;
						 }		 
						 else 
                         {  
							 DHT11_record[4]=0;	
						     TH_RH[2] = 1;
						 }						 		 
		  }	
xTaskResumeAll();										//恢复OS调度		
		}
	return 0;
}


u8 DHT11_check(u8 DHT11_ID) //检查对应的设备是否正常，0正常，1异常
{   u8 t;
	for(t=0;t<10;t++)
	{
	 DHT11_GET_RH(DHT11_ID);
	if(DHT11_record[4]==1)
   {  return 0x00;}
	}
	return 0x01;
}
