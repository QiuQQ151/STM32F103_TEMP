/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

#include "mmc_sd.h"
#include "flash.h"
// RTOS
#include "FreeRTOS.h"
#include "task.h"

//定义设备号//两个设备
#define SD_CARD	 0  //SD卡,卷标为0
#define EX_FLASH 1	//外部flash,卷标为1

#define FLASH_SECTOR_SIZE 	512	  //512字节一个扇区	  		 			    
u16	    FLASH_SECTOR_COUNT= 1024*16;//16384;	  //W25Q16q128共16M，前8M用于fafts，后2M用户使用，后6M存放字库
#define FLASH_BLOCK_SIZE   	8     	//每个BLOCK有16个扇区

/* Get Drive Status */
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
//	DSTATUS stat;
//	int result;

//	switch (pdrv) {
//	case SD_CARD :
//		result = RAM_disk_status();

//		// translate the reslut code here

//		return stat;

//	case EX_FLASH :
//		result = MMC_disk_status();

//		// translate the reslut code here

//		return stat;

//	case DEV_USB :
//		result = USB_disk_status();

//		// translate the reslut code here

//		return stat;
//	}
	return 0;
}




//初始化设备
//back 0: ok
//back 1: err
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;
	switch (pdrv) {
	case SD_CARD :
		              result = SD_Initialize();  //初始化成功则返回0
									if(result)//STM32 SPI的bug,在sd卡操作失败的时候如果不执行下面的语句,可能导致SPI读写异常
									{
										SD_SPI_SpeedLow();
										SD_SPI_ReadWriteByte(0xff);//提供额外的8个时钟
										SD_SPI_SpeedHigh();
									}
		              break;
                  
	case EX_FLASH :
		              result = SPI_Flash_Init();
                  break;
	}
	if(result)
	{ return STA_NOINIT; } //初始化失败
	else 
	{ return 0; }
}



//读扇区
//pdrv:卷标 buff:读缓冲首地址 sector:开始扇区地址 count:扇区数
DRESULT disk_read 
(
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	u8 res;
	if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误	
	switch (pdrv) 
	{
	    case SD_CARD :  res=SD_ReadDisk(buff,sector,count);
                      if(res)//STM32 SPI的bug,在sd卡操作失败的时候如果不执行下面的语句,可能导致SPI读写异常
											{
												SD_SPI_SpeedLow();
												SD_SPI_ReadWriteByte(0xff);//提供额外的8个时钟
												SD_SPI_SpeedHigh();
												return RES_NOTRDY; //未准备
											}
	                  	return RES_OK;

	    case EX_FLASH :
											for(;count>0;count--) //
											{
												SPI_Flash_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
												sector++;
												buff+=FLASH_SECTOR_SIZE;
											}
		                  return RES_OK;

	}
	return RES_PARERR;  //无效参数
}



//写一个扇区
#if FF_FS_READONLY == 0

DRESULT disk_write 
(
	BYTE  pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT  count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;
	
  if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误	
	switch (pdrv) 
		{
	      case SD_CARD: res=SD_WriteDisk((u8*)buff,sector,count); //写入成功返回0
		                  if(res){ break;}
			                return res;
			
	      case EX_FLASH:			
					              for(;count>0;count--)
												{										    
													SPI_Flash_Write((u8*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
													sector++;
													buff+=FLASH_SECTOR_SIZE;
												}
												res=0;
		                    return RES_OK;
	  }
	return RES_PARERR;
}

#endif


//其他磁盘控制
DRESULT disk_ioctl
(
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,	  	/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;

	switch (pdrv) 
		{
	       case SD_CARD : 
					              switch(cmd)
												{
													case CTRL_SYNC:         
														                      SD_CS(0);													
																									if(SD_WaitReady()==0)res = RES_OK; 
																									else res = RES_ERROR;	  
																									SD_CS(1);
																									break;	 
													case GET_SECTOR_SIZE:
																									*(WORD*)buff = 512;
																									res = RES_OK;
																									break;	 
													case GET_BLOCK_SIZE:
																									*(WORD*)buff = 8;
																									res = RES_OK;
																									break;	 
													case GET_SECTOR_COUNT:
																									*(DWORD*)buff = SD_GetSectorCount();
																									res = RES_OK;
																									break;
													default:
															                    res = RES_PARERR;
															                    break;
												}
												return res;
	       case EX_FLASH :
													switch(cmd)
														{
															case CTRL_SYNC:
																										res = RES_OK; 
																										break;	 
															case GET_SECTOR_SIZE:
																										*(WORD*)buff = FLASH_SECTOR_SIZE;
																										res = RES_OK;
																										break;	 
															case GET_BLOCK_SIZE:
																										*(WORD*)buff = FLASH_BLOCK_SIZE;
																										res = RES_OK;
																										break;	 
															case GET_SECTOR_COUNT:
																										*(DWORD*)buff = FLASH_SECTOR_COUNT;
																										res = RES_OK;
																										break;
															default:
																										res = RES_PARERR;
																										break;
														}
														return res;		                

	 }
	return RES_PARERR;
}

//获得时间
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{				 
	return 0;
}	

