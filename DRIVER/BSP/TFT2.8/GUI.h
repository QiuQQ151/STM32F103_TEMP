#ifndef __GUI_H__
#define __GUI_H__
#include "stm32f10x.h"

/* 用户二次封装  */
static void swap(u16 *a, u16 *b);
void Draw_Point(u16 x,u16 y,u16 color);//画点
void Draw_Line(u16 x1, u16 y1, u16 x2, u16 y2);     //画线
void Draw_Rectangle(u16 x1, u16 y1, u16 x2, u16 y2); //画框
void Draw_Fill_Rectangle(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void Draw_Circle(int xc, int yc,u16 color,int r, int fill);
void _draw_circle_8(int xc, int yc, int x, int y, u16 c);
void Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void Draw_Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
//按钮底色
void Draw_Button(u16 x,u16 y,u16 color,u8 strcount,u8 strsize);
void ShowFontStrButton(u16 x,u16 y,u16 fc,u16 bc,u8* str,u8 size,u16 button_color); 
//字符显示 内部ASCII字库
void ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode); //显示单个字符
void ShowString(u16 x,u16 y,u16 fc, u16 bc,u8 *p,u8 size,u8 mode); //显示字符串
void ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);                  //显示数字
//外部GBK+内部ASCII 混用
void ShowFontStr_area(u16 x,u16 y,u16 width,u16 height,u8* str,u8 size,u8 mode); //显示字符串，汉字加字符，汉字为外部库，字符为内部库
void ShowFontStr(u16 x,u16 y,u16 fc,u16 bc,u8* str,u8 size,u8 mode);             //按行显示
void ShowFont(u16 x,u16 y,u16 fc, u16 bc,u8 *font,u8 size,u8 mode);              //显示单个汉字


/* LCD厂商提供 */
u32 mypow(u8 m,u8 n);
void GUI_DrawPoint(u16 x,u16 y,u16 color);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_Draw_Circle(u16 x0,u16 y0,u16 fc,u8 r);
void Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len,u8 size,u8 mode);
void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode);
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void Show_Str(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p); //显示40*40 QQ图片
void gui_circle(int xc, int yc,u16 c,int r, int fill);
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, u8 *str,u8 size,u8 mode);
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
#endif

