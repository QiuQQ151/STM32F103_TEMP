#include "ds1307n.h"
#include "myiic.h"


//---------ʱ��----------//
u8 time[7]; //�꣬�£��գ����ڣ�ʱ���֣���



//����ֵ0��ʱ�������У���ȡ����ʱ��
//����ֵ1��ʱ��ֹͣ���У�����24h�ƣ�ʹ������
u8 DS1307N_init()
{
  u8 temp=0;
	temp=ReadOneByte(DS1307N,second_addr);
	if(temp&=0X80==1)
	{  
		 WriteOneByte(DS1307N,hour_addr,0);//��ʼ��Ϊ24h��
	   WriteOneByte(DS1307N,second_addr,1<<7);//ʹ��ʱ��	
	   WriteOneByte(DS1307N,second_addr,0);//ʹ��ʱ��	
		 return 1;
	}
	else
	{
		DS1307N_read_time();
		return 0;
	}
}

void DS1307N_write_time(u8 year,u8 month,u8 date,u8 day,u8 hour,u8 minute,u8 second) //����ʱ��
{
	  u8 temp1=0;  //���븨��
	  u8 temp2=0;
	  temp1=year/10;
	  temp2=year%10;
  	WriteOneByte(DS1307N,year_addr,(temp1<<4)+temp2); //д�����
    
	  temp1=month/10;
	  temp2=month%10;
  	WriteOneByte(DS1307N,month_addr,month); //д���·�	  
    
	  temp1=date/10;
	  temp2=date%10;
  	WriteOneByte(DS1307N,date_addr,(temp1<<4)+temp2); //д������

  	WriteOneByte(DS1307N,day_addr,day); //д������	
	
		temp1=hour/10;
	  temp2=hour%10;
  	WriteOneByte(DS1307N,hour_addr,(temp1<<4)+temp2); //д��Сʱ
	
	  temp1=minute/10;
	  temp2=minute%10;
  	WriteOneByte(DS1307N,minute_addr,(temp1<<4)+temp2); //д�����
		
		temp1=second/10;
	  temp2=second%10;
  	WriteOneByte(DS1307N,second_addr,(temp1<<4)+temp2); //д����
	
}
void DS1307N_read_time()  //��ȡʱ��BCD��
{
	  u8 temp1=0;
	  u8 temp2=0;
	  
	  temp1=ReadOneByte(DS1307N,year_addr);//year
    time[0]=(temp1>>4)*10+(temp1&0X0F);
	
    temp1=ReadOneByte(DS1307N,month_addr);  //month
    time[1]=(temp1>>4)*10+(temp1&0X0F);
	
    temp1=ReadOneByte(DS1307N,date_addr);  //date
    time[2]=(temp1>>4)*10+(temp1&0X0F);	  
	
    temp1=ReadOneByte(DS1307N,day_addr);   //day
    time[3]=temp1;
	
    temp1=ReadOneByte(DS1307N,hour_addr);
    time[4]=((temp1&0X30)>>4)*10+(temp1&0X0F);	 

    temp1=ReadOneByte(DS1307N,minute_addr);
    time[5]=(temp1>>4)*10+(temp1&0X0F);

    temp1=ReadOneByte(DS1307N,second_addr);
    time[6]=(temp1>>4)*10+(temp1&0X0F);


 }