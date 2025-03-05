#include "ds1307n.h"
#include "myiic.h"


//---------时间----------//
u8 time[7]; //年，月，日，星期，时，分，秒



//返回值0：时钟在运行，获取最新时间
//返回值1：时钟停止运行，设置24h制，使能运行
u8 DS1307N_init()
{
  u8 temp=0;
	temp=ReadOneByte(DS1307N,second_addr);
	if(temp&=0X80==1)
	{  
		 WriteOneByte(DS1307N,hour_addr,0);//初始化为24h制
	   WriteOneByte(DS1307N,second_addr,1<<7);//使能时钟	
	   WriteOneByte(DS1307N,second_addr,0);//使能时钟	
		 return 1;
	}
	else
	{
		DS1307N_read_time();
		return 0;
	}
}

void DS1307N_write_time(u8 year,u8 month,u8 date,u8 day,u8 hour,u8 minute,u8 second) //设置时间
{
	  u8 temp1=0;  //编码辅助
	  u8 temp2=0;
	  temp1=year/10;
	  temp2=year%10;
  	WriteOneByte(DS1307N,year_addr,(temp1<<4)+temp2); //写入年份
    
	  temp1=month/10;
	  temp2=month%10;
  	WriteOneByte(DS1307N,month_addr,month); //写入月份	  
    
	  temp1=date/10;
	  temp2=date%10;
  	WriteOneByte(DS1307N,date_addr,(temp1<<4)+temp2); //写入日期

  	WriteOneByte(DS1307N,day_addr,day); //写入星期	
	
		temp1=hour/10;
	  temp2=hour%10;
  	WriteOneByte(DS1307N,hour_addr,(temp1<<4)+temp2); //写入小时
	
	  temp1=minute/10;
	  temp2=minute%10;
  	WriteOneByte(DS1307N,minute_addr,(temp1<<4)+temp2); //写入分钟
		
		temp1=second/10;
	  temp2=second%10;
  	WriteOneByte(DS1307N,second_addr,(temp1<<4)+temp2); //写入秒
	
}
void DS1307N_read_time()  //读取时间BCD码
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