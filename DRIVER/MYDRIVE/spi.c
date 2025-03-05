#include "spi.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "delay.h"


// SPI2 ��ʼ������
void SPI2_Init(void)
{
    // ���� SPI2
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//ʹ��SPI2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOB

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	 //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��GPIOB
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//����SPI����ģʽ������ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//����SPI���ݴ�С��8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//����ͬ��ʱ�ӿ���ʱSCLKλ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//����ͬ��ʱ�ӿյڶ���ʱ���ز���
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS�ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//������Ԥ��Ƶֵ��������Ԥ��ƵֵΪ2
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//���ݴ����λ����
	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2,&SPI_InitStructure);//��ʼ��SPI
	SPI_Cmd(SPI2, ENABLE);//ʹ��SPI

//    // ���� SPI2 DMA ����ʹ��
//    SPI2->CR2 |= SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN; // ʹ�� TX �� RX DMA ����
}

// ���� SPI2 ������
void SPI2_SetSpeed(uint8_t SpeedSet)
{
    SpeedSet &= 0x07; // ���Ʒ�ΧΪ 0 ~ 7
    SPI2->CR1 &= 0xFFC7;  // �������������
    SPI2->CR1 |= SpeedSet << 3;  // ���� SPI2 ������
    SPI2->CR1 |= SPI_CR1_SPE; // ȷ�� SPI2 �豸ʹ��
}

// SPI2 ���ݶ�д����
uint8_t SPI2_RW_Byte(uint8_t txData)
{
    u16 retry=0;
	while((SPI2->SR&1<<1)==0)//�ȴ���������
		{
		   retry++;
		   if(retry>0XFFFE) return 0;
		}
		
		SPI2->DR=txData; //����һ�� byte
		retry=0;
		//delay_us(50);
		while((SPI2->SR&1<<0)==0) //�ȴ�������һ�� byte
		{
		   retry++;
		   if(retry>0XFFFE) return 0;
		}
return SPI2->DR; //�����յ�������
}

// DMA ��ʼ������
void SPI2_DMA_Init(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  // ʹ�� DMA1 ʱ��

    DMA_InitTypeDef DMA_InitStructure;

    // ���� SPI2 TX DMA (DMA1_Channel5)
    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;  // SPI2 ���ݼĴ�����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = 0;  // ��ʼ�ڴ��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  // �����ڴ浽����
    DMA_InitStructure.DMA_BufferSize = 0;  // ��ʼ��������С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);

    // ���� SPI2 RX DMA (DMA1_Channel4)
    DMA_DeInit(DMA1_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  // �������赽�ڴ�
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);

    // ���� SPI2 �� DMA ����
    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);  // ʹ�� SPI2 TX DMA
    SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);  // ʹ�� SPI2 RX DMA
}

// DMA ���亯��
void SPI2_DMA_Transmit(uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t size)
{
    // ���� DMA ����
    DMA_Cmd(DMA1_Channel4, DISABLE);
    DMA_Cmd(DMA1_Channel5, DISABLE);

    // ���� DMA ��������ַ�����ݴ�С
    DMA1_Channel4->CNDTR = size;  // ���ý������ݴ�С
    DMA1_Channel4->CMAR = (uint32_t)rxBuffer;  // ���ý��ջ�������ַ
    DMA1_Channel5->CNDTR = size;  // ���÷������ݴ�С
    DMA1_Channel5->CMAR = (uint32_t)txBuffer;  // ���÷��ͻ�������ַ

    // ���� DMA ����
    DMA_Cmd(DMA1_Channel4, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);

    // �ȴ� DMA �������
    while (!DMA_GetFlagStatus(DMA1_FLAG_TC4) || !DMA_GetFlagStatus(DMA1_FLAG_TC5));

    // ��� DMA ������ɱ�־
    DMA_ClearFlag(DMA1_FLAG_TC4);
    DMA_ClearFlag(DMA1_FLAG_TC5);

    // ���� DMA ����
    DMA_Cmd(DMA1_Channel4, DISABLE);
    DMA_Cmd(DMA1_Channel5, DISABLE);
}


////------------SPI2-----------------------------------//
//void SPI2_Init(void)
//{
//	   RCC->APB2ENR|=1<<3;  //GPIOBʱ��ʹ��
//	   RCC->APB1ENR|=1<<14;  //SPI2ʱ��ʹ��
//	
//	   GPIOB->CRH&=0X000FFFFF; //��ʼ��B13��14��15��
//	   GPIOB->CRH|=0XB8B00000; //GPIO��ģʽ����
//	   GPIOB->ODR|=0X7<<5;     //����
//	   
//	    SPI2->CR1|=0<<10; //ȫ˫��ģʽ
//		SPI2->CR1|=1<<9; //��� nss ����
//		SPI2->CR1|=1<<8;
//		SPI2->CR1|=1<<2; //SPI ����
//		SPI2->CR1&=0xF7FF; //8bit ���ݸ�ʽ0<<11|=0<<11
//		SPI2->CR1|=1<<1; //����ģʽ�� SCK Ϊ 1 CPOL=1
//		SPI2->CR1|=1<<0; //���ݲ����ӵڶ���ʱ����ؿ�ʼ,CPHA=1
//	    SPI2->CR1|=7<<3; //Fsck=Fcpu/256
//        SPI2->CR1|=0<<7; //MSBfirst
//        SPI2->CR1|=1<<6; //SPI �豸ʹ��
//		
//	    SPI2_SetSpeed(SPI_SPEED_2);
//        SPI2_RW_Byte(0xff); //��������(��Ҫ���ã�ά�� MOSI Ϊ��)
//	 // ���� SPI2 DMA ����ʹ��
//    SPI2->CR2 |= SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN; // ʹ�� TX �� RX DMA ���� 
//}


////SPI2 �ٶ����ú���
////SpeedSet:0~7
////SPI �ٶ�=fAPB2/2^(SpeedSet+1)
////APB2 ʱ��һ��Ϊ 72Mhz
//void SPI2_SetSpeed(u8 SpeedSet)
//{
//		SpeedSet&=0X07; //���Ʒ�Χ
//		SPI2->CR1&=0XFFC7;
//		SPI2->CR1|=SpeedSet<<3; //���� SPI2 �ٶ�
//		SPI2->CR1|=1<<6; //SPI �豸ʹ��
//}


////SPI2 ��дһ���ֽ�
////TxData:Ҫд����ֽ�
////����ֵ:��ȡ�����ֽ�
//u8 SPI2_RW_Byte(u8 TxData)
//{
//    u16 retry=0;
//	while((SPI2->SR&1<<1)==0)//�ȴ���������
//		{
//		   retry++;
//		   if(retry>0XFFFE) return 0;
//		}
//		
//		SPI2->DR=TxData; //����һ�� byte
//		retry=0;
//		
//		while((SPI2->SR&1<<0)==0) //�ȴ�������һ�� byte
//		{
//		   retry++;
//		   if(retry>0XFFFE) return 0;
//		}
//		return SPI2->DR; //�����յ�������
//}


//-----------SPI3-----------------------------------//   
//SPI3 �ٶ����ú���
//SpeedSet:0~7
//SPI�ٶ�=fAPB2/2^(SpeedSet+1)
//APB2ʱ��һ��Ϊ72Mhz
void SPI3_SetSpeed(u8 SpeedSet)
{
	SpeedSet&=0X07;			//���Ʒ�Χ
	SPI3->CR1&=0XFFC7; 
	SPI3->CR1|=SpeedSet<<3;	//����SPI3�ٶ�  
	SPI3->CR1|=1<<6; 		//SPI�豸ʹ�� 
}

//SPI3 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI3_RW_Byte(u8 TxData)
{		
	u16 retry=0;				 
	while((SPI3->SR&1<<1)==0)//�ȴ���������	
	{
		retry++;
		if(retry>0XFFFE)return 0;
	}			  
	SPI3->DR=TxData;	 	  //����һ��byte 
	retry=0;
	while((SPI3->SR&1<<0)==0) //�ȴ�������һ��byte  
	{
		retry++;
		if(retry>0XFFFE)return 0;
	}	  						    
	return SPI3->DR;          //�����յ�������				    
}


//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25Q64/24L01 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI3�ĳ�ʼ��
void SPI3_Init(void)
{	 
  //ʹ����SPI3��B345��Ҫ�ض���IO��
	RCC->APB2ENR|=1<<0;      //ʹ��AFIO
	AFIO->MAPR&=!(7<<24);    //����SWģʽ
	AFIO->MAPR|=(2<<24);
	
	RCC->APB1ENR|=1<<15;      //SPI3ʱ��ʹ�� 
	//����ֻ���SPI�ڳ�ʼ��
	RCC->APB2ENR|=1<<3;       //PORTBʱ��ʹ�� 	
	GPIOB->CRL&=0XFF000FFF; 
	GPIOB->CRL|=0X00BBB000;//PB345���� 	    
	GPIOB->ODR|=0X7<<3;    //����
		
	SPI3->CR1|=0<<10;//ȫ˫��ģʽ	
	SPI3->CR1|=1<<9; //���nss����
	SPI3->CR1|=1<<8;  

	SPI3->CR1|=1<<2; //SPI����
	SPI3->CR1|=0<<11;//8bit���ݸ�ʽ	
	SPI3->CR1|=1<<1; //����ģʽ��SCKΪ1 CPOL=1
	SPI3->CR1|=1<<0; //���ݲ����ӵڶ���ʱ����ؿ�ʼ,CPHA=1  
	SPI3->CR1|=7<<3; //Fsck=Fcpu/256
	SPI3->CR1|=0<<7; //MSBfirst   
	SPI3->CR1|=1<<6; //SPI�豸ʹ��
	SPI3_RW_Byte(0xff);//��������(��Ҫ���ã�ά��MOSIΪ��)		 
}
