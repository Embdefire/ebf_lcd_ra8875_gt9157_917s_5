#ifndef __BSP_RA8875_LCD_H
#define	__BSP_RA8875_LCD_H

#include "stm32f10x.h"
#include "GUI.h"

/***************************************************************************************
2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

ѡ��BANK1-BORSRAM4 ���� TFT����ַ��ΧΪ0X6C00 0000 ~ 0X6FFF FFFF
FSMC_A23 ��LCD��DC(�Ĵ���/����ѡ��)��
�Ĵ�������ַ = 0X6C00 0000
RAM����ַ = 0X6D00 0000 = 0X6C00 0000+2^23*2 = 0X6C00 0000 + 0X100 0000 = 0X6D00 0000
��ѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼���  
****************************************************************************************/

//RA8875��RS�ߵ�ƽ��ʾ�Ĵ������͵�ƽ��ʾ���ݣ���ILI9341�෴

#define Bank1_LCD_RSL    ((u32)0x6C000000)	      //��Һ������RS�������ӵ�A23Ϊ �͵�ƽ
#define Bank1_LCD_RSH    ((u32)0x6D000000)       //��Һ������RS�������ӵ�A23Ϊ �ߵ�ƽ// A23 PE2


/*ѡ��LCDָ���Ĵ���*/
#define LCD_WR_RSH(val)      ((*(__IO u16 *) (Bank1_LCD_RSH)) = ((u16)val))   //��RSH��ַд��
#define LCD_WR_RSL(val)      ((*(__IO u16 *) (Bank1_LCD_RSL)) = ((u16)(val))) //��RSL��ַд��
#define LCD_RD_RSH()         (*(__IO u16 *) (Bank1_LCD_RSH))                   //��RSH��ַ����
#define LCD_RD_RSL()         (*(__IO u16 *) (Bank1_LCD_RSL))                   //��RSL��ַ���� 

/*RA8875 ��RSΪ�ߵ�ƽʱ��ʾ�Ĵ������͵�ƽ��ʾ���ݣ���ILI9341�෴*/
#define LCD_RA8875_REG(index)            LCD_WR_RSH(index)                    
#define LCD_RA8875_WRITE_DATA(val)       LCD_WR_RSL(val)
#define LCD_RA8875_READ_DATA()           LCD_RD_RSL() 
#define LCD_RA8875_READ_REG()            LCD_RD_RSH() 
#define LCD_RA8875_READ_STATUS()          LCD_RD_RSH()

void LCD_Init(void);


enum
{
	RA_FONT_16 = 0,		/* RA8875 ���� 16���� */
	RA_FONT_24 = 1,		/* RA8875 ���� 24���� */
	RA_FONT_32 = 2		/* RA8875 ���� 32���� */
};

u16 GUIColorToRGB(GUI_COLOR color);
void RA8875_SetFrontColor(uint16_t _usColor);
void RA8875_SetBackColor(uint16_t _usColor);
void RA8875_SetFont(uint8_t _ucFontType, uint8_t _ucLineSpace, uint8_t _ucCharSpace);
void RA8875_DispChar(U16P c);
void RA8875_SetTextMode(uint8_t Mode);
void RA8875_SetTextCursor(uint16_t _usX, uint16_t _usY);
void RA8875_SetCursor(uint16_t _usX, uint16_t _usY);
void RA8875_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth);

uint16_t RA8875_ReadReg(uint16_t RegAddr);
void RA8875_WriteReg(uint8_t RegAddr, uint8_t RegValue);

void BTE_Start(void);
void BTE_Wait(void);
void BTE_SetSourse(U16 x0,U16 y0,U16 Layer);
void BTE_SetDist(U16 x0,U16 y0,U16 Layer);
void BTE_SetArea(U16 x0,U16 y0);
void BTE_DRAW(U16 x,U16 y,U16 ch,U8 u_Font);
#endif /* __BSP_RA8875_LCD_H */
