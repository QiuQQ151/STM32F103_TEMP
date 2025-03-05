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

//Mini STM32������ʹ�õ���24c02�����Զ���EE_TYPEΪAT24C02
#define EE_TYPE  AT24C256
#define EEPROM1  0XA0  //��������
#define EEPROM2  0XA2  //

void AT24CXX_Init(void); //��ʼ��

//����8bit����					  
u8   AT24CXX_ReadOneByte(u8 Epprom_Addr,u16 ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void AT24CXX_WriteOneByte(u8 Epprom_Addr,u16 WriteAddr,u8 DataToWrite);		//ָ����ַд��һ���ֽ�

//8bit�������
void AT24CXX_Write(u8 Epprom_Addr,u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_Read(u8 Epprom_Addr,u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

//(16bit����,�൱��2*8bit)
u16 AT24CXX_Read16bit(u8 Epprom_Addr,u16 ReadAddr);    //����short��
void AT24CXX_Write16bit(u8 Epprom_Addr,u16 WriteAddr,u32 DataToWrite);

//(32bit����,�൱��4*8bit)
u32 AT24CXX_Read32bit(u8 Epprom_Addr,u16 ReadAddr);
void AT24CXX_Write32bit(u8 Epprom_Addr,u16 WriteAddr,u32 DataToWrite);


u8 AT24CXX_Check(u8 Epprom_Addr,u16 Epprom_type);  //�������
void AT24CXX_Init(void); //��ʼ��IIC
#endif
















