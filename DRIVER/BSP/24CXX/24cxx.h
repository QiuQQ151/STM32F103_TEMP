#ifndef __24CXX_H
#define __24CXX_H
#include "myiic.h"   

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
#define AT24C512	65535  

//Mini STM32开发板使用的是24c02，所以定义EE_TYPE为AT24C02
#define EE_TYPE  AT24C256
#define EEPROM1  0XA0  //存数据用
#define EEPROM2  0XA2  //

void AT24CXX_Init(void); //初始化

//单个8bit操作					  
u8   AT24CXX_ReadOneByte(u8 Epprom_Addr,u16 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(u8 Epprom_Addr,u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节

//8bit数组操作
void AT24CXX_Write(u8 Epprom_Addr,u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(u8 Epprom_Addr,u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据

//(16bit操作,相当于2*8bit)
u16 AT24CXX_Read16bit(u8 Epprom_Addr,u16 ReadAddr);    //返回short型
void AT24CXX_Write16bit(u8 Epprom_Addr,u16 WriteAddr,u32 DataToWrite);

//(32bit操作,相当于4*8bit)
u32 AT24CXX_Read32bit(u8 Epprom_Addr,u16 ReadAddr);
void AT24CXX_Write32bit(u8 Epprom_Addr,u16 WriteAddr,u32 DataToWrite);


u8 AT24CXX_Check(u8 Epprom_Addr,u16 Epprom_type);  //检查器件
void AT24CXX_Init(void); //初始化IIC
#endif
















