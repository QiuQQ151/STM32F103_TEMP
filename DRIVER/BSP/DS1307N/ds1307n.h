#ifndef __DS1307N_H
#define __DS1307N_H

#include "sys.h"

#define DS1307N     0XD0  //时钟芯片地址
#define year_addr   0X06
#define month_addr  0X05
#define date_addr   0X04
#define day_addr    0X03
#define hour_addr   0X02
#define minute_addr 0X01
#define second_addr 0X00



u8 DS1307N_init(void);
void DS1307N_write_time(u8 year,u8 month,u8 date,u8 day,u8 hour,u8 minute,u8 second); //设置时间
void DS1307N_read_time(void);  //读取时间BCD码






#endif 
