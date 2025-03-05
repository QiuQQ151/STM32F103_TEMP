#include "sram.h"
#include "stm32f10x.h"
#include "led.h"


/**��ʼ�� �ⲿSRAM*/
void sram_init(void)
{
     /* CS��ʱ��ʹ�� G10*/
	   RCC->APB2ENR|=1<<8;
	   GPIOG->CRH&=0XFFFFF0FF;
	   GPIOG->CRH|=0X00000B00;
     /* WR\RD��ʱ��ʹ�� D5 D4*/
	   RCC->APB2ENR|=1<<5;
	   GPIOD->CRL&=0XFF00FFFF;
	   GPIOD->CRL|=0X00BB0000;	  
     /* NBL0, NBL1, �ߵ��ֽ�ѡ�� IO��ʼ�� E0 E1*/
	   RCC->APB2ENR|=1<<6;
	   GPIOE->CRL&=0XFFFFFF00;
	   GPIOE->CRL|=0X000000BB;		
	
	   RCC->AHBENR|=1<<8;  //ʹ��FSMCʱ��
    
    /* SRAM A0~A19 D0~D15���� */
    RCC->APB2ENR|=0XF<<5;  //����DEFGʱ��
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
		
		FSMC_Bank1->BTCR[4]=0X00000000;   //����
		FSMC_Bank1->BTCR[5]=0X00000000;
		FSMC_Bank1E->BWTR[4]=0X00000000;	

    FSMC_Bank1->BTCR[4]|=1<<12; //дʹ��
		FSMC_Bank1->BTCR[4]|=1<<4;  //���߿�ȣ�16bit
		FSMC_Bank1->BTCR[4]|=0<<2;  //�洢���ͣ�����ΪSRAM
		FSMC_Bank1->BTCR[4]|=1<<0;  //��Ӧ�Ĵ洢��ʹ��	
		
		FSMC_Bank1->BTCR[5]|=3<<8;  //���ݱ���ʱ��
		FSMC_Bank1->BTCR[5]|=0<<4;  //��ַ����ʱ��
		FSMC_Bank1->BTCR[5]|=0<<0;	//��ַ����ʱ��	
		
		FSMC_Bank1E->BWTR[4]=0X0FFFFFFF; //����ģʽA


		
		
}

/**
 * @brief       ��SRAMָ����ַд��ָ����������
 * @param       pbuf    : ���ݴ洢��
 * @param       addr    : ��ʼд��ĵ�ַ(���32bit)
 * @param       datalen : Ҫд����ֽ���(���32bit)
 * @retval      ��
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
 * @brief       ��SRAMָ����ַ��ȡָ����������
 * @param       pbuf    : ���ݴ洢��
 * @param       addr    : ��ʼ��ȡ�ĵ�ַ(���32bit)
 * @param       datalen : Ҫ��ȡ���ֽ���(���32bit)
 * @retval      ��
 */
void sram_read(uint8_t *pbuf, uint32_t addr, uint32_t datalen)
{
    for (; datalen != 0; datalen--)
    {
        *pbuf++ = *(volatile uint8_t *)(SRAM_BASE_ADDR + addr);
        addr++;
    }
}

/*******************���Ժ���**********************************/

/**
 * @brief       ���Ժ��� ��SRAMָ����ַд��1���ֽ�
 * @param       addr    : ��ʼд��ĵ�ַ(���32bit)
 * @param       data    : Ҫд����ֽ�
 * @retval      ��
 */
void sram_test_write(uint32_t addr, uint8_t data)
{
    sram_write(&data, addr, 1); /* д��1���ֽ� */
}

/**
 * @brief       ���Ժ��� ��SRAMָ����ַ��ȡ1���ֽ�
 * @param       addr    : ��ʼ��ȡ�ĵ�ַ(���32bit)
 * @retval      ��ȡ��������(1���ֽ�)
 */
uint8_t sram_test_read(uint32_t addr)
{
    uint8_t data;
    sram_read(&data, addr, 1); /* ��ȡ1���ֽ� */
    return data;
}


/**
 * @brief       �ⲿ�ڴ����(���֧��1M�ֽ��ڴ����)
 * @param       ��
 * @retval      ��
 */
void fsmc_sram_test(uint16_t x, uint16_t y)
{
    uint32_t i = 0;
    uint8_t temp = 0;
    uint8_t sval = 0; /* �ڵ�ַ0���������� */

   // ShowFontStr(x, y,WHITE,BLACK, (u8*)"Test:   0KB", 16,0);

    /* ÿ��4K�ֽ�,д��һ������,�ܹ�д��256������,�պ���1M�ֽ� */
    for (i = 0; i < 1024 * 1024; i += 1)
    {
        sram_write(&temp, i, 1);
        temp++;
    }

    /* ���ζ���֮ǰд�������,����У�� */
    for (i = 0; i < 1024 * 1024; i += 1)
    {
        sram_read(&temp, i, 1);

        if (i == 0)
        {
            sval = temp;
        }
        else if (temp <= sval)
        {
            break; /* �������������һ��Ҫ�ȵ�һ�ζ��������ݴ� */
        }

     //   ShowNum(x + 5 * 8, y, (uint16_t)(temp - sval + 1) * 4, 4, 16); /* ��ʾ�ڴ����� */
    }
}








