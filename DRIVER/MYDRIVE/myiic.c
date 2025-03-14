#include "myiic.h"
#include "delay.h"

//初始化IIC
void IIC_Init(void)
{					     
 	RCC->APB2ENR|=1<<3;//先使能外设IO PORTB时钟 							 
	GPIOB->CRH&=0XFFFFFF00;//PB8/9 推挽输出
	GPIOB->CRH|=0X00000033;	   
	GPIOB->ODR|=3<<0;     //输出高
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA(1);	  	  
	IIC_SCL(1);
	delay_us(4);
 	IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL(0);//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL(0);
	IIC_SDA(0);
 	delay_us(4); 
	IIC_SCL(1);//STOP:when CLK is high DATA change form low to high
 	delay_us(4); 
	IIC_SDA(1);//发送I2C总线结束信号 						   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA(1);delay_us(1);	   
	IIC_SCL(1);delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(0);
	delay_us(2);
	IIC_SCL(1);
	delay_us(2);
	IIC_SCL(0);
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(1);
	delay_us(2);
	IIC_SCL(1);
	delay_us(2);
	IIC_SCL(0);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL(0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA( (txd&0x80)>>7 );
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL(1);
		delay_us(2); 
		IIC_SCL(0);	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL(0); 
        delay_us(2);
		   IIC_SCL(1);
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}


//指定地址读出一个数据
//DeviceAddr:设备地址
//ReadAddr:开始读数的地址(u8范围内地址)
//返回值  :读到的数据
u8 ReadOneByte(u8 DeviceAddr,u8 ReadAddr)
{				  
	  u8 temp=0;		  	    																 
    IIC_Start();  
		IIC_Send_Byte(DeviceAddr|0X00);	 //发送写命令
		IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr);     //发送数据地址		   
	  IIC_Wait_Ack();	
    IIC_Start();
		IIC_Send_Byte(DeviceAddr|0X01);	 //发送读命令
		IIC_Wait_Ack();  	
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	  return temp;
}

//指定地址写入一个数据
//DeviceAddr:写入数据的设备地址（u8内） 
//WriteAddr:写入地址
//DataToWrite:要写入的数据
void WriteOneByte(u8 DeviceAddr,u8 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
		IIC_Send_Byte(DeviceAddr|0X00);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr);//发送地址	  
	  IIC_Wait_Ack();		
	  IIC_Send_Byte(DataToWrite);     //发送字节							   
	  IIC_Wait_Ack();  		    	   
    IIC_Stop();		//产生一个停止条件 
	  delay_ms(10);	//对于EEPROM器件，每写一次要等待一段时间，否则写失败！	 
}

























