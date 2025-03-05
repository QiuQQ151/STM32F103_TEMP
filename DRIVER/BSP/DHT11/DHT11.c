#include "DHT11.h"
#include "stm32f10x.h"
#include "delay.h"
// RTOS
#include "FreeRTOS.h"
#include "task.h"


/* �û����� */
u8 DHT11_record[5]={0,0,0,0,0};//��¼��ʪ��,�¶�����+�¶�С��+ʪ������+ʪ��С��+1(������Ч����0��Ч��
float TH_RH[3] = {0,0,0};//��¼�¶�-ʪ��-��Ч�ԣ�0��Ч��1��Ч��
/* �ڲ�*/
u8 U8FLAG;  //������
u8 U8temp;
u8 U8DHT11_COMdata;   //�������ݽ���

/* �Զ��� */
//��������0��1��2
u8 DHT11_init(void) 
{
	u8 sta=0;
	RCC->APB2ENR|=1<<2;      //����A��ʱ��
	GPIOA->CRH&=0XFFFFF00F;  //���ؼ��ⲿ̽ͷ
	GPIOA->CRH|=(u32)3<<4;
	GPIOA->ODR|=1<<9;	
	GPIOA->CRH|=(u32)3<<8;
	GPIOA->ODR|=1<<10;	
	return sta;
}

 
 /*  �ڲ�ʹ��  */
u8 DHT11_COM(u8 DHT11_ID)
{
     
	u8 i;
    u8 time_D=1;
    u8 time_U=1;	
   for(i=0;i<8;i++)	   
	 {   
		   if(DHT11_ID==FI_DHT11)  //����
			 {    
				    U8FLAG=2;
						while( (!FI_DHT11_DATE_IN)&&U8FLAG++ )
						{ time_D+=1;delay_us(1);}
						U8FLAG=2;
						while(FI_DHT11_DATE_IN &&U8FLAG++)
						{ time_U+=1;delay_us(1);}	
			 }
		   else if(DHT11_ID==EX_DHT11)  //���
			 {
				    U8FLAG=2;
						while((!EX_DHT11_DATE_IN)&&U8FLAG++ )
						{ time_D+=1;delay_us(1);}
						U8FLAG=2;
						while(EX_DHT11_DATE_IN&&U8FLAG++ )
						{ time_U+=1;delay_us(1);}					 
			 }

			U8DHT11_COMdata<<=1; //��������һλ����ĩβ����
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
	//-----ʪ�ȶ�ȡ�ӳ��� ------------
u8 DHT11_GET_RH(u8 DHT11_ID)
{ 
     u8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp; //��ʪ����ʱ��¼	   
	 DHT11_record[4]=0;	
	TH_RH[2] = 1;
    if(DHT11_ID==EX_DHT11)	
	{
		
	    EX_DHT11_IO_OUT();  //���ģʽ
        EX_DHT11_DATE_OUT(0);//��������18ms
	    delay_ms(18);  
		vTaskSuspendAll();
	    //������Ϊ���� �жϴӻ���Ӧ�ź� 
		EX_DHT11_IO_IN();//����ģʽ
		U8FLAG=2;
		while(EX_DHT11_DATE_IN&&U8FLAG++); //�ȴ�DHT11�͵�ƽ��Ӧ��ʱ�˳�	
		
        if(!EX_DHT11_DATE_IN)	//�͵�ƽ(DHT11��Ӧ��)������һ�����ǵ͵�ƽ������	  
	    {
			U8FLAG=2;
			while((!EX_DHT11_DATE_IN)&&U8FLAG++);//�ȴ��ӻ��͵�ƽ����	
            U8FLAG=2;				
			while(EX_DHT11_DATE_IN&&U8FLAG++);//�ȴ��ӻ��ߵ�ƽ����
			//�������ݽ���
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
						 EX_DHT11_IO_OUT();  //���ģʽ���ָ��ߵ�ƽ
						 EX_DHT11_DATE_OUT(1);		

					   //����У�� 		 
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
xTaskResumeAll();										//�ָ�OS����		
		}
	return 0;
}


u8 DHT11_check(u8 DHT11_ID) //����Ӧ���豸�Ƿ�������0������1�쳣
{   u8 t;
	for(t=0;t<10;t++)
	{
	 DHT11_GET_RH(DHT11_ID);
	if(DHT11_record[4]==1)
   {  return 0x00;}
	}
	return 0x01;
}
