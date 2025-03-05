#include "fontupdate.h"
#include "ff.h"	  
#include "flash.h"   
#include "GUI.h"
#include "lcd.h"
// RTOS
#include "FreeRTOS.h"
#include "task.h"

//字库存放起始地址
#define FONTINFOADDR 	10*1024*1024 				//采用16flash，从10M后地址开始

//字库信息结构体. 
//用来保存字库基本信息，地址，大小等
_font_info ftinfo;


//字库存放在sd卡中的路径/sd "0:"
const u8 *GBK24_PATH=(u8*)"0:/SYSTEM/FONT/GBK24.FON";		//GBK24的存放位置
const u8 *GBK16_PATH=(u8*)"0:/SYSTEM/FONT/GBK16.FON";		//GBK16的存放位置
const u8 *GBK12_PATH=(u8*)"0:/SYSTEM/FONT/GBK12.FON";		//GBK12的存放位置
const u8 *UNIGBK_PATH=(u8*)"0:/SYSTEM/FONT/UNIGBK.BIN";		//UNIGBK.BIN的存放位置


//显示当前字体更新进度
//x,y:坐标
//size:字体大小
//fsize:整个文件大小
//pos:当前文件指针位置
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
	  LCD_ShowNum(x,y,prog,3,size);//显示数值
	}
	return 0;					    
} 


//更新某一个
//x,y:坐标
//size:字体大小
//fxpath:路径
//fx:更新的内容 0,ungbk;1,gbk12;2,gbk16;3,gbk24;
//返回值:0,成功;其他,失败.
u8 updata_fontx(u16 x,u16 y,u8 size,u8 *fxpath,u8 fx)
{
	u32 flashaddr=0;								    
	FIL *fftemp;
	FILINFO *ffifo;  //用于查询文件状态
	u8 *tempbuf;
 	u8 res;	
	u16 bread;
	u32 offx=0;
	u8 rval=0;	   
  
	fftemp=(FIL *)pvPortMalloc(sizeof(FIL));	 //分配内存	
	ffifo=(FILINFO *)pvPortMalloc(sizeof(FILINFO));
	if(fftemp==NULL||ffifo==NULL) rval=1;               //内存分配失败
	tempbuf=pvPortMalloc(4096);	//分配4096个字节空间
	if(tempbuf==NULL)rval=1;//内存分配失败
	
 	res=f_open(fftemp,(const TCHAR*)fxpath,FA_READ); //打开文件 
 	if(res)rval=2;//打开文件失败
  
 	if(rval==0)	 //打开成功
	{
		f_stat((const TCHAR*)fxpath,ffifo); //查询打开后的文件状态
		switch(fx)
		{
			case 0:												                        //更新UNIGBK.BIN
							ftinfo.ugbkaddr=FONTINFOADDR+sizeof(ftinfo);	//信息头之后，紧跟UNIGBK转换码表
							ftinfo.ugbksize=ffifo->fsize;					        //UNIGBK大小
							flashaddr=ftinfo.ugbkaddr;
							break;
			case 1:
							ftinfo.f12addr=ftinfo.ugbkaddr+ftinfo.ugbksize;	//UNIGBK之后，紧跟GBK12字库
							ftinfo.gbk12size=ffifo->fsize;					        //GBK12字库大小
							flashaddr=ftinfo.f12addr;						            //GBK12的起始地址
							break;
			case 2:
							ftinfo.f16addr=ftinfo.f12addr+ftinfo.gbk12size;	//GBK12之后，紧跟GBK16字库
							ftinfo.gbk16size=ffifo->fsize;					        //GBK16字库大小
							flashaddr=ftinfo.f16addr;						            //GBK16的起始地址
							break;
			case 3:
							ftinfo.f24addr=ftinfo.f16addr+ftinfo.gbk16size;	//GBK16之后，紧跟GBK24字库
							ftinfo.gkb24size=ffifo->fsize;					        //GBK24字库大小
							flashaddr=ftinfo.f24addr;					             	//GBK24的起始地址
							break;
		}   
		while(res==FR_OK)//死循环执行
		{
	 		res=f_read(fftemp,tempbuf,4096,(UINT *)&bread);		//读取数据	 
			if(res!=FR_OK) break;								//执行错误
			SPI_Flash_Write(tempbuf,offx+flashaddr,4096);		//从0开始写入4096个数据  
	  		offx+=bread;	  
			fupd_prog(x,y,size,ffifo->fsize,offx);	 			//进度显示
			if(bread!=4096)break;								//读完了.
	 	} 	
		f_close(fftemp);		
	}			 
	vPortFree(fftemp);	//释放内存
	vPortFree(tempbuf);	//释放内存
	vPortFree(ffifo);
	return res;
}





//更新字体文件,UNIGBK,GBK12,GBK16,GBK24一起更新
//x,y:提示信息的显示地址
//size:提示信息字体大小										  
//返回值:0,更新成功; 其他,错误代码.  
u8 update_font(u16 x,u16 y,u8 size)
{	
	u8 *gbk24_path=(u8*)GBK24_PATH;
	u8 *gbk16_path=(u8*)GBK16_PATH;
	u8 *gbk12_path=(u8*)GBK12_PATH;
	u8 *unigbk_path=(u8*)UNIGBK_PATH;
	u8 res;		   
 	res=0XFF;		
	ftinfo.fontok=0XFF;
	
  SPI_Flash_Write((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));	//清除之前字库成功的标志.防止更新到一半重启,导致的字库部分数据丢失.
 	SPI_Flash_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));	//重新读出ftinfo结构体数据
 	LCD_ShowString(x,y,size,(u8*)"Updating UNIGBK.BIN",1);		
	res=updata_fontx(x+20*size/2,y,size,unigbk_path,0);			//更新UNIGBK.BIN
	if(res)return 1;
 	LCD_ShowString(x,y+30,size,(u8*)"Updating GBK12.BIN  ",1);
	res=updata_fontx(x+20*size/2,y,size,gbk12_path,1);			//更新GBK12.FON
	if(res)return 2;
	LCD_ShowString(x,y+60,size,(u8*)"Updating GBK16.BIN  ",1);
	res=updata_fontx(x+20*size/2,y,size,gbk16_path,2);			//更新GBK16.FON
	if(res)return 3;
	LCD_ShowString(x,y+90,size,(u8*)"Updating GBK24.BIN  ",1);
	res=updata_fontx(x+20*size/2,y,size,gbk24_path,3);			//更新GBK24.FON
	if(res)return 4;	   
	//全部更新好了
	ftinfo.fontok=0XAA;
  SPI_Flash_Write((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));	//保存字库信息
	LCD_ShowString(x,y+120,size,(u8*)"ok！！！！",1);
	ShowFontStr(x,y+150,RED,WHITE,(u8*)"本句为字库测试语句，校验字库查询正确性！",size,0);
	return 0;//无错误.		 
} 

//初始化字体
//返回值:0,字库完好.其他,字库丢失	 
u8 font_init(void)
{			  												 
	SPI_Flash_Init();  
 	SPI_Flash_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//读出ftinfo结构体数据
	if(ftinfo.fontok!=0XAA)return 1;			//字库错误. 
	return 0;		    
}


//从字库中查找出字模
//code 字符串的开始地址,GBK码
//mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小	
//size:字体大小
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{		    
	unsigned char qh,ql;
	unsigned char i;					  
	unsigned long foffset; 
	u8 csize=(size/8+((size%8)?1:0))*(size); //得到字体一个字符对应点阵集所占的字节数	 
	
	qh=*code;     //高位
	ql=*(++code); //低位
	
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非常用汉字
	{   		    
	    for(i=0;i<csize;i++)*mat++=0xff;//填充满格
	    return; //结束访问
	}  
	
	if(ql<0x7f)ql-=0x40;//注意!
	else ql-=0x41;
	qh-=0x81;   
	foffset=((unsigned long)190*qh+ql)*csize;	//得到字库中的字节偏移量  		  
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

