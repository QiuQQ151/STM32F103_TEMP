#include "thermometer_app.h"
//
#include "stm32f10x.h"
#include "stdio.h"
//
#include "dht11.h"
#include "24cxx.h"
#include "buzzer.h"
#include "led.h"
#include "lcd.h"
#include "GUI.h"
//
#include "FreeRTOS.h"
#include "task.h"

/*  dht11����  */
extern u8 DHT11_record[];
extern float TH_RH[3];
/*   LCD����   */



u8 thermometer_app(void)   //��ʪ�Ƚ���
{
	u8 step_change=1;  //1ҳ����ֱ��,0��
	u8 t;	
	u8 temp;
	float* T1; //����ָ��//��¼��һ������// ��������
	float* T2;//����ָ��//��¼���δ�����// ���ڸ���
	u8 * Ta;//���ڼ�¼����λ��
	float*R1;//����ָ��//��¼��һ������
	float*R2;//����ָ��//��¼���δ�����	
	u8* Ra;
	float T_max,T_min,R_max,R_min;//��¼�����Сֵ
	int T_y_max,T_y_min,T_y_max_l,T_y_min_l;  //y�����������С
	int R_y_max,R_y_min,R_y_max_l,R_y_min_l;
	u8 T_y_change=0; //��������ֵ�ı䣬1�ı䣬0����//��ʼֵ0
	u8 R_y_change=0; //��������ֵ�ı䣬1�ı䣬0����//��ʼֵ0
	
     
	//�ڴ�����
	u8 slen = 1;
	u16 display_len = 250/slen; //ȡ����//��250���㣬��x40-x290
	
    T1=(float*)pvPortMalloc(sizeof(float)*(display_len+1) );
	T2=(float*)pvPortMalloc(sizeof(float)*(display_len+1) );
	Ta=(u8*)pvPortMalloc(sizeof(u8)*(display_len+1) );
    R1=(float*)pvPortMalloc(sizeof(float)*(display_len+1) );
	R2=(float*)pvPortMalloc(sizeof(float)*(display_len+1) );	
	Ra=(u8*)pvPortMalloc(sizeof(u8)*(display_len+1) ); 
	
	if(T1==NULL ||T2==NULL || Ta == NULL ||R1==NULL ||R2==NULL || Ra == NULL ) //�ڴ治��
	{
		ShowFontStr(50,100,RED,WHITE,(u8*)"�ڴ治�㣡",24,0);
		vPortFree(T1);
		vPortFree(T2);
		vPortFree(R1);
		vPortFree(R2);	
        delay_ms(1000);		
		return 0; //�˳�
	}

	u8 a = sizeof(float);
    //��ʼ������
	 T_max = T_min = 21;
	 R_max = R_min = 41;
	 T_y_max=22;
	 T_y_max_l=22;
	 T_y_min=20;
	 T_y_min_l=20;
	 R_y_max=22;
	 R_y_max_l=22;
	 R_y_min=18;
	 R_y_min_l=18;							
	 T_y_change=1;
	 R_y_change=1;
	
	for(t=0;t<display_len+1;t++)
	{ 
		 *(T1+t) = 21.0;
		 *(T2+t) = 21.0;
		 *(R1+t) = 20.0;
		 *(R2+t) = 20.0;		
	}	

	
	//����������
    LCD_Clear(WHITE);	
	ShowFontStrButton(3,7,BLACK,WHITE,(u8*)"����",16,RED);
	ShowFontStr(60,7,RED,WHITE,(u8*)"  .  ��",16,1);	
	ShowFontStr(132,7,BLUE,WHITE,(u8*)"  %",16,1);	
	ShowFontStrButton(180,7,BLACK,WHITE,(u8*)"���",16,LIGHTBLUE);
	POINT_COLOR=RED;
	Draw_Line(24,30,24,230); //������
	Draw_Line(20,35,24,30);  
	Draw_Line(28,35,24,30);
	Draw_Line(24,80,30,80);  //�̶�
	Draw_Line(24,130,30,130);
	Draw_Line(24,180,30,180);
	POINT_COLOR=BLUE;
	Draw_Line(304,30,304,230); //������
	Draw_Line(300,35,304,30);
	Draw_Line(308,35,304,30);
	Draw_Line(300,80,304,80); //�̶�
	Draw_Line(300,130,304,130);
	Draw_Line(300,180,304,180);
	POINT_COLOR=BLACK;
	Draw_Line(24,230,304,230); //X��
	for(t=0;t<28;t++)
	{  Draw_Line(30+t*10,230,30+t*10,235); }
	
	 while(1)
	 { 		   
        //buzzer_on;
		DHT11_GET_RH(EX_DHT11);//������ʪ��	
        		 
		if(TH_RH[2]==0)//������Ч
		{
		    Draw_Circle(225,15,GREEN,3,1);
			//������д��
			*(T2+display_len)= TH_RH[0];
			*(R2+display_len)= TH_RH[1];			
			//		
			T_max = *(T2+2);
			T_min = *(T2+2);
			R_max = *(R2+2);
			R_min = *(R2+2);
			for(t=0;t<display_len;t++) //��һ������,//������λ
			{
				//�¶�
				*(T1+t)=*(T2+t);  //���¾ɵ� 
				*(T2+t)=*(T2+t+1);//���εģ���λ����
				//*(Ta+t) = 0;
				if( *(T1+t) != *(T2+t) )
				{		*(Ta+t) = 1;   }
				if( T_max< *(T2+t) )
				{
					T_max = *(T2+t);
				}
				if( T_min> *(T2+t) )
				{
					T_min = *(T2+t);
				}				
				//ʪ��
				*(R1+t)=*(R2+t);  //���¾ɵ�
				*(R2+t)=*(R2+t+1);//���εģ���λ���� 	
				//*(Ra+t) = 0;
				if( *(R1+t) != *(R2+t) )
				{		*(Ra+t) = 1;   }
				if( R_max< *(R2+t) )
				{
					R_max = *(R2+t);
				}
				if( R_min> *(R2+t) )
				{
					R_min = *(R2+t);
				}				
			}
			buzzer_off;				
							
			//---------------------��ͼ---------------------------//								
			//ͷ���¶�ʵʱ��ʾ
			POINT_COLOR=RED;
            // ������
			if( *(T2+(display_len-1) )>= 0 ){
			 ShowFontStr(50,7,RED,WHITE,(u8*)"+",16,1);
			}else{
			 Draw_Fill_Rectangle(50,7,58,23,WHITE);
			 ShowFontStr(50,7,RED,WHITE,(u8*)"-",16,1);
			}				
				
			//
			LCD_ShowNum(60,7,abs( (int)(*(T2+(display_len-1))*10) )/10,2,16);
			LCD_ShowNum(84,7,abs( (int)(*(T2+(display_len-1))*10) )%10,1,16);
			POINT_COLOR=BLUE; 
			LCD_ShowNum(132,7,abs( (int)(*(R2+(display_len-1))*10 ))/10,2,16);
			//LCD_ShowNum(146,7,abs( (int)(*(R2+(display_len-1))*10 ))%10,1,16);

			//��ͼ
					T_y_max = (int)T_max + 1; 
					T_y_min = (int)T_min - 1;		
					R_y_max = (int)R_max + 1;	
			        R_y_min = (int)R_min - 1;			
				for(t=1;t<display_len;t++)
				{
					//�¶�ȫ���ػ�
					if( (230-(float)( *(T1+t-1)-T_y_min_l )/(T_y_max_l-T_y_min_l)*200) != (230-(float)( *(T2+t-1)-T_y_min )/(T_y_max-T_y_min)*200) | 
						(230-(float)( *(T1+t)-T_y_min_l )/(T_y_max_l-T_y_min_l)*200)!=(230-(float)( *(T2+t)-T_y_min )/(T_y_max-T_y_min)*200) )
						{
							POINT_COLOR=WHITE;
							Draw_Line( 40+(t-1)*slen+1,230-(float)( *(T1+t-1)-T_y_min_l )/(T_y_max_l-T_y_min_l)*200,40+t*slen+1,230-(float)( *(T1+t)-T_y_min_l )/(T_y_max_l-T_y_min_l)*200);
							POINT_COLOR=RED;	
							Draw_Line( 40+(t-1)*slen+1,230-(float)( *(T2+t-1)-T_y_min )/(T_y_max-T_y_min)*200,40+t*slen+1,230-(float)( *(T2+t)-T_y_min )/(T_y_max-T_y_min)*200);							
						}
	
					if( (230-(float)( *(R1+t-1)-R_y_min_l )/(R_y_max_l-R_y_min_l)*200) != (230-(float)( *(R2+t-1)-R_y_min )/(R_y_max-R_y_min)*200) | 
						(230-(float)( *(R1+t)-R_y_min_l )/(R_y_max_l-R_y_min_l)*200)!=(230-(float)( *(R2+t)-R_y_min )/(R_y_max-R_y_min)*200) )						
						{
							POINT_COLOR=WHITE;
							Draw_Line( 40+(t-1)*slen+1,230-(float)( *(R1+t-1)-R_y_min_l )/(R_y_max_l-R_y_min_l)*200,40+t*slen+1,230-(float)( *(R1+t)-R_y_min_l )/(R_y_max_l-R_y_min_l)*200);
							POINT_COLOR=BLUE;
							Draw_Line( 40+(t-1)*slen+1,230-(float)( *(R2+t-1)-R_y_min )/(R_y_max-R_y_min)*200,40+t*slen+1,230-(float)( *(R2+t)-R_y_min )/(R_y_max-R_y_min)*200);							
						}

				}
             ShowFontStr(40+(display_len-1)*slen+1, 223-(float)( *(T1+display_len-1)-T_y_min_l )/(T_y_max_l-T_y_min_l)*200, WHITE ,WHITE,(u8*)">",16,1);
			 ShowFontStr(40+(display_len-1)*slen+1, 223-(float)( *(T2+display_len-1)-T_y_min )/(T_y_max-T_y_min)*200, RED ,WHITE,(u8*)">",16,1);
				 
			 ShowFontStr(40+(display_len-1)*slen+1, 223-(float)( *(R1+display_len-1)-R_y_min_l )/(R_y_max_l-R_y_min_l)*200, WHITE ,WHITE,(u8*)">",16,1);
			 ShowFontStr(40+(display_len-1)*slen+1, 223-(float)( *(R2+display_len-1)-R_y_min )/(R_y_max-R_y_min)*200, BLUE ,WHITE,(u8*)">",16,1);
			 T_y_max_l=T_y_max;
			 T_y_min_l=T_y_min;					
		     R_y_max_l=R_y_max;	
			 R_y_min_l=R_y_min;					
						
			//����y�����᷶Χ
			 if( T_y_max != T_y_max_l | T_y_min != T_y_min_l)
			 { T_y_change = 1; }
			 if( R_y_max != R_y_max_l | R_y_min != R_y_min_l)
			 { R_y_change = 1; }

			//��������ֵ���²���ʾ
			if(T_y_change==1)
			{
				POINT_COLOR=RED;
				// ������
				if(T_y_max>0){
				ShowFontStr(0,28,RED,WHITE,(u8*)"*",12,1);
				}else{
				ShowFontStr(0,28,BLUE,WHITE,(u8*)"*",12,1);
				}	
				if(T_y_min>0){
				ShowFontStr(0,220,RED,WHITE,(u8*)"*",12,1);
				}else{
				ShowFontStr(0,220,BLUE,WHITE,(u8*)"*",12,1);
				}				
				// ��ֵ
				LCD_ShowNum(6,28,abs(T_y_max),2,12); 
				LCD_ShowNum(6,220,abs(T_y_min),2,12); 						
			}
			if(R_y_change==1)
			{
				POINT_COLOR=BLUE;
				//
				
				//
				LCD_ShowNum(308,28,abs(R_y_max),2,12); 
				LCD_ShowNum(308,220,abs(R_y_min),2,12); 											
			}						
				}
				else  //��Ч���ݣ�������
				{  // buzzer_arl
					Draw_Circle(225,15,RED,3,1);
					delay_ms(50);
					
					buzzer_off;
				}
		}
      // �ڴ��ͷ�
		vPortFree(T1);
		vPortFree(T2);
		vPortFree(R1);
		vPortFree(R2);
	
}
