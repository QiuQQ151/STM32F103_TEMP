#include "fontupdate.h"
#include "ff.h"	  
#include "flash.h"   
#include "GUI.h"
#include "lcd.h"
// RTOS
#include "FreeRTOS.h"
#include "task.h"

//�ֿ�����ʼ��ַ
#define FONTINFOADDR 	10*1024*1024 				//����16flash����10M���ַ��ʼ

//�ֿ���Ϣ�ṹ��. 
//���������ֿ������Ϣ����ַ����С��
_font_info ftinfo;


//�ֿ�����sd���е�·��/sd "0:"
const u8 *GBK24_PATH=(u8*)"0:/SYSTEM/FONT/GBK24.FON";		//GBK24�Ĵ��λ��
const u8 *GBK16_PATH=(u8*)"0:/SYSTEM/FONT/GBK16.FON";		//GBK16�Ĵ��λ��
const u8 *GBK12_PATH=(u8*)"0:/SYSTEM/FONT/GBK12.FON";		//GBK12�Ĵ��λ��
const u8 *UNIGBK_PATH=(u8*)"0:/SYSTEM/FONT/UNIGBK.BIN";		//UNIGBK.BIN�Ĵ��λ��


//��ʾ��ǰ������½���
//x,y:����
//size:�����С
//fsize:�����ļ���С
//pos:��ǰ�ļ�ָ��λ��
u32 fupd_prog(u16 x,u16 y,u8 size,u32 fsize,u32 pos)
{
	float prog;
	u8 t=0XFF;
	prog=(float)pos/fsize;
	prog*=100;
	if(t!=prog)
	{
	  ShowFontStr(x+3*size/2,y,RED,WHITE,(u8*)"%",size,0);		
		t=prog;
		if(t>100)t=100;
	  LCD_ShowNum(x,y,prog,3,size);//��ʾ��ֵ
	}
	return 0;					    
} 


//����ĳһ��
//x,y:����
//size:�����С
//fxpath:·��
//fx:���µ����� 0,ungbk;1,gbk12;2,gbk16;3,gbk24;
//����ֵ:0,�ɹ�;����,ʧ��.
u8 updata_fontx(u16 x,u16 y,u8 size,u8 *fxpath,u8 fx)
{
	u32 flashaddr=0;								    
	FIL *fftemp;
	FILINFO *ffifo;  //���ڲ�ѯ�ļ�״̬
	u8 *tempbuf;
 	u8 res;	
	u16 bread;
	u32 offx=0;
	u8 rval=0;	   
  
	fftemp=(FIL *)pvPortMalloc(sizeof(FIL));	 //�����ڴ�	
	ffifo=(FILINFO *)pvPortMalloc(sizeof(FILINFO));
	if(fftemp==NULL||ffifo==NULL) rval=1;               //�ڴ����ʧ��
	tempbuf=pvPortMalloc(4096);	//����4096���ֽڿռ�
	if(tempbuf==NULL)rval=1;//�ڴ����ʧ��
	
 	res=f_open(fftemp,(const TCHAR*)fxpath,FA_READ); //���ļ� 
 	if(res)rval=2;//���ļ�ʧ��
  
 	if(rval==0)	 //�򿪳ɹ�
	{
		f_stat((const TCHAR*)fxpath,ffifo); //��ѯ�򿪺���ļ�״̬
		switch(fx)
		{
			case 0:												                        //����UNIGBK.BIN
							ftinfo.ugbkaddr=FONTINFOADDR+sizeof(ftinfo);	//��Ϣͷ֮�󣬽���UNIGBKת�����
							ftinfo.ugbksize=ffifo->fsize;					        //UNIGBK��С
							flashaddr=ftinfo.ugbkaddr;
							break;
			case 1:
							ftinfo.f12addr=ftinfo.ugbkaddr+ftinfo.ugbksize;	//UNIGBK֮�󣬽���GBK12�ֿ�
							ftinfo.gbk12size=ffifo->fsize;					        //GBK12�ֿ��С
							flashaddr=ftinfo.f12addr;						            //GBK12����ʼ��ַ
							break;
			case 2:
							ftinfo.f16addr=ftinfo.f12addr+ftinfo.gbk12size;	//GBK12֮�󣬽���GBK16�ֿ�
							ftinfo.gbk16size=ffifo->fsize;					        //GBK16�ֿ��С
							flashaddr=ftinfo.f16addr;						            //GBK16����ʼ��ַ
							break;
			case 3:
							ftinfo.f24addr=ftinfo.f16addr+ftinfo.gbk16size;	//GBK16֮�󣬽���GBK24�ֿ�
							ftinfo.gkb24size=ffifo->fsize;					        //GBK24�ֿ��С
							flashaddr=ftinfo.f24addr;					             	//GBK24����ʼ��ַ
							break;
		}   
		while(res==FR_OK)//��ѭ��ִ��
		{
	 		res=f_read(fftemp,tempbuf,4096,(UINT *)&bread);		//��ȡ����	 
			if(res!=FR_OK) break;								//ִ�д���
			SPI_Flash_Write(tempbuf,offx+flashaddr,4096);		//��0��ʼд��4096������  
	  		offx+=bread;	  
			fupd_prog(x,y,size,ffifo->fsize,offx);	 			//������ʾ
			if(bread!=4096)break;								//������.
	 	} 	
		f_close(fftemp);		
	}			 
	vPortFree(fftemp);	//�ͷ��ڴ�
	vPortFree(tempbuf);	//�ͷ��ڴ�
	vPortFree(ffifo);
	return res;
}





//���������ļ�,UNIGBK,GBK12,GBK16,GBK24һ�����
//x,y:��ʾ��Ϣ����ʾ��ַ
//size:��ʾ��Ϣ�����С										  
//����ֵ:0,���³ɹ�; ����,�������.  
u8 update_font(u16 x,u16 y,u8 size)
{	
	u8 *gbk24_path=(u8*)GBK24_PATH;
	u8 *gbk16_path=(u8*)GBK16_PATH;
	u8 *gbk12_path=(u8*)GBK12_PATH;
	u8 *unigbk_path=(u8*)UNIGBK_PATH;
	u8 res;		   
 	res=0XFF;		
	ftinfo.fontok=0XFF;
	
  SPI_Flash_Write((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));	//���֮ǰ�ֿ�ɹ��ı�־.��ֹ���µ�һ������,���µ��ֿⲿ�����ݶ�ʧ.
 	SPI_Flash_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));	//���¶���ftinfo�ṹ������
 	LCD_ShowString(x,y,size,(u8*)"Updating UNIGBK.BIN",1);		
	res=updata_fontx(x+20*size/2,y,size,unigbk_path,0);			//����UNIGBK.BIN
	if(res)return 1;
 	LCD_ShowString(x,y+30,size,(u8*)"Updating GBK12.BIN  ",1);
	res=updata_fontx(x+20*size/2,y,size,gbk12_path,1);			//����GBK12.FON
	if(res)return 2;
	LCD_ShowString(x,y+60,size,(u8*)"Updating GBK16.BIN  ",1);
	res=updata_fontx(x+20*size/2,y,size,gbk16_path,2);			//����GBK16.FON
	if(res)return 3;
	LCD_ShowString(x,y+90,size,(u8*)"Updating GBK24.BIN  ",1);
	res=updata_fontx(x+20*size/2,y,size,gbk24_path,3);			//����GBK24.FON
	if(res)return 4;	   
	//ȫ�����º���
	ftinfo.fontok=0XAA;
  SPI_Flash_Write((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));	//�����ֿ���Ϣ
	LCD_ShowString(x,y+120,size,(u8*)"ok��������",1);
	ShowFontStr(x,y+150,RED,WHITE,(u8*)"����Ϊ�ֿ������䣬У���ֿ��ѯ��ȷ�ԣ�",size,0);
	return 0;//�޴���.		 
} 

//��ʼ������
//����ֵ:0,�ֿ����.����,�ֿⶪʧ	 
u8 font_init(void)
{			  												 
	SPI_Flash_Init();  
 	SPI_Flash_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//����ftinfo�ṹ������
	if(ftinfo.fontok!=0XAA)return 1;			//�ֿ����. 
	return 0;		    
}


//���ֿ��в��ҳ���ģ
//code �ַ����Ŀ�ʼ��ַ,GBK��
//mat  ���ݴ�ŵ�ַ (size/8+((size%8)?1:0))*(size) bytes��С	
//size:�����С
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{		    
	unsigned char qh,ql;
	unsigned char i;					  
	unsigned long foffset; 
	u8 csize=(size/8+((size%8)?1:0))*(size); //�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	
	qh=*code;     //��λ
	ql=*(++code); //��λ
	
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//�ǳ��ú���
	{   		    
	    for(i=0;i<csize;i++)*mat++=0xff;//�������
	    return; //��������
	}  
	
	if(ql<0x7f)ql-=0x40;//ע��!
	else ql-=0x41;
	qh-=0x81;   
	foffset=((unsigned long)190*qh+ql)*csize;	//�õ��ֿ��е��ֽ�ƫ����  		  
	switch(size)
	{
		case 12:
			SPI_Flash_Read(mat,foffset+ftinfo.f12addr,24);
			break;
		case 16:
			SPI_Flash_Read(mat,foffset+ftinfo.f16addr,32);
			break;
		case 24:
			SPI_Flash_Read(mat,foffset+ftinfo.f24addr,72);
			break;
			
	}     												    
}  

