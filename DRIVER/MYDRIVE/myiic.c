#include "myiic.h"
#include "delay.h"

//��ʼ��IIC
void IIC_Init(void)
{					     
 	RCC->APB2ENR|=1<<3;//��ʹ������IO PORTBʱ�� 							 
	GPIOB->CRH&=0XFFFFFF00;//PB8/9 �������
	GPIOB->CRH|=0X00000033;	   
	GPIOB->ODR|=3<<0;     //�����
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA(1);	  	  
	IIC_SCL(1);
	delay_us(4);
 	IIC_SDA(0);//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL(0);//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL(0);
	IIC_SDA(0);
 	delay_us(4); 
	IIC_SCL(1);//STOP:when CLK is high DATA change form low to high
 	delay_us(4); 
	IIC_SDA(1);//����I2C���߽����ź� 						   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL(0);//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA( (txd&0x80)>>7 );
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL(1);
		delay_us(2); 
		IIC_SCL(0);	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}


//ָ����ַ����һ������
//DeviceAddr:�豸��ַ
//ReadAddr:��ʼ�����ĵ�ַ(u8��Χ�ڵ�ַ)
//����ֵ  :����������
u8 ReadOneByte(u8 DeviceAddr,u8 ReadAddr)
{				  
	  u8 temp=0;		  	    																 
    IIC_Start();  
		IIC_Send_Byte(DeviceAddr|0X00);	 //����д����
		IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr);     //�������ݵ�ַ		   
	  IIC_Wait_Ack();	
    IIC_Start();
		IIC_Send_Byte(DeviceAddr|0X01);	 //���Ͷ�����
		IIC_Wait_Ack();  	
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	  return temp;
}

//ָ����ַд��һ������
//DeviceAddr:д�����ݵ��豸��ַ��u8�ڣ� 
//WriteAddr:д���ַ
//DataToWrite:Ҫд�������
void WriteOneByte(u8 DeviceAddr,u8 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
		IIC_Send_Byte(DeviceAddr|0X00);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr);//���͵�ַ	  
	  IIC_Wait_Ack();		
	  IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	  IIC_Wait_Ack();  		    	   
    IIC_Stop();		//����һ��ֹͣ���� 
	  delay_ms(10);	//����EEPROM������ÿдһ��Ҫ�ȴ�һ��ʱ�䣬����дʧ�ܣ�	 
}

























