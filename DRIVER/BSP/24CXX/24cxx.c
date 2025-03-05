#include "24cxx.h" 
#include "delay.h" 
#include "led.h"
#include "myiic.h"

//初始化IIC接口
void AT24CXX_Init(void)
{
	IIC_Init();
}

//在AT24CXX指定地址读出一个数据
//Epprom_Addr:epprom地址
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 AT24CXX_ReadOneByte(u8 Epprom_Addr,u16 ReadAddr)
{				  
	u8 temp=0;
    LED_off;
    IIC_Start();  
	  if(EE_TYPE>AT24C16)
	  {
		    IIC_Send_Byte(Epprom_Addr|0X00);	   //发送写命令
		    IIC_Wait_Ack();
		    IIC_Send_Byte(ReadAddr>>8);//发送数据高地址	    
	   }
		else 
		{  
			IIC_Send_Byte(Epprom_Addr|0X00+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	   
		}
		
		IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);     //发送数据低地址
	  IIC_Wait_Ack();	    
	  IIC_Start();  	 	   
	  IIC_Send_Byte(Epprom_Addr|0X01); //读取命令，进入接收模式			   
	  IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	  
    LED_on;		
	return temp;
}

//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u8 Epprom_Addr,u16 WriteAddr,u8 DataToWrite)
{			
   LED_off;
   IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(Epprom_Addr|0X00);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
	}
	else 
	{ 
		IIC_Send_Byte(Epprom_Addr|0X00+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 	 
	}
		
	  IIC_Wait_Ack();	   
      IIC_Send_Byte(WriteAddr%256);   //发送低地址
	  IIC_Wait_Ack(); 	 										  		   
	  IIC_Send_Byte(DataToWrite);     //发送字节							   
	  IIC_Wait_Ack();  		    	   
      IIC_Stop();		//产生一个停止条件 
	  delay_ms(10);	//对于EEPROM器件，每写一次要等待一段时间，否则写失败！	
      LED_on;	
}


//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字0x55,通过读取判断是否正确存储
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
u8 AT24CXX_Check(u8 Epprom_Addr,u16 Epprom_type)
{
	 u8 temp;
	 temp=AT24CXX_ReadOneByte(Epprom_Addr,Epprom_type);//避免每次开机都写AT24CXX	
	
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		  AT24CXX_WriteOneByte(Epprom_Addr,Epprom_type,0X55);    //写
	    temp=AT24CXX_ReadOneByte(Epprom_Addr,Epprom_type);	   //再读出，判断
		  if(temp==0X55)   return 0;
	}
	return 1;											  
}


//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(u8 Epprom_Addr,u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(Epprom_Addr,ReadAddr++);	
		NumToRead--;
	}
}

//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(u8 Epprom_Addr,u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(Epprom_Addr,WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

//读取16bit(2*8bit)
//先低位后高位
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

//写入16bit(2*8bit)
//先低位后高位
void AT24CXX_Write16bit(u8 Epprom_Addr,u16 WriteAddr,u32 DataToWrite)
{
	u8 t;
	for(t=0;t<2;t++)
	{
		AT24CXX_WriteOneByte(Epprom_Addr,WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}
}


//读取32bit(4*8bit)
//先低位后高位
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

//写入32bit(4*8bit)
//先低位后高位
void AT24CXX_Write32bit(u8 Epprom_Addr,u16 WriteAddr,u32 DataToWrite)
{
	u8 t;
	for(t=0;t<4;t++)
	{
		AT24CXX_WriteOneByte(Epprom_Addr,WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}
}





