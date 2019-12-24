#ifndef __BSP_RA8875_LCD_H
#define	__BSP_RA8875_LCD_H

#include "stm32f10x.h"

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

void LCD_Init(void);


#endif /* __BSP_RA8875_LCD_H */
