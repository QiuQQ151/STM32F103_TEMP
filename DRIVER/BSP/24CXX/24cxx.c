#include "24cxx.h" 
#include "delay.h" 
#include "led.h"
#include "myiic.h"

//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
	IIC_Init();
}

//��AT24CXXָ����ַ����һ������
//Epprom_Addr:epprom��ַ
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u8 Epprom_Addr,u16 ReadAddr)
{				  
	u8 temp=0;
    LED_off;
    IIC_Start();  
	  if(EE_TYPE>AT24C16)
	  {
		    IIC_Send_Byte(Epprom_Addr|0X00);	   //����д����
		    IIC_Wait_Ack();
		    IIC_Send_Byte(ReadAddr>>8);//�������ݸߵ�ַ	    
	   }
		else 
		{  
			IIC_Send_Byte(Epprom_Addr|0X00+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	   
		}
		
		IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);     //�������ݵ͵�ַ
	  IIC_Wait_Ack();	    
	  IIC_Start();  	 	   
	  IIC_Send_Byte(Epprom_Addr|0X01); //��ȡ����������ģʽ			   
	  IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	  
    LED_on;		
	return temp;
}

//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u8 Epprom_Addr,u16 WriteAddr,u8 DataToWrite)
{			
   LED_off;
   IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(Epprom_Addr|0X00);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
	}
	else 
	{ 
		IIC_Send_Byte(Epprom_Addr|0X00+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 	 
	}
		
	  IIC_Wait_Ack();	   
      IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	  IIC_Wait_Ack(); 	 										  		   
	  IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	  IIC_Wait_Ack();  		    	   
      IIC_Stop();		//����һ��ֹͣ���� 
	  delay_ms(10);	//����EEPROM������ÿдһ��Ҫ�ȴ�һ��ʱ�䣬����дʧ�ܣ�	
      LED_on;	
}


//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��0x55,ͨ����ȡ�ж��Ƿ���ȷ�洢
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(u8 Epprom_Addr,u16 Epprom_type)
{
	 u8 temp;
	 temp=AT24CXX_ReadOneByte(Epprom_Addr,Epprom_type);//����ÿ�ο�����дAT24CXX	
	
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		  AT24CXX_WriteOneByte(Epprom_Addr,Epprom_type,0X55);    //д
	    temp=AT24CXX_ReadOneByte(Epprom_Addr,Epprom_type);	   //�ٶ������ж�
		  if(temp==0X55)   return 0;
	}
	return 1;											  
}


//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u8 Epprom_Addr,u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(Epprom_Addr,ReadAddr++);	
		NumToRead--;
	}
}

//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ 
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u8 Epprom_Addr,u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(Epprom_Addr,WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

//��ȡ16bit(2*8bit)
//�ȵ�λ���λ
u16 AT24CXX_Read16bit(u8 Epprom_Addr,u16 ReadAddr)
{
  u8 t;
	u16 temp=0;
	for(t=0;t<2;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(Epprom_Addr,ReadAddr+1-t); 	 				   
	}
	return temp;	
}

//д��16bit(2*8bit)
//�ȵ�λ���λ
void AT24CXX_Write16bit(u8 Epprom_Addr,u16 WriteAddr,u32 DataToWrite)
{
	u8 t;
	for(t=0;t<2;t++)
	{
		AT24CXX_WriteOneByte(Epprom_Addr,WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}
}


//��ȡ32bit(4*8bit)
//�ȵ�λ���λ
u32 AT24CXX_Read32bit(u8 Epprom_Addr,u16 ReadAddr)
{
  u8 t;
	u32 temp=0;
	for(t=0;t<4;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(Epprom_Addr,ReadAddr+3-t); 	 				   
	}
	return temp;
	
}

//д��32bit(4*8bit)
//�ȵ�λ���λ
void AT24CXX_Write32bit(u8 Epprom_Addr,u16 WriteAddr,u32 DataToWrite)
{
	u8 t;
	for(t=0;t<4;t++)
	{
		AT24CXX_WriteOneByte(Epprom_Addr,WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}
}





