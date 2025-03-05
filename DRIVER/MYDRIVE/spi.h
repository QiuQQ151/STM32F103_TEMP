#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"



// SPI �����ٶ�����
#define SPI_SPEED_2    0
#define SPI_SPEED_4    1
#define SPI_SPEED_8    2
#define SPI_SPEED_16   3
#define SPI_SPEED_32   4
#define SPI_SPEED_64   5
#define SPI_SPEED_128  6
#define SPI_SPEED_256  7


void SPI3_Init(void);            //��ʼ�� SPI ��
void SPI3_SetSpeed(u8 SpeedSet); //���� SPI �ٶ�
u8   SPI3_RW_Byte(u8 TxData);    //SPI ���߶�дһ���ֽ�

void SPI2_Init(void);            //SPI2��ʼ��
void SPI2_SetSpeed(u8 SpeedSet); //���� SPI �ٶ�
u8   SPI2_RW_Byte(u8 TxData);    //SPI ���߶�дһ���ֽ�
void SPI2_DMA_Init(void);
void SPI2_DMA_Transmit(uint8_t *txBuffer, uint8_t *rxBuffer, uint16_t size);


#endif
