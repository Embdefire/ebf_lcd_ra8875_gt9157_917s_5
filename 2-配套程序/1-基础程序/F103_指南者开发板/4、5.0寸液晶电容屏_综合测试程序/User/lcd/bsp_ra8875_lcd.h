#ifndef __BSP_RA8875_LCD_H
#define	__BSP_RA8875_LCD_H

#include "stm32f10x.h"
#include "bsp_usart1.h"

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

#define Bank1_LCD_RSL    ((u32)0x60000000)	      //��Һ������RS�������ӵ�A16Ϊ �͵�ƽ
#define Bank1_LCD_RSH    ((u32)0x60020000)       //��Һ������RS�������ӵ�A16Ϊ �ߵ�ƽ// A16 PD11

/*ѡ��LCDָ���Ĵ���*/
#define LCD_WR_RSH(val)      ((*(__IO u16 *) (Bank1_LCD_RSH)) = ((u16)val))   //��RSH��ַд��
#define LCD_WR_RSL(val)      ((*(__IO u16 *) (Bank1_LCD_RSL)) = ((u16)(val))) //��RSL��ַд��
#define LCD_RD_RSH()         (*(__IO u16 *) (Bank1_LCD_RSH))                   //��RSH��ַ����
#define LCD_RD_RSL()         (*(__IO u16 *) (Bank1_LCD_RSL))                   //��RSL��ַ���� 

/* ILI9341 ��RSΪ�͵�ƽʱ��ʾ�Ĵ������ߵ�ƽ��ʾ���� */
#define LCD_ILI9341_CMD(index)            LCD_WR_RSL(index)
#define LCD_ILI9341_Parameter(val)	       LCD_WR_RSH(val)
#define LCD_ILI9341_READ_DATA()           LCD_RD_RSH()

/*RA8875 ��RSΪ�ߵ�ƽʱ��ʾ�Ĵ������͵�ƽ��ʾ���ݣ���ILI9341�෴*/
#define LCD_RA8875_REG(index)            LCD_WR_RSH(index)                    
#define LCD_RA8875_WRITE_DATA(val)      LCD_WR_RSL(val)
#define LCD_RA8875_READ_DATA()           LCD_RD_RSL() 
#define LCD_RA8875_READ_STATUS()          LCD_RD_RSH()
#define LCD_RA8875_RAM_ADDR               Bank1_LCD_RSL


#define LCD_WIDTH     800
#define LCD_HEIGHT    480


  
#define LCD_DEBUG_ON          1
#define LCD_DEBUG_ARRAY_ON   1
#define LCD_DEBUG_FUNC_ON    1
   
   
// Log define
#define LCD_INFO(fmt,arg...)           printf("<<-LCD-INFO->> "fmt"\n",##arg)
#define LCD_ERROR(fmt,arg...)          printf("<<-LCD-ERROR->> "fmt"\n",##arg)
#define LCD_DEBUG(fmt,arg...)          do{\
                                         if(LCD_DEBUG_ON)\
                                         printf("<<-LCD-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                       }while(0)
#define LCD_DEBUG_ARRAY(array, num)    do{\
                                         int32_t i;\
                                         uint8_t* a = array;\
                                         if(LCD_DEBUG_ARRAY_ON)\
                                         {\
                                            printf("<<-LCD-DEBUG-ARRAY->>\n");\
                                            for (i = 0; i < (num); i++)\
                                            {\
                                                printf("%02x   ", (a)[i]);\
                                                if ((i + 1 ) %10 == 0)\
                                                {\
                                                    printf("\n");\
                                                }\
                                            }\
                                            printf("\n");\
                                        }\
                                       }while(0)
#define LCD_DEBUG_FUNC()               do{\
                                         if(LCD_DEBUG_FUNC_ON)\
                                         printf("<<-LCD-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)


/* ��������� */
#define BRIGHT_MAX		    255
#define BRIGHT_MIN		    0
#define BRIGHT_DEFAULT	200
#define BRIGHT_STEP		  5

/*
	LCD ��ɫ���룬CL_��Color�ļ�д
	16Bit�ɸ�λ����λ�� RRRR RGGG GGGB BBBB

	�����RGB �꽫24λ��RGBֵת��Ϊ16λ��ʽ��
	����windows�Ļ��ʳ��򣬵���༭��ɫ��ѡ���Զ�����ɫ�����Ի�õ�RGBֵ��

	�Ƽ�ʹ������ȡɫ���������㿴���Ľ�����ɫ��
*/
/*�����B��R�������߽�����*/
#define RGB(R,G,B)	(((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3))	/* ��8λR,G,Bת��Ϊ 16λRGB565��ʽ */
#define RGB565_R(x)  ((x >> 8) & 0xF8)
#define RGB565_G(x)  ((x >> 3) & 0xFC)
#define RGB565_B(x)  ((x << 3) & 0xF8)

enum
{
	CL_WHITE    = RGB(255,255,255),	/* ��ɫ */
	CL_BLACK    = RGB(  0,  0,  0),	/* ��ɫ */
	CL_RED      = RGB(255,	0,  0),	/* ��ɫ */
	CL_GREEN    = RGB(  0,255,  0),	/* ��ɫ */
	CL_BLUE     = RGB(  0,	0,255),	/* ��ɫ */
	CL_YELLOW   = RGB(255,255,  0),	/* ��ɫ */

	CL_GREY    = RGB( 98, 98, 98), 	/* ���ɫ */
	CL_GREY1		= RGB( 150, 150, 150), 	/* ǳ��ɫ */
	CL_GREY2		= RGB( 180, 180, 180), 	/* ǳ��ɫ */
	CL_GREY3		= RGB( 200, 200, 200), 	/* ��ǳ��ɫ */
	CL_GREY4		= RGB( 230, 230, 230), 	/* ��ǳ��ɫ */

	CL_BUTTON_GREY	= RGB( 220, 220, 220), /* WINDOWS ��ť�����ɫ */

	CL_MAGENTA      = 0xF81F,	/* ����ɫ�����ɫ */
	CL_CYAN         = 0x7FFF,	/* ����ɫ����ɫ */

	CL_BLUE1        = RGB(  0,  0, 240),		/* ����ɫ */
	CL_BLUE2        = RGB(  0,  0, 128),		/* ����ɫ */
	CL_BLUE3        = RGB(  68, 68, 255),		/* ǳ��ɫ1 */
	CL_BLUE4        = RGB(  0, 64, 128),		/* ǳ��ɫ1 */

	/* UI ���� Windows�ؼ�����ɫ */
	CL_BTN_FACE		  = RGB(236, 233, 216),	/* ��ť������ɫ(��) */
	CL_BOX_BORDER1	= RGB(172, 168,153),	/* �����������ɫ */
	CL_BOX_BORDER2	= RGB(255, 255,255),	/* �������Ӱ����ɫ */


	CL_MASK			    = 0x9999	/* ��ɫ���룬�������ֱ���͸�� */
};


enum
{
	RA_FONT_16 = 0,		/* RA8875 ���� 16���� */
	RA_FONT_24 = 1,		/* RA8875 ���� 24���� */
	RA_FONT_32 = 2		/* RA8875 ���� 32���� */
};

/* ���ַŴ���� */
enum
{
	RA_SIZE_X1 = 0,		/* ԭʼ��С */
	RA_SIZE_X2 = 1,		/* �Ŵ�2�� */
	RA_SIZE_X3 = 2,		/* �Ŵ�3�� */
	RA_SIZE_X4 = 3		/* �Ŵ�4�� */
};


void LCD_Init(void);

uint16_t RA8875_ReadID(void);

void Display_ON(void);
void Display_OFF(void);
void RA8875_SetBackLight(uint8_t _bright);
void RA8875_SetCursor(uint16_t _usX, uint16_t _usY);
void RA8875_SetReadCursor(uint16_t _usX, uint16_t _usY);
void RA8875_SetTextCursor(uint16_t _usX, uint16_t _usY);
void RA8875_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth);
void RA8875_QuitWinMode(void);
void RA8875_SetWriteDirection(uint8_t horizontal,uint16_t vertical);
void RA8875_Config(void);
void RA8875_DispAscii(uint16_t _usX, uint16_t _usY, char *_ptr);
void RA8875_ClrScr(uint16_t _usColor);
void RA8875_SetBackLight(uint8_t _bright);

void RA8875_SetFrontColor(uint16_t _usColor);
void RA8875_SetBackColor(uint16_t _usColor);
void RA8875_SetActiveWindow(uint16_t _usX1,uint16_t _usY1 ,uint16_t _usX2 ,uint16_t _usY2);
void RA8875_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor);
void RA8875_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor);
void RA8875_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void RA8875_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void RA8875_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void RA8875_FillCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void RA8875_SetFont(uint8_t _ucFontType, uint8_t _ucLineSpace, uint8_t _ucCharSpace);
void RA8875_SetTextZoom(uint8_t _ucHSize, uint8_t _ucVSize);
void RA8875_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor);
void RA8875_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr);



#endif /* __BSP_RA8875_LCD_H */
