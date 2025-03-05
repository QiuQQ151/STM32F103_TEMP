#include "sram.h"
#include "stm32f10x.h"
#include "led.h"


/**初始化 外部SRAM*/
void sram_init(void)
{
     /* CS脚时钟使能 G10*/
	   RCC->APB2ENR|=1<<8;
	   GPIOG->CRH&=0XFFFFF0FF;
	   GPIOG->CRH|=0X00000B00;
     /* WR\RD脚时钟使能 D5 D4*/
	   RCC->APB2ENR|=1<<5;
	   GPIOD->CRL&=0XFF00FFFF;
	   GPIOD->CRL|=0X00BB0000;	  
     /* NBL0, NBL1, 高低字节选择 IO初始化 E0 E1*/
	   RCC->APB2ENR|=1<<6;
	   GPIOE->CRL&=0XFFFFFF00;
	   GPIOE->CRL|=0X000000BB;		
	
	   RCC->AHBENR|=1<<8;  //使能FSMC时钟
    
    /* SRAM A0~A19 D0~D15设置 */
    RCC->APB2ENR|=0XF<<5;  //开启DEFG时钟
	  GPIOD->CRL&=0XFFFFFF00;
	  GPIOD->CRL|=0X000000BB;
	  GPIOD->CRH&=0X00000000;
	  GPIOD->CRH|=0XBBBBBBBB;	
	
	  GPIOE->CRL&=0X0FFFFFFF;
	  GPIOE->CRL|=0XB0000000;
	  GPIOE->CRH&=0X00000000;
	  GPIOE->CRH|=0XBBBBBBBB;		
		
		GPIOF->CRL&=0XFF000000;
	  GPIOF->CRL|=0X00BBBBBB;
	  GPIOF->CRH&=0X0000FFFF;
	  GPIOF->CRH|=0XBBBB0000;	
		
	  GPIOG->CRL&=0XFF000000;
	  GPIOG->CRL|=0X00BBBBBB;
		
		FSMC_Bank1->BTCR[4]=0X00000000;   //清零
		FSMC_Bank1->BTCR[5]=0X00000000;
		FSMC_Bank1E->BWTR[4]=0X00000000;	

    FSMC_Bank1->BTCR[4]|=1<<12; //写使能
		FSMC_Bank1->BTCR[4]|=1<<4;  //总线宽度，16bit
		FSMC_Bank1->BTCR[4]|=0<<2;  //存储类型：设置为SRAM
		FSMC_Bank1->BTCR[4]|=1<<0;  //对应的存储块使能	
		
		FSMC_Bank1->BTCR[5]|=3<<8;  //数据保持时间
		FSMC_Bank1->BTCR[5]|=0<<4;  //地址保持时间
		FSMC_Bank1->BTCR[5]|=0<<0;	//地址建立时间	
		
		FSMC_Bank1E->BWTR[4]=0X0FFFFFFF; //访问模式A


		
		
}

/**
 * @brief       往SRAM指定地址写入指定长度数据
 * @param       pbuf    : 数据存储区
 * @param       addr    : 开始写入的地址(最大32bit)
 * @param       datalen : 要写入的字节数(最大32bit)
 * @retval      无
 */
void sram_write(uint8_t *pbuf, uint32_t addr, uint32_t datalen)
{
    for (; datalen != 0; datalen--)
    {
        *(volatile uint8_t *)(SRAM_BASE_ADDR + addr) = *pbuf;
        addr++;
        pbuf++;
    }
}

/**
 * @brief       从SRAM指定地址读取指定长度数据
 * @param       pbuf    : 数据存储区
 * @param       addr    : 开始读取的地址(最大32bit)
 * @param       datalen : 要读取的字节数(最大32bit)
 * @retval      无
 */
void sram_read(uint8_t *pbuf, uint32_t addr, uint32_t datalen)
{
    for (; datalen != 0; datalen--)
    {
        *pbuf++ = *(volatile uint8_t *)(SRAM_BASE_ADDR + addr);
        addr++;
    }
}

/*******************测试函数**********************************/

/**
 * @brief       测试函数 在SRAM指定地址写入1个字节
 * @param       addr    : 开始写入的地址(最大32bit)
 * @param       data    : 要写入的字节
 * @retval      无
 */
void sram_test_write(uint32_t addr, uint8_t data)
{
    sram_write(&data, addr, 1); /* 写入1个字节 */
}

/**
 * @brief       测试函数 在SRAM指定地址读取1个字节
 * @param       addr    : 开始读取的地址(最大32bit)
 * @retval      读取到的数据(1个字节)
 */
uint8_t sram_test_read(uint32_t addr)
{
    uint8_t data;
    sram_read(&data, addr, 1); /* 读取1个字节 */
    return data;
}


/**
 * @brief       外部内存测试(最大支持1M字节内存测试)
 * @param       无
 * @retval      无
 */
void fsmc_sram_test(uint16_t x, uint16_t y)
{
    uint32_t i = 0;
    uint8_t temp = 0;
    uint8_t sval = 0; /* 在地址0读到的数据 */

   // ShowFontStr(x, y,WHITE,BLACK, (u8*)"Test:   0KB", 16,0);

    /* 每隔4K字节,写入一个数据,总共写入256个数据,刚好是1M字节 */
    for (i = 0; i < 1024 * 1024; i += 1)
    {
        sram_write(&temp, i, 1);
        temp++;
    }

    /* 依次读出之前写入的数据,进行校验 */
    for (i = 0; i < 1024 * 1024; i += 1)
    {
        sram_read(&temp, i, 1);

        if (i == 0)
        {
            sval = temp;
        }
        else if (temp <= sval)
        {
            break; /* 后面读出的数据一定要比第一次读到的数据大 */
        }

     //   ShowNum(x + 5 * 8, y, (uint16_t)(temp - sval + 1) * 4, 4, 16); /* 显示内存容量 */
    }
}








