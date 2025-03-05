#include "spi.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "delay.h"


// SPI2 初始化函数
void SPI2_Init(void)
{
    // 配置 SPI2
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//使能SPI2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	 //复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化GPIOB
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//设置SPI工作模式：主机模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//设置SPI数据大小：8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//串行同步时钟空闲时SCLK位高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//串行同步时钟空第二个时钟沿捕获
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS信号由软件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//波特率预分频值：波特率预分频值为2
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//数据传输高位先行
	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC值计算的多项式
	SPI_Init(SPI2,&SPI_InitStructure);//初始化SPI
	SPI_Cmd(SPI2, ENABLE);//使能SPI

//    // 配置 SPI2 DMA 请求使能
//    SPI2->CR2 |= SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN; // 使能 TX 和 RX DMA 请求
}

// 设置 SPI2 波特率
void SPI2_SetSpeed(uint8_t SpeedSet)
{
    SpeedSet &= 0x07; // 限制范围为 0 ~ 7
    SPI2->CR1 &= 0xFFC7;  // 清除波特率配置
    SPI2->CR1 |= SpeedSet << 3;  // 设置 SPI2 波特率
    SPI2->CR1 |= SPI_CR1_SPE; // 确保 SPI2 设备使能
}

// SPI2 数据读写函数
uint8_t SPI2_RW_Byte(uint8_t txData)
{
    u16 retry=0;
	while((SPI2->SR&1<<1)==0)//等待发送区空
		{
		   retry++;
		   if(retry>0XFFFE) return 0;
		}
		
		SPI2->DR=txData; //发送一个 byte
		retry=0;
		//delay_us(50);
		while((SPI2->SR&1<<0)==0) //等待接收完一个 byte
		{
		   retry++;
		   if(retry>0XFFFE) return 0;
		}
return SPI2->DR; //返回收到的数据
}

// DMA 初始化函数
void SPI2_DMA_Init(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  // 使能 DMA1 时钟

    DMA_InitTypeDef DMA_InitStructure;

    // 配置 SPI2 TX DMA (DMA1_Channel5)
    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;  // SPI2 数据寄存器地址
    DMA_InitStructure.DMA_MemoryBaseAddr = 0;  // 初始内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  // 方向：内存到外设
    DMA_InitStructure.DMA_BufferSize = 0;  // 初始缓冲区大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    // 配置 SPI2 RX DMA (DMA1_Channel4)
    DMA_DeInit(DMA1_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  // 方向：外设到内存
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);

    // 启用 SPI2 的 DMA 请求
    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);  // 使能 SPI2 TX DMA
    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);  // 使能 SPI2 RX DMA
}

// DMA 传输函数
void SPI2_DMA_Transmit(uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t size)
{
    // 禁用 DMA 传输
    DMA_Cmd(DMA1_Channel4, DISABLE);
    DMA_Cmd(DMA1_Channel5, DISABLE);

    // 设置 DMA 缓冲区地址和数据大小
    DMA1_Channel4->CNDTR = size;  // 设置接收数据大小
    DMA1_Channel4->CMAR = (uint32_t)rxBuffer;  // 设置接收缓冲区地址
    DMA1_Channel5->CNDTR = size;  // 设置发送数据大小
    DMA1_Channel5->CMAR = (uint32_t)txBuffer;  // 设置发送缓冲区地址

    // 启用 DMA 传输
    DMA_Cmd(DMA1_Channel4, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);

    // 等待 DMA 传输完成
    while (!DMA_GetFlagStatus(DMA1_FLAG_TC4) || !DMA_GetFlagStatus(DMA1_FLAG_TC5));

    // 清除 DMA 传输完成标志
    DMA_ClearFlag(DMA1_FLAG_TC4);
    DMA_ClearFlag(DMA1_FLAG_TC5);

    // 禁用 DMA 传输
    DMA_Cmd(DMA1_Channel4, DISABLE);
    DMA_Cmd(DMA1_Channel5, DISABLE);
}


////------------SPI2-----------------------------------//
//void SPI2_Init(void)
//{
//	   RCC->APB2ENR|=1<<3;  //GPIOB时钟使能
//	   RCC->APB1ENR|=1<<14;  //SPI2时钟使能
//	
//	   GPIOB->CRH&=0X000FFFFF; //初始化B13、14、15口
//	   GPIOB->CRH|=0XB8B00000; //GPIO口模式配置
//	   GPIOB->ODR|=0X7<<5;     //上拉
//	   
//	    SPI2->CR1|=0<<10; //全双工模式
//		SPI2->CR1|=1<<9; //软件 nss 管理
//		SPI2->CR1|=1<<8;
//		SPI2->CR1|=1<<2; //SPI 主机
//		SPI2->CR1&=0xF7FF; //8bit 数据格式0<<11|=0<<11
//		SPI2->CR1|=1<<1; //空闲模式下 SCK 为 1 CPOL=1
//		SPI2->CR1|=1<<0; //数据采样从第二个时间边沿开始,CPHA=1
//	    SPI2->CR1|=7<<3; //Fsck=Fcpu/256
//        SPI2->CR1|=0<<7; //MSBfirst
//        SPI2->CR1|=1<<6; //SPI 设备使能
//		
//	    SPI2_SetSpeed(SPI_SPEED_2);
//        SPI2_RW_Byte(0xff); //启动传输(主要作用：维持 MOSI 为高)
//	 // 配置 SPI2 DMA 请求使能
//    SPI2->CR2 |= SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN; // 使能 TX 和 RX DMA 请求 
//}


////SPI2 速度设置函数
////SpeedSet:0~7
////SPI 速度=fAPB2/2^(SpeedSet+1)
////APB2 时钟一般为 72Mhz
//void SPI2_SetSpeed(u8 SpeedSet)
//{
//		SpeedSet&=0X07; //限制范围
//		SPI2->CR1&=0XFFC7;
//		SPI2->CR1|=SpeedSet<<3; //设置 SPI2 速度
//		SPI2->CR1|=1<<6; //SPI 设备使能
//}


////SPI2 读写一个字节
////TxData:要写入的字节
////返回值:读取到的字节
//u8 SPI2_RW_Byte(u8 TxData)
//{
//    u16 retry=0;
//	while((SPI2->SR&1<<1)==0)//等待发送区空
//		{
//		   retry++;
//		   if(retry>0XFFFE) return 0;
//		}
//		
//		SPI2->DR=TxData; //发送一个 byte
//		retry=0;
//		
//		while((SPI2->SR&1<<0)==0) //等待接收完一个 byte
//		{
//		   retry++;
//		   if(retry>0XFFFE) return 0;
//		}
//		return SPI2->DR; //返回收到的数据
//}


//-----------SPI3-----------------------------------//   
//SPI3 速度设置函数
//SpeedSet:0~7
//SPI速度=fAPB2/2^(SpeedSet+1)
//APB2时钟一般为72Mhz
void SPI3_SetSpeed(u8 SpeedSet)
{
	SpeedSet&=0X07;			//限制范围
	SPI3->CR1&=0XFFC7; 
	SPI3->CR1|=SpeedSet<<3;	//设置SPI3速度  
	SPI3->CR1|=1<<6; 		//SPI设备使能 
}

//SPI3 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI3_RW_Byte(u8 TxData)
{		
	u16 retry=0;				 
	while((SPI3->SR&1<<1)==0)//等待发送区空	
	{
		retry++;
		if(retry>0XFFFE)return 0;
	}			  
	SPI3->DR=TxData;	 	  //发送一个byte 
	retry=0;
	while((SPI3->SR&1<<0)==0) //等待接收完一个byte  
	{
		retry++;
		if(retry>0XFFFE)return 0;
	}	  						    
	return SPI3->DR;          //返回收到的数据				    
}


//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25Q64/24L01 						  
//SPI口初始化
//这里针是对SPI3的初始化
void SPI3_Init(void)
{	 
  //使用了SPI3的B345，要重定义IO口
	RCC->APB2ENR|=1<<0;      //使能AFIO
	AFIO->MAPR&=!(7<<24);    //开启SW模式
	AFIO->MAPR|=(2<<24);
	
	RCC->APB1ENR|=1<<15;      //SPI3时钟使能 
	//这里只针对SPI口初始化
	RCC->APB2ENR|=1<<3;       //PORTB时钟使能 	
	GPIOB->CRL&=0XFF000FFF; 
	GPIOB->CRL|=0X00BBB000;//PB345复用 	    
	GPIOB->ODR|=0X7<<3;    //上拉
		
	SPI3->CR1|=0<<10;//全双工模式	
	SPI3->CR1|=1<<9; //软件nss管理
	SPI3->CR1|=1<<8;  

	SPI3->CR1|=1<<2; //SPI主机
	SPI3->CR1|=0<<11;//8bit数据格式	
	SPI3->CR1|=1<<1; //空闲模式下SCK为1 CPOL=1
	SPI3->CR1|=1<<0; //数据采样从第二个时间边沿开始,CPHA=1  
	SPI3->CR1|=7<<3; //Fsck=Fcpu/256
	SPI3->CR1|=0<<7; //MSBfirst   
	SPI3->CR1|=1<<6; //SPI设备使能
	SPI3_RW_Byte(0xff);//启动传输(主要作用：维持MOSI为高)		 
}
