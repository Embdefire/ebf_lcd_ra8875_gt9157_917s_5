/**
  ******************************************************************************
  * @file    bsp_ra8875_lcd.c
  * @author  �ο�armflyҺ������
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ra8875��������������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 


#include "bsp_ra8875_lcd.h"

#define DEBUG_DELAY()




/* ���浱ǰ��ʾ���ڵ�λ�úʹ�С���⼸�������� RA8875_SetDispWin() �������� */
static uint16_t s_WinX = 0;
static uint16_t s_WinY = 0;
static uint16_t s_WinHeight = LCD_HEIGHT;
static uint16_t s_WinWidth = LCD_WIDTH;

uint16_t g_LcdHeight = LCD_HEIGHT;			/* ��ʾ���ֱ���-�߶� */
uint16_t g_LcdWidth = LCD_WIDTH;			/* ��ʾ���ֱ���-��� */


static uint16_t RA8875_ReadReg(uint16_t RegAddr);

void LCD_Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

 /**
  * @brief  ��ʼ������LcD��IO
  * @param  ��
  * @retval ��
  */
void LCD_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* ʹ��FSMCʱ��*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    
    /* ʹ��FSMC��Ӧ��Ӧ�ܽ�ʱ��*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE 
	                          | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG 
	                          | RCC_APB2Periph_GPIOF , ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    /* ����LCD��λ���ƹܽ�*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    

    
    /* ����FSMC���Ӧ��������,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10*/	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | 
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
		/* ����FSMC���Ӧ�Ŀ�����
		 * PD4-FSMC_NOE   :LCD-RD
		 * PD5-FSMC_NWE   :LCD-WR
		 * PG12-FSMC_NE4  :LCD-CS
		 * PE2-FSMC_A23   :LCD-DC
		*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
    GPIO_Init(GPIOG, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_Init(GPIOE, &GPIO_InitStructure);  
    

    /* ����LCDæµ��ѯ�ܽ�*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

}

 /**
  * @brief  LCD  FSMC ģʽ����
  * @param  ��
  * @retval ��
  */
void LCD_FSMC_Config(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p; 
    
    
    p.FSMC_AddressSetupTime = 0x02;	 //��ַ����ʱ��
    p.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ��
    p.FSMC_DataSetupTime = 0x05;		 //���ݽ���ʱ��
    p.FSMC_BusTurnAroundDuration = 0x00;
    p.FSMC_CLKDivision = 0x00;
    p.FSMC_DataLatency = 0x00;
    p.FSMC_AccessMode = FSMC_AccessMode_B;	 // һ��ʹ��ģʽB������LCD
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
//    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
		FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;  
    
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
    
    /* ʹ�� FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  
}

 /**
  * @brief  RA8875_WaitStatus�ȴ�ra8875������
  * @param  ��
  * @retval ��
  */
void RA8875_WaitStatus(void)
{
	while(!(GPIOG->IDR & GPIO_Pin_11 ));
}

/*
*********************************************************************************************************
*	�� �� ��: SPFD5420_ReadReg
*	����˵��: ��ȡLCD�������ļĴ�����ֵ��
*	��    ��:  
*			SPFD5420_Reg ���Ĵ�����ַ;
*			SPFD5420_RegValue : �Ĵ���ֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static uint16_t RA8875_ReadReg(uint16_t RegAddr)
{
	/* Write 16-bit Index (then Read Reg) */
	LCD_RA8875_REG (RegAddr);
	/* Read 16-bit Reg */
	return (LCD_RA8875_READ_DATA());
}

void RA8875_WriteReg(uint8_t RegAddr, uint8_t RegValue)
{
  LCD_RA8875_REG (RegAddr);
  LCD_RA8875_WRITE_DATA(RegValue);

}



/*
*********************************************************************************************************
*	�� �� ��: RA8875_ReadID
*	����˵��: ��ȡLCD����оƬID
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t RA8875_ReadID(void)
{
	return RA8875_ReadReg(0x00);
}


/*
*********************************************************************************************************
*	�� �� ��: RA8875_ReadStatus
*	����˵��: ��RA8875״̬�Ĵ���
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static uint8_t RA8875_ReadStatus(void)
{
	return  LCD_RA8875_READ_STATUS();;
}


/*
*********************************************************************************************************
*	�� �� ��: RA8875_SetCursor
*	����˵��: ����д�Դ�Ĺ��λ�ã�ͼ��ģʽ��
*	��    ��:  _usX : X����; _usY: Y����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_SetCursor(uint16_t _usX, uint16_t _usY)
{
	/* �����ڴ�д�������� ��ע��0x80-83 �ǹ��ͼ�ε����꡿ */
	RA8875_WriteReg(0x46, _usX);
	RA8875_WriteReg(0x47, _usX >> 8);
	RA8875_WriteReg(0x48, _usY);
	RA8875_WriteReg(0x49, _usY >> 8);
}


/*
*********************************************************************************************************
*	�� �� ��: RA8875_SetDispWin
*	����˵��: ������ʾ���ڣ����봰����ʾģʽ���ڴ�����ʾģʽ������д�Դ棬�����Զ����趨�����ڽ��е���
*	��    ��:
*		_usX : ˮƽ����
*		_usY : ��ֱ����
*		_usHeight: ���ڸ߶�
*		_usWidth : ���ڿ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth)
{

	uint16_t usTemp;

	/* ����ϵͳʾ��ͼ�� ��������
			 -----------------------------
			|(0,0)                        |
			|     --------->              |
			|         |                   |
			|         |                   |
			|         |                   |
			|         V                   |
			|     --------->              |
			|                    (800,480)|
			 -----------------------------

		���Ͻ����������, ɨ�跽���ȴ����ң��ٴ��ϵ��¡�

		�����Ҫ��������ʽ������Ҫ��������������߼������ת��
	*/

	RA8875_WriteReg(0x30, _usX);
    RA8875_WriteReg(0x31, _usX >> 8);

	RA8875_WriteReg(0x32, _usY);
    RA8875_WriteReg(0x33, _usY >> 8);

	usTemp = _usWidth + _usX - 1;
	RA8875_WriteReg(0x34, usTemp);
    RA8875_WriteReg(0x35, usTemp >> 8);

	usTemp = _usHeight + _usY - 1;
	RA8875_WriteReg(0x36, usTemp);
    RA8875_WriteReg(0x37, usTemp >> 8);

	RA8875_SetCursor(_usX, _usY);

	/* ���浱ǰ������Ϣ������Ժ�ɫ��������Ч��.
	����һ��������ͨ����ȡ0x30-0x37�Ĵ�����õ�ǰ���ڣ�����Ч�ʽϵ� */
	s_WinX = _usX;
	s_WinY = _usY;
	s_WinHeight = _usHeight;
	s_WinWidth = _usWidth;
}


 /**
  * @brief  RA8875_SetDirection����Һ��GRAMд��ķ���(��ʾbmp��ʽͼƬ��Ҫ����)
  * @param  
  *   @arg  horizontal|vertical   00 �����ң�Ȼ���ϵ���
                                  01 ���ҵ���Ȼ���ϵ���
                                  10 ���ϵ��£�Ȼ������
                                  11 ���µ��ϣ�Ȼ������
  * @retval ��

	 ����ϵͳʾ��ͼ�� ��������
			 -----------------------------
			|(0,0)                       |
			|     ----����- --->        |
			|         |                   |
      |         |                   |
			|         ��                  |
			|         ��                  |
      |         ��                  |
      |         | 
			|         V                   |
			|     --------->              |
			|                    (800,480)|
			 -----------------------------
  */
void RA8875_SetWriteDirection(uint8_t horizontal,uint16_t vertical)
{
  uint8_t temp;
  
  /*��ȡԭʼ����*/
  temp = RA8875_ReadReg(0x40);  
  
  /*����ˮƽ����*/
	RA8875_WriteReg(0x40, horizontal?(temp|(1<<2)):(temp&~(1<<2)));
     
  /*���ô�ֱ����*/
  temp = RA8875_ReadReg(0x40);

  RA8875_WriteReg(0x40, vertical?(temp|(1<<3)):(temp&~(1<<3)));
    
}


void RA8875_Config(void)
{

		/* ��ʼ��PLL.  ����Ƶ��Ϊ25M */
		LCD_RA8875_REG(0x88);
		LCD_Delay(10);		/* �����ӳ� */
		LCD_RA8875_WRITE_DATA(12);	/* PLLDIVM [7] = 0 ;  PLLDIVN [4:0] = 10 */

	  LCD_Delay(10000);
  

		LCD_RA8875_REG(0x89);
		LCD_Delay(10000);		/* �ӳ�1us */
		LCD_RA8875_WRITE_DATA(2);	/* PLLDIVK[2:0] = 2, ����4 */

  

		/* RA8875 ���ڲ�ϵͳƵ�� (SYS_CLK) �ǽ���񵴵�·��PLL ��·��������Ƶ�ʼ��㹫ʽ���� :
		  SYS_CLK = FIN * ( PLLDIVN [4:0] +1 ) / ((PLLDIVM+1 ) * ( 2^PLLDIVK [2:0] ))
		          = 25M * (12 + 1) / ((0 + 1) * (2 ^ 2))
				  = 81.25MHz
		*/

		/* REG[88h]��REG[89h]���趨��Ϊ��֤PLL ����ȶ�����ȴ�һ�Ρ���Ƶʱ�䡹(< 100us)��*/
	    LCD_Delay(10000);
      
      /*
			����ϵͳ���ƼĴ����� ����pdf ��18ҳ:

			bit3:2 ɫ������趨 (Color Depth Setting)
				00b : 8-bpp ��ͨ��TFT �ӿڣ� i.e. 256 ɫ��
				1xb : 16-bpp ��ͨ��TFT �ӿڣ� i.e. 65K ɫ��	 ��ѡ�����

			bit1:0 MCUIF ѡ��
				00b : 8-λMCU �ӿڡ�
				1xb : 16-λMCU �ӿڡ� ��ѡ�����
		*/
    
    RA8875_WriteReg(0x10, (1 <<3 ) | (1 << 1));	/* ����16λMCU���ڣ�65Kɫ */
  
  

		/* REG[04h] Pixel Clock Setting Register (PCSR)
			bit7  PCLK Inversion
				0 : PDAT ����PCLK ��Ե���� (Rising Edge) ʱ��ȡ����
				1 : PDAT ����PCLK ��Ե�½� (Falling Edge) ʱ��ȡ����
			bit1:0 PCLK Ƶ�������趨
				Pixel Clock ,PCLK Ƶ�������趨��
				00b: PCLK Ƶ������= ϵͳƵ�����ڡ�
				01b: PCLK Ƶ������= 2 ����ϵͳƵ�����ڡ�
				10b: PCLK Ƶ������= 4 ����ϵͳƵ�����ڡ�
				11b: PCLK Ƶ������= 8 ����ϵͳƵ�����ڡ�
		*/
	    RA8875_WriteReg(0x04, 0x81);
	    LCD_Delay(10000);
      
      
     
		/* OTD9960 & OTA7001 ���� */
		RA8875_WriteReg(0x14, 0x63);
		RA8875_WriteReg(0x15, 0x00);
		RA8875_WriteReg(0x16, 0x03);
		RA8875_WriteReg(0x17, 0x03);
		RA8875_WriteReg(0x18, 0x0B);
		RA8875_WriteReg(0x19, 0xDF);
		RA8875_WriteReg(0x1A, 0x01);
		RA8875_WriteReg(0x1B, 0x1F);
		RA8875_WriteReg(0x1C, 0x00);
		RA8875_WriteReg(0x1D, 0x16);
		RA8875_WriteReg(0x1E, 0x00);
		RA8875_WriteReg(0x1F, 0x01);
    
                       

    
    /* ����TFT���� DISP  ����Ϊ�ߣ�ʹ�����. Ұ��TFTģ���DISP�������ӵ�RA8875оƬ��GP0X�� */
    RA8875_WriteReg(0xC7, 0x01);	/* DISP = 1 */

    /* LCD ��ʾ/�ر�Ѷ�� (LCD Display on) */
    RA8875_WriteReg(0x01, 0x80);

    /* 	REG[40h] Memory Write Control Register 0 (MWCR0)

      Bit 7	��ʾģʽ�趨
        0 : ��ͼģʽ��
        1 : ����ģʽ��

      Bit 6	����д����/�ڴ�д�����趨
        0 : �趨����/�ڴ�д����Ϊ����ʾ��
        1 : �趨����/�ڴ�д����Ϊ��ʾ��

      Bit 5	����д����/�ڴ�д������˸�趨
        0 : �α겻��˸��
        1 : �α���˸��

      Bit 4   NA

      Bit 3-2  ��ͼģʽʱ���ڴ�д�뷽��
        00b : �� -> �ң�Ȼ���� -> �¡�
        01b : �� -> ��Ȼ���� -> �¡�
        10b : �� -> �£�Ȼ���� -> �ҡ�
        11b : �� -> �ϣ�Ȼ���� -> �ҡ�

      Bit 1 	�ڴ�д�����Զ����ӹ����趨
        0 : ���ڴ�д��ʱ���λ���Զ���һ��
        1 : ���ڴ�д��ʱ���λ�ò����Զ���һ��

      Bit 0 �ڴ��ȡ����Զ����ӹ����趨
        0 : ���ڴ��ȡʱ���λ���Զ���һ��
        1 : ���ڴ��ȡʱ���λ�ò����Զ���һ��
    */
    RA8875_WriteReg(0x40, 0x00);	/* ѡ���ͼģʽ */


    /* 	REG[41h] Memory Write Control Register1 (MWCR1)
      д��Ŀ��λ�ã�ѡ��ͼ��1
    */
    RA8875_WriteReg(0x41, 0x01);	/* ѡ���ͼģʽ, Ŀ��ΪCGRAM */

    RA8875_SetDispWin(0, 0, g_LcdHeight, g_LcdWidth);

}


/*
*********************************************************************************************************
*	�� �� ��: BTE_SetTarBlock
*	����˵��: ����RA8875 BTEĿ�������Լ�Ŀ��ͼ��
*	��    ��:
*			uint16_t _usX : ˮƽ�������
*			uint16_t _usY : ��ֱ�������
*			uint16_t _usHeight : ����߶�
*			uint16_t _usWidth : ������
*			uint8_t _ucLayer �� 0 ͼ��1�� 1 ͼ��2
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BTE_SetTarBlock(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t _ucLayer)
{
	/* ����������� */
	RA8875_WriteReg(0x58, _usX);
	RA8875_WriteReg(0x59, _usX >> 8);

	RA8875_WriteReg(0x5A, _usY);
	if (_ucLayer == 0)	/* ͼ��2 */
	{
		RA8875_WriteReg(0x5B, _usY >> 8);
	}
	else
	{
		RA8875_WriteReg(0x5B, (1 << 7) | (_usY >> 8));	/* Bit7 ��ʾͼ�㣬 0 ͼ��1�� 1 ͼ��2*/
	}

	/* ���������� */
	RA8875_WriteReg(0x5C, _usWidth);
	RA8875_WriteReg(0x5D, _usWidth >> 8);

	/* ��������߶� */
	RA8875_WriteReg(0x5E, _usHeight);
	RA8875_WriteReg(0x5F, _usHeight >> 8);
}

/*
*********************************************************************************************************
*	�� �� ��: BTE_SetOperateCode
*	����˵��: �趨BTE ������͹�դ������
*	��    ��: _ucOperate : ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BTE_SetOperateCode(uint8_t _ucOperate)
{
	/*  �趨BTE ������͹�դ������  */
	RA8875_WriteReg(0x51, _ucOperate);
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_SetFrontColor
*	����˵��: �趨ǰ��ɫ
*	��    ��: ��ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_SetFrontColor(uint16_t _usColor)
{
	RA8875_WriteReg(0x63, (_usColor & 0xF800) >> 11);	/* R5  */
	RA8875_WriteReg(0x64, (_usColor & 0x07E0) >> 5);	/* G6 */
	RA8875_WriteReg(0x65, (_usColor & 0x001F));			/* B5 */
	
}


/**
 * @brief  RA8875_SetActiveWindow���û��������
 * @param  _usX1 �������ʼ�����xֵ
 * @param  _usY1 �������ʼ�����yֵ
 * @param  _usY2 ����ڽ��������xֵ
 * @param  _usY2 ����ڽ��������yֵ
 * @retval ��
 */
void RA8875_SetActiveWindow(uint16_t _usX1,uint16_t _usY1 ,uint16_t _usX2 ,uint16_t _usY2)
{
	uint8_t temp;
    //setting active window X
	temp=_usX1;   
    LCD_RA8875_REG(0x30);//HSAW0
	LCD_RA8875_WRITE_DATA(temp);
	temp=_usX1>>8;   
    LCD_RA8875_REG(0x31);//HSAW1	   
	LCD_RA8875_WRITE_DATA(temp);

	temp=_usX2;   
    LCD_RA8875_REG(0x34);//HEAW0
	LCD_RA8875_WRITE_DATA(temp);
	temp=_usX2>>8;   
    LCD_RA8875_REG(0x35);//HEAW1	   
	LCD_RA8875_WRITE_DATA(temp);

    //setting active window Y
	temp=_usY1;   
    LCD_RA8875_REG(0x32);//VSAW0
	LCD_RA8875_WRITE_DATA(temp);
	temp=_usY1>>8;   
    LCD_RA8875_REG(0x33);//VSAW1	   
	LCD_RA8875_WRITE_DATA(temp);

	temp=_usY2;   
    LCD_RA8875_REG(0x36);//VEAW0
	LCD_RA8875_WRITE_DATA(temp);
	temp=_usY2>>8;   
    LCD_RA8875_REG(0x37);//VEAW1	   
	LCD_RA8875_WRITE_DATA(temp);
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_SetBackColor
*	����˵��: �趨����ɫ
*	��    ��: ��ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_SetBackColor(uint16_t _usColor)
{

	RA8875_WriteReg(0x60, (_usColor & 0xF800) >> 11);	/* R5  */
	RA8875_WriteReg(0x61, (_usColor & 0x07E0) >> 5);	/* G6 */
	RA8875_WriteReg(0x62, (_usColor & 0x001F));			/* B5 */
	
}


/*
*********************************************************************************************************
*	�� �� ��: BTE_Start
*	����˵��: ����BTE����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BTE_Start(void)
{
	
	/* RA8875_WriteReg(0x50, 0x80);  ����ʹ�������������Ϊ�ڲ��Ѿ������� s_ucRA8875Busy ��־ */
	LCD_RA8875_REG(0x50);	/* ���üĴ�����ַ */
	LCD_RA8875_WRITE_DATA(0x80);	/* д��Ĵ���ֵ */
}

/*
*********************************************************************************************************
*	�� �� ��: BTE_Wait
*	����˵��: �ȴ�BTE��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BTE_Wait(void)
{
	while ((RA8875_ReadStatus() & 0x40) == 0x40);
	
}



/*
*********************************************************************************************************
*	�� �� ��: RA8875_WaitBusy
*	����˵��: �ȴ�RA8875����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_WaitBusy(void)
{
	while ((RA8875_ReadStatus() & 0x80) == 0x80);
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_Layer1Visable
*	����˵��: RA8875 ͼ��1��ʾ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_Layer1Visable(void)
{
	/* 0x52 �Ĵ����� Bit2:0
		ͼ����ʾģʽ
		000b : ֻ��ͼ��1 ��ʾ��
		001b : ֻ��ͼ��2 ��ʾ��
		010b : ��ʾͼ��1 ��ͼ��2 �Ľ���/����ģʽ��
		011b : ��ʾͼ��1 ��ͼ��2 ��ͨ͸ģʽ��
		100b : Boolean OR��
		101b : Boolean AND��
		110b : ��������ģʽ (Floating window mode)��
		111b :������
	*/
	RA8875_WriteReg(0x52, RA8875_ReadReg(0x52) & 0xF8);	/* ֻ��ͼ��1 ��ʾ */
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_Layer2Visable
*	����˵��: ֻ��ʾͼ��2
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_Layer2Visable(void)
{
	/* 0x52 �Ĵ����� Bit2:0
		ͼ����ʾģʽ
		000b : ֻ��ͼ��1 ��ʾ��
		001b : ֻ��ͼ��2 ��ʾ��
		010b : ��ʾͼ��1 ��ͼ��2 �Ľ���/����ģʽ��
		011b : ��ʾͼ��1 ��ͼ��2 ��ͨ͸ģʽ��
		100b : Boolean OR��
		101b : Boolean AND��
		110b : ��������ģʽ (Floating window mode)��
		111b :������
	*/
	RA8875_WriteReg(0x52, (RA8875_ReadReg(0x52) & 0xF8) | 0x01);	/* ֻ��ͼ��2 ��ʾ */
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_DispOn
*	����˵��: ����ʾ
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_DispOn(void)
{
	RA8875_WriteReg(0x01, 0x80);
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_DispOff
*	����˵��: �ر���ʾ
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_DispOff(void)
{
	RA8875_WriteReg(0x01, 0x00);
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_Sleep
*	����˵��: RA8875����˯��ģʽ��ͬʱ�ر���ʾ����
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_Sleep(void)
{
	RA8875_SetBackLight(0);		/* �رձ��� */

	/* ����TFT���� DISP  ����Ϊ�ߣ�ʹ�����. Ұ��TFTģ���DISP�������ӵ�RA8875оƬ��GP0X�� */
	RA8875_WriteReg(0xC7, 0x00);	/* DISP = 0  */

	RA8875_WriteReg(0x01, 0x01);
	RA8875_WriteReg(0x01, 0x00);

	RA8875_WriteReg(0x01, 0x02);	/* RA8875 Sleep */
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_PutPixel
*	����˵��: ��1������
*	��    ��:
*			_usX,_usY : ��������
*			_usColor  ��������ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor)
{
	/* չ�� 	RA8875_SetCursor(_usX, _usY); ���� */
	

	LCD_RA8875_REG(0x46); LCD_RA8875_WRITE_DATA(_usX);
	LCD_RA8875_REG(0x47); LCD_RA8875_WRITE_DATA(_usX >> 8);
	LCD_RA8875_REG(0x48); LCD_RA8875_WRITE_DATA(_usY);
	LCD_RA8875_REG(0x49); LCD_RA8875_WRITE_DATA(_usY >> 8);

	LCD_RA8875_REG(0x02); 		/* �����趨RA8875 �����ڴ�(DDRAM��CGRAM)��ȡ/д��ģʽ */
	LCD_RA8875_WRITE_DATA(_usColor);

	
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_GetPixel
*	����˵��: ��ȡ1������
*	��    ��:
*			_usX,_usY : ��������
*			_usColor  ��������ɫ
*	�� �� ֵ: RGB��ɫֵ
*********************************************************************************************************
*/
uint16_t RA8875_GetPixel(uint16_t _usX, uint16_t _usY)
{
	uint16_t usRGB;

	RA8875_WriteReg(0x40, (1 << 0));	/* ����Ϊ��ͼģʽ����ȡ��겻�Զ���1 */

	RA8875_SetReadCursor(_usX, _usY);	/* ���ö�ȡ���λ�� */

	

	LCD_RA8875_REG(0x02);
	usRGB = LCD_RA8875_READ_DATA();	/* ��1�ζ�ȡ���ݶ��� */
	usRGB = LCD_RA8875_READ_DATA();
	usRGB = LCD_RA8875_READ_DATA();

	

	return usRGB;
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_ClrScr
*	����˵��: �����������ɫֵ����.RA8875֧��Ӳ����ɫ��䡣�ú������Ե�ǰ�������ʾ���ڽ�������. ��ʾ
*			 ���ڵ�λ�úʹ�С�� RA8875_SetDispWin() ������������
*	��    ��:  _usColor : ����ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_ClrScr(uint16_t _usColor)
{
	/* Ҳ����ͨ��0x30-0x37�Ĵ�����û�õ�ǰ�������ʾ���� */

	/* ��ɫ��������, ����pdf ��162ҳ
	�˹���ʹ���ڽ�ѡ���ض�������������ǉ\�����ĳ��ǰ��ɫ��R8875 ����ĵ�ɫ�趨ΪBTE ǰ��ɫ��

	�������裺
		1. �趨Ŀ��ͼ���λ�� REG[58h], [59h], [5Ah], [5Bh]
		2. �趨BTE ��Ⱥ͸߶� REG[5Ch], [5Dh], [5Eh], [5Fh]
		3. �趨BTE ������͹�դ������  REG[51h] Bit[3:0] = 0Ch
		4. �趨ǰ��ɫ  REG[63h], [64h], [65h]
		5. ����BTE ����  REG[50h] Bit7 = 1
		6. ���״̬������ STSR Bit6��ȷ��BTE �Ƿ����
	*/
	BTE_SetTarBlock(s_WinX, s_WinY, s_WinHeight, s_WinWidth, 0);	/* ����BTEλ�úͿ�ȸ߶��Լ�Ŀ��ͼ�㣨0��1�� */
	BTE_SetOperateCode(0x0C);		/* �趨BTE ������͹�դ������  REG[51h] Bit[3:0] = 0Ch */
	RA8875_SetFrontColor(_usColor);	/* ����BTEǰ��ɫ */
	BTE_Start();					/* ����BTE ���� */
	BTE_Wait();						/* �ȴ��������� */
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_DrawBMP
*	����˵��: ��LCD����ʾһ��BMPλͼ��λͼ����ɨ����򣺴����ң����ϵ���
*	��    ��:
*			_usX, _usY : ͼƬ������
*			_usHeight  ��ͼƬ�߶�
*			_usWidth   ��ͼƬ���
*			_ptr       ��ͼƬ����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr)
{
	uint32_t index = 0;
	const uint16_t *p;

	/* ����ͼƬ��λ�úʹ�С�� ��������ʾ���� */
	RA8875_SetDispWin(_usX, _usY, _usHeight, _usWidth);

	

	LCD_RA8875_REG(0x02); 		/* ׼����д�Դ� */

	p = _ptr;
	for (index = 0; index < _usHeight * _usWidth; index++)
	{
		/*
			armfly : �����Ż�, �����͵�չ��
			RA8875_WriteRAM(_ptr[index]);

			�˴��ɿ�����DMA����
		*/
		LCD_RA8875_WRITE_DATA(*p++);
	}
	

	/* �˳����ڻ�ͼģʽ */
	RA8875_QuitWinMode();
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_StartDirectDraw
*	����˵��: ��ʼֱ�ӻ�ͼģʽ.  ��֧��SPIģʽ����֧��8080ģʽ
*	��    ��:
*			_usX, _usY : ��ʾ���ڵ�����
*			_usHeight  : ��ʾ���ڵĸ߶�
*			_usWidth   : ��ʾ���ڵĿ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_StartDirectDraw(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth)
{
	/* ����ͼƬ��λ�úʹ�С�� ��������ʾ���� */
	RA8875_SetDispWin(_usX, _usY, _usHeight, _usWidth);

	

	LCD_RA8875_REG(0x02); 		/* ׼����д�Դ� */

	/* ֮��Ӧ�ó������ֱ�ӻ�ͼ */
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_GetDispMemAddr
*	����˵��: ����Դ��ַ��
*	��    ��: ��
*	�� �� ֵ: �Դ��ַ
*********************************************************************************************************
*/
uint32_t RA8875_GetDispMemAddr(void)
{
	return LCD_RA8875_RAM_ADDR;
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_QuitDirectDraw
*	����˵��: �˳���ͼģʽ.  ��֧��SPIģʽ����֧��8080ģʽ
*	��    ��:
*			_usX, _usY : ��ʾ���ڵ�����
*			_usHeight  : ��ʾ���ڵĸ߶�
*			_usWidth   : ��ʾ���ڵĿ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_QuitDirectDraw(void)
{
	

	/* �˳����ڻ�ͼģʽ */
	RA8875_QuitWinMode();
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_DrawLine
*	����˵��: ����RA8875��Ӳ�����ܣ���2��仭һ��ֱ�ߡ�
*	��    ��:
*			_usX1, _usY1 ����ʼ������
*			_usX2, _usY2 ����ֹ��Y����
*			_usColor     ����ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor)
{
	/* pdf ��131ҳ
		RA8875 ֧��ֱ�߻�ͼ���ܣ���ʹ�����Լ��׻���ٵ�MCU �Ϳ�����TFT ģ���ϻ�ֱ�ߡ�����
		��ֱ�ߵ���ʼ��REG[91h~94h] �������REG[95h~98h]��ֱ�ߵ���ɫREG[63h~65h]��Ȼ����
		����ͼ�趨REG[90h] Bit4 = 0, Bit0=0 ��REG[90h] Bit7 = 1��RA8875 �ͻὫֱ�ߵ�ͼ��д��
		DDRAM����Ե���TFT ģ���ϾͿ�����ʾ������ֱ�ߡ�
	*/

	if ((_usX1 == _usX2) && (_usY1 == _usY2))
	{
		RA8875_PutPixel(_usX1, _usY1, _usColor);
		return;
	}
	
	/* ����������� */
	RA8875_WriteReg(0x91, _usX1);
	RA8875_WriteReg(0x92, _usX1 >> 8);
	RA8875_WriteReg(0x93, _usY1);
	RA8875_WriteReg(0x94, _usY1 >> 8);

	/* �����յ����� */
	RA8875_WriteReg(0x95, _usX2);
	RA8875_WriteReg(0x96, _usX2 >> 8);
	RA8875_WriteReg(0x97, _usY2);
	RA8875_WriteReg(0x98, _usY2 >> 8);

	RA8875_SetFrontColor(_usColor);	/* ����ֱ�ߵ���ɫ */

	
	RA8875_WriteReg(0x90, (1 << 7) | (0 << 4) | (0 << 0));	/* ��ʼ��ֱ�� */
//	while (RA8875_ReadReg(0x90) & (1 << 7));				/* �ȴ����� */
	RA8875_WaitBusy();
	
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_DrawRect
*	����˵��: ����RA8875Ӳ�����ܻ��ƾ���
*	��    ��:
*			_usX,_usY���������Ͻǵ�����
*			_usHeight �����εĸ߶�
*			_usWidth  �����εĿ��
*			_usColor  ����ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	/*
		RA8875 ֧�ַ��λ�ͼ���ܣ���ʹ�����Լ��׻���ٵ�MCU �Ϳ�����TFT ģ���ϻ����Ρ�����
	�����ε���ʼ��REG[91h~94h]�������REG[95h~98h]�����ε���ɫREG[63h~65h]��Ȼ����
	����ͼ�趨REG[90h] Bit4=1, Bit0=0 ��REG[90h] Bit7 = 1��RA8875 �ͻὫ���ε�ͼ��д��
	DDRAM����Ե���TFT ģ���ϾͿ�����ʾ�����ķ��Ρ����趨REG[90h] Bit5 = 1����ɻ���һ
	ʵ�ķ��� (Fill)

	 ---------------->---
	|(_usX��_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/

	/* ����������� */
	RA8875_WriteReg(0x91, _usX);
	RA8875_WriteReg(0x92, _usX >> 8);
	RA8875_WriteReg(0x93, _usY);
	RA8875_WriteReg(0x94, _usY >> 8);

	/* �����յ����� */
	RA8875_WriteReg(0x95, _usX + _usWidth - 1);
	RA8875_WriteReg(0x96, (_usX + _usWidth - 1) >> 8);
	RA8875_WriteReg(0x97, _usY + _usHeight - 1);
	RA8875_WriteReg(0x98, (_usY + _usHeight - 1) >> 8);

	RA8875_SetFrontColor(_usColor);	/* ������ɫ */

	
	RA8875_WriteReg(0x90, (1 << 7) | (0 << 5) | (1 << 4) | (0 << 0));	/* ��ʼ������  */
//	while (RA8875_ReadReg(0x90) & (1 << 7));							/* �ȴ����� */
	RA8875_WaitBusy();
	
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_FillRect
*	����˵��: ����RA8875Ӳ���������һ������Ϊ��ɫ
*	��    ��:
*			_usX,_usY���������Ͻǵ�����
*			_usHeight �����εĸ߶�
*			_usWidth  �����εĿ��
*			_usColor  ����ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	/*
		RA8875 ֧�ַ��λ�ͼ���ܣ���ʹ�����Լ��׻���ٵ�MCU �Ϳ�����TFT ģ���ϻ����Ρ�����
	�����ε���ʼ��REG[91h~94h]�������REG[95h~98h]�����ε���ɫREG[63h~65h]��Ȼ����
	����ͼ�趨REG[90h] Bit4=1, Bit0=0 ��REG[90h] Bit7 = 1��RA8875 �ͻὫ���ε�ͼ��д��
	DDRAM����Ե���TFT ģ���ϾͿ�����ʾ�����ķ��Ρ����趨REG[90h] Bit5 = 1����ɻ���һ
	ʵ�ķ��� (Fill)

	 ---------------->---
	|(_usX��_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/

	/* ����������� */
	RA8875_WriteReg(0x91, _usX);
	RA8875_WriteReg(0x92, _usX >> 8);
	RA8875_WriteReg(0x93, _usY);
	RA8875_WriteReg(0x94, _usY >> 8);

	/* �����յ����� */
	RA8875_WriteReg(0x95, _usX + _usWidth - 1);
	RA8875_WriteReg(0x96, (_usX + _usWidth - 1) >> 8);
	RA8875_WriteReg(0x97, _usY + _usHeight - 1);
	RA8875_WriteReg(0x98, (_usY + _usHeight - 1) >> 8);

	RA8875_SetFrontColor(_usColor);	/* ������ɫ */

	
	RA8875_WriteReg(0x90, (1 << 7) | (1 << 5) | (1 << 4) | (0 << 0));	/* ��ʼ������  */
//	while (RA8875_ReadReg(0x90) & (1 << 7));							/* �ȴ����� */
	RA8875_WaitBusy();
	
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_DrawCircle
*	����˵��: ����һ��Բ���ʿ�Ϊ1������
*	��    ��:
*			_usX,_usY  ��Բ�ĵ�����
*			_usRadius  ��Բ�İ뾶
*			_usColor  ����ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor)
{
	/*
		RA8875 ֧��Բ�λ�ͼ���ܣ���ʹ�����Լ��׻���ٵ�MCU �Ϳ�����TFT ģ���ϻ�Բ�����趨
		Բ�����ĵ�REG[99h~9Ch]��Բ�İ뾶REG[9Dh]��Բ����ɫREG[63h~65h]��Ȼ��������ͼ
		REG[90h] Bit6 = 1��RA8875 �ͻὫԲ��ͼ��д��DDRAM����Ե���TFT ģ���ϾͿ�����ʾ��
		����Բ�����趨REG[90h] Bit5 = 1����ɻ���һʵ��Բ (Fill)�����趨REG[90h] Bit5 = 0�����
		��������Բ (Not Fill
	*/
	/* ����Բ�İ뾶 */
	if (_usRadius > 255)
	{
		return;
	}

	/* ����Բ������ */
	RA8875_WriteReg(0x99, _usX);
	RA8875_WriteReg(0x9A, _usX >> 8);
	RA8875_WriteReg(0x9B, _usY);
	RA8875_WriteReg(0x9C, _usY >> 8);

	RA8875_WriteReg(0x9D, _usRadius);	/* ����Բ�İ뾶 */

	RA8875_SetFrontColor(_usColor);	/* ������ɫ */

	
	RA8875_WriteReg(0x90, (1 << 6) | (0 << 5));				/* ��ʼ��Բ, �����  */
//	while (RA8875_ReadReg(0x90) & (1 << 6));				/* �ȴ����� */
	RA8875_WaitBusy();
	
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_FillCircle
*	����˵��: ���һ��Բ
*	��    ��:
*			_usX,_usY  ��Բ�ĵ�����
*			_usRadius  ��Բ�İ뾶
*			_usColor   ����ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_FillCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor)
{
	/*
		RA8875 ֧��Բ�λ�ͼ���ܣ���ʹ�����Լ��׻���ٵ�MCU �Ϳ�����TFT ģ���ϻ�Բ�����趨
		Բ�����ĵ�REG[99h~9Ch]��Բ�İ뾶REG[9Dh]��Բ����ɫREG[63h~65h]��Ȼ��������ͼ
		REG[90h] Bit6 = 1��RA8875 �ͻὫԲ��ͼ��д��DDRAM����Ե���TFT ģ���ϾͿ�����ʾ��
		����Բ�����趨REG[90h] Bit5 = 1����ɻ���һʵ��Բ (Fill)�����趨REG[90h] Bit5 = 0�����
		��������Բ (Not Fill
	*/
	/* ����Բ�İ뾶 */
	if (_usRadius > 255)
	{
		return;
	}

	/* ����Բ������ */
	RA8875_WriteReg(0x99, _usX);
	RA8875_WriteReg(0x9A, _usX >> 8);
	RA8875_WriteReg(0x9B, _usY);
	RA8875_WriteReg(0x9C, _usY >> 8);

	RA8875_WriteReg(0x9D, _usRadius);	/* ����Բ�İ뾶 */

	RA8875_SetFrontColor(_usColor);	/* ������ɫ */

	
	RA8875_WriteReg(0x90, (1 << 6) | (1 << 5));				/* ��ʼ��Բ, ���  */
//	while (RA8875_ReadReg(0x90) & (1 << 6));				/* �ȴ����� */
	RA8875_WaitBusy();
	
}


/*
*********************************************************************************************************
*	�� �� ��: RA8875_SetFont (�����������ò�����)
*	����˵��: �ı�ģʽ�������������塢�о���־�
*	��    ��:
*			_ucFontType : ��������: RA_FONT_16, RA_FONT_24, RA_FONT_32
*			_ucLineSpace : �о࣬���ص�λ (0-31)
*			_ucCharSpace : �־࣬���ص�λ (0-63)
*
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_SetFont(uint8_t _ucFontType, uint8_t _ucLineSpace, uint8_t _ucCharSpace)
{
	/*
		[29H]������ģʽ�£������趨���ּ���о� (��λ: ����) ��
		ֻ�е�5��bit��Ч��0-31
	*/
	if (_ucLineSpace >31)
	{
		_ucLineSpace = 31;
	}
	RA8875_WriteReg(0x29, _ucLineSpace);

	/*
		[2EH] �����ַ���ࣨ���ص�λ��0-63���������壨16*16��24*24��32*32��
	*/
	if (_ucCharSpace > 63)
	{
		_ucCharSpace = 63;
	}
	if (_ucFontType > RA_FONT_32)
	{
		_ucFontType = RA_FONT_16;
	}
	RA8875_WriteReg(0x2E, (_ucCharSpace & 0x3F) | (_ucFontType << 6));
}


/*
*********************************************************************************************************
*	�� �� ��: RA8875_SetFontZoom
*	����˵��: �ı�ģʽ���������ֵķŴ�ģʽ��1X,2X,3X, 4X
*	��    ��:
*			_ucHSize : ����ˮƽ�Ŵ�����0-3 �ֱ��Ӧ X1��X2��X3��X4
*			_ucVSize : ���ִ��÷Ŵ�����0-3 �ֱ��Ӧ X1��X2��X3��X4
*		�������������ö�٣�RA_SIZE_X1��RA_SIZE_X2��RA_SIZE_X3��RA_SIZE_X4
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_SetTextZoom(uint8_t _ucHSize, uint8_t _ucVSize)
{
	/*
		pdf ��22ҳ		[22H]
		bit7 ���ڶ��룬һ�㲻�ã�ȱʡ��0
		bit6 ����ͨ͸��һ�㲻�ã�ȱʡ��0
		bit4 ������ת90����һ�㲻�ã�ȱʡ��0
		bit3-2 ˮƽ�Ŵ���
		bit1-0 ��ֱ�Ŵ���
	*/
	RA8875_WriteReg(0x22, ((_ucHSize & 0x03) << 2) | ( _ucVSize & 0x03));
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_DispAscii
*	����˵��: ��ʾASCII�ַ������ַ�����������RA8875�ڲ�ROM�е�ASCII�ַ�����8*16������֧�ֺ���.
*			������ɫ��������ɫ���Ƿ�ͨ͸�������ĺ�����������
*			void RA8875_SetFrontColor(uint16_t _usColor);
*			void RA8875_SetBackColor(uint16_t _usColor);
*	��    ��:  _usX, _usY : ��ʾλ�ã����꣩
*			 _ptr : AascII�ַ�������0����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_DispAscii(uint16_t _usX, uint16_t _usY, char *_ptr)
{
	/*
		RA8875 �ڽ�8x16 ���ASCII ����ROM���ṩʹ���߸�����ķ�ʽ���ض����� (Code) ������
		�֡��ڽ����ּ�֧��ISO/IEC 8859-1~4 �����׼�����⣬ʹ���߿���͸��REG[60h~62h] ѡ��
		����ǰ����ɫ���Լ�͸��REG[63h~65h] ѡ�񱳾���ɫ.

		ISO/IEC 8859-1���ֳ�Latin-1����ŷ���ԡ����ǹ��ʱ�׼����֯��ISO/IEC 8859�ĵ�һ��8λ�ַ�����
		����ASCIIΪ�������ڿ��õ�0xA0-0xFF�ķ�Χ�ڣ�����192����ĸ�����ţ����Թ�ʹ�ñ������ŵ�������ĸ����ʹ�á�

		ISO/IEC 8859-2 Latin-2����ŷ���ԡ����ǹ��ʱ�׼����֯��ISO/IEC 8859������һ��8λ�ַ��� .
		ISO/IEC 8859-3 ��ŷ�����ַ���
		ISO/IEC 8859-4 ��ŷ�����ַ���
	*/

	/*
	(1) Text mode  REG[40h] bit7=1
	(2) Internal Font ROM Select   REG[21h] bit7=0, bit5=0
	(3) Font foreground and background color Select  REG[63h~65h], REG[60h~62h]
	(4) Write the font Code  CMD_WR[02h]    DATA_WR[font_code]
	*/

	RA8875_SetTextCursor(_usX, _usY);

	RA8875_WriteReg(0x40, (1 << 7));	/* ����Ϊ�ı�ģʽ */

	/* ѡ��CGROM font; ѡ���ڲ�CGROM; �ڲ�CGROM ����ѡ��ISO/IEC 8859-1. */
	RA8875_WriteReg(0x2F, 0x00);
	RA8875_WriteReg(0x21, (0 << 7) | (0 << 5) | (0 << 1) | (0 << 0));

	


	LCD_RA8875_REG(0x02); 			/* �����趨RA8875 �����ڴ�(DDRAM��CGRAM)��ȡ/д��ģʽ */

	/* ��ʼѭ�������ַ� */
	while (*_ptr != 0)
	{
		LCD_RA8875_WRITE_DATA(*_ptr);
		while ((RA8875_ReadStatus() & 0x80) == 0x80);	/* ����ȴ��ڲ�д��������� */
		_ptr++;
	}
	

	RA8875_WriteReg(0x40, (0 << 7));	/* ��ԭΪͼ��ģʽ */
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_DispStr
*	����˵��: ��ʾ�ַ������ַ�����������RA8875��ӵ��ֿ�оƬ��֧�ֺ���.
*			������ɫ��������ɫ���Ƿ�ͨ͸�������ĺ�����������
*			void RA8875_SetFrontColor(uint16_t _usColor);
*			void RA8875_SetBackColor(uint16_t _usColor);
*	��    ��:  _usX, _usY : ��ʾλ�ã����꣩
*			 _ptr : AascII�ַ�������0����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr)
{
	/*
		RA8875 ͸��ʹ�� �� �Ϻ���ͨ��˾ (Genitop Inc) �ⲿ����ʽ�����ڴ� (Font ROM)����֧�ָ���
		������д�뵽DDRAM �С�RA8875 ���Ϻ���ͨ��˾ (Genitop Inc) ���ݵĲ�Ʒ���� :
		GT21L16TW/GT21H16T1W ��GT23L16U2W ��GT23L24T3Y/GT23H24T3Y ��GT23L24M1Z ��
		�� GT23L32S4W/GT23H32S4W����Щ�������16x16, 24x24, 32x32 �� (Dot) �벻ͬ����

		Ұ��RA8875�����弯�ɵ��ֿ�оƬΪ ��ͨ�ֿ�оƬ_GT23l32S4W

		GT23L32S4W��һ���ں�11X12����15X16�㡢24X24����32X32����ĺ��ֿ�оƬ��֧��GB2312
		���꺺�֣����й����ű�ί�Ϸ���Ȩ����SCII�ַ������и�ʽΪ���ú��š��û�ͨ���ַ����룬���ñ���
		���ṩ�ķ�����������ַ�������оƬ�еĵ�ַ���ɴӸõ�ַ���������ַ�������Ϣ��
	*/

	/* �����ı���ʾλ�ã�ע���ı�ģʽ��д�����ͼ��ģʽ��д�����ǲ�ͬ�ļĴ��� */
	RA8875_SetTextCursor(_usX, _usY);

	RA8875_WriteReg(0x40, (1 << 7));	/* ����Ϊ�ı�ģʽ */

	/*
		Serial Flash/ROM Ƶ��Ƶ���趨
			0xb: SFCL Ƶ�� = ϵͳƵ��Ƶ��(��DMA Ϊ����״̬������ɫ�����Ϊ256 ɫ����SFCL Ƶ��
				�̶�Ϊ=ϵͳƵ��Ƶ��/ 2)
			10b: SFCL Ƶ�� =ϵͳƵ��Ƶ��/ 2
			11b: SFCL Ƶ�� =ϵͳƵ��Ƶ��/ 4

		������������ϵͳƵ��Ϊ 68MHz

		GT23L32S4W�ķ����ٶȣ�SPI ʱ��Ƶ�ʣ�20MHz(max.)

		�����Ҫ���� 4 ��Ƶ, 17MHz
	*/
	RA8875_WriteReg(0x06, (3 << 0));	/* ����Ϊ�ı�ģʽ */

	/* ѡ���ⲿ����ROM */
	RA8875_WriteReg(0x21, (0 << 7) | (1 << 5));

	/* 05H  REG[05h] Serial Flash/ROM Configuration Register (SROC)
		7	Serial Flash/ROM I/F # ѡ��
				0:ѡ��Serial Flash/ROM 0 �ӿڡ�
				1:ѡ��Serial Flash/ROM 1 �ӿڡ�
		6	Serial Flash/ROM Ѱַģʽ
				0: 24 λѰַģʽ��
				��λ������Ϊ0��
		5	Serial Flash/ROM ����ģʽ
				0: ����ģʽ 0��
				1: ����ģʽ 3��
		4-3	Serial Flash /ROM ��ȡ���� (Read Cycle)
			00b: 4 bus ?? �޿����� (No Dummy Cycle)��
			01b: 5 bus ??1 byte �����ڡ�
			1Xb: 6 bus ??2 byte �����ڡ�
		2	Serial Flash /ROM ��ȡģʽ (Access Mode)
			0: ����ģʽ ��
			1: DMA ģʽ��
		1-0	Serial Flash /ROM I/F Data Latch ѡ��ģʽ
			0Xb: ��һģʽ��
			10b: ˫��ģʽ0��
			11b: ˫��ģʽ1��
	*/
	RA8875_WriteReg(0x05, (0 << 7) | (0 << 6) | (1 << 5) | (1 << 3) | (0 << 2) | (0 << 1));

	/*
		�����ⲿ����оƬ�ͺ�Ϊ GT23L32S4W, ����ΪGB2312,

		Bit1:0 ����ASCII�ַ��ĸ�ʽ��
			0 = NORMAL		 [�ʻ�ϸ, �ͺ��ֶ�������]
			1 = Arial		 [�ʻ��֣��ͺ��ֵײ�����]
			2 = Roman		 [�ʻ�ϸ, �ͺ��ֵײ�����]
			3 = Bold		 [����,������]
	 */
	RA8875_WriteReg(0x2F, (4 << 5) | (0 << 2) | (1 << 0));

	
	LCD_RA8875_REG(0x02); 			/* �����趨RA8875 �����ڴ�(DDRAM��CGRAM)��ȡ/д��ģʽ */

	/* ��ʼѭ�������ַ� */
	while (*_ptr != 0)
	{
		LCD_RA8875_WRITE_DATA(*_ptr);
		while ((RA8875_ReadStatus() & 0x80) == 0x80);
		_ptr++;
	}
	

	RA8875_WriteReg(0x40, (0 << 7));	/* ��ԭΪͼ��ģʽ */
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_SetCursor
*	����˵��: ���ö��Դ�Ĺ��λ�á� �ܶ������Ŀ�����д���Ͷ��������ͬ�ļĴ���������RA8875�ǲ�ͬ�ġ�
*	��    ��:  _usX : X����; _usY: Y����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 void RA8875_SetReadCursor(uint16_t _usX, uint16_t _usY)
{
	/* �����ڴ���������� */
	RA8875_WriteReg(0x4A, _usX);
	RA8875_WriteReg(0x4B, _usX >> 8);
	RA8875_WriteReg(0x4C, _usY);
	RA8875_WriteReg(0x4D, _usY >> 8);
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_SetTextCursor
*	����˵��: �����ı�д����λ��
*	��    ��:  _usX : X����; _usY: Y����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
 void RA8875_SetTextCursor(uint16_t _usX, uint16_t _usY)
{
	/* �����ڴ���������� */
	RA8875_WriteReg(0x2A, _usX);
	RA8875_WriteReg(0x2B, _usX >> 8);
	RA8875_WriteReg(0x2C, _usY);
	RA8875_WriteReg(0x2D, _usY >> 8);
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_QuitWinMode
*	����˵��: �˳�������ʾģʽ����Ϊȫ����ʾģʽ
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_QuitWinMode(void)
{
	RA8875_SetDispWin(0, 0, g_LcdHeight, g_LcdWidth);
}


/*
*********************************************************************************************************
*	�� �� ��: RA8875_CtrlGPO
*	����˵��: ����GPO���״̬��Ҳ����KOUT0-KOUT3��״̬
*	��    ��: _pin : 0-3, �ֱ��Ӧ GPO0 GPO1 GPO2 GPO3
*			  _value : 0 �� 1
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_CtrlGPO(uint8_t _pin, uint8_t _value)
{
	RA8875_WriteReg(0x13, _value << _pin);
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_SetBackLight
*	����˵��: ����RA8875оƬ��PWM1��ؼĴ���������LCD����
*	��    ��:  _bright ���ȣ�0����255������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_SetBackLight(uint8_t _bright)
{
	if (_bright == 0)
	{
		/* �ر�PWM, PWM1����ȱʡ���LOW  */
		RA8875_WriteReg(0x8A, 0 << 6);
	}
	else if (_bright == BRIGHT_MAX)	/* ������� */
	{
		/* �ر�PWM, PWM1����ȱʡ���HIGH */
		RA8875_WriteReg(0x8A, 1 << 6);
	}
	else
	{
		/* ʹ��PWM1, ����ռ�ձȵ��� */

		/* 	REG[8Ah] PWM1 Control Register (P1CR)

			Bit7 ������ȵ��� (PWM1) �趨
				0 : �رգ���״̬�£�PWM1 ���׼λ���մ˻�����Bit6 ������
				1 : ������

			Bit6 PWM1 �ر�ʱ��׼λ
				0 : ��PWM �رջ���˯��ģʽʱ��PWM1 ���Ϊ��Low�� ״̬��
				1 : ��PWM �رջ���˯��ģʽʱ��PWM1 ���Ϊ��High�� ״̬��

			Bit5 ����

			Bit4 PWM1 ����ѡ��
				0 : PWM1 ���ܡ�
				1 : PWM1 �̶����һƵ��Ϊ�ⲿ��������Clock (Fin) Ƶ�� 1 /16 ��Clock

			Bit3-0  PWM1 ��·��Ƶ����Դѡ��PWM_CLK	������PWM���Ƶ�ʡ�
				0000b : SYS_CLK / 1   , 1000b : SYS_CLK / 256
				0001b : SYS_CLK / 2   , 1001b : SYS_CLK / 512
				0010b : SYS_CLK / 4   , 1010b : SYS_CLK / 1024
				0011b : SYS_CLK / 8   , 1011b : SYS_CLK / 2048
				0100b : SYS_CLK / 16  , 1100b : SYS_CLK / 4096
				0101b : SYS_CLK / 32  , 1101b : SYS_CLK / 8192
				0110b : SYS_CLK / 64  , 1110b : SYS_CLK / 16384
				0111b : SYS_CLK / 128 , 1111b : SYS_CLK / 32768

				��SYS_CLK�� ����ϵͳƵ�ʣ� ����SYS_CLK Ϊ20MHz�� ��Bit[3:0] =0001b ʱ��PWM1 Ƶ����ԴΪ10MHz��
				ʵ�������PWMƵ�ʻ���Ҫ���� 256��֧��8λ�ķֱ��ʡ�


				Ϊ�˱�����Ƶ������PWMƵ�ʿ���ѡ��
				��1�� ��Ƶ100Hz
				��2�� ���� 20KHz

				���磬Bit3-0Ϊ0011bʱ SYS_CLK / 8��
		*/

		RA8875_WriteReg(0x8A, (1 << 7) | 12); 

		/* REG[8Bh] PWM1 Duty Cycle Register (P1DCR) */
		RA8875_WriteReg(0x8B, _bright);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_DrawHLine
*	����˵��: ����һ���̶���ɫ��ˮƽ�� ����Ҫ����UCGUI�Ľӿں�����
*	��    �Σ�_usX1    ����ʼ��X����
*			  _usY1    ��ˮƽ�ߵ�Y����
*			  _usX2    ��������X����
*			  _usColor : ��ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor)
{
	uint16_t i;

	RA8875_SetCursor(_usX1, _usY1);

	LCD_RA8875_REG(0x02); 		/* �����趨RA8875 �����ڴ�(DDRAM��CGRAM)��ȡ/д��ģʽ */
	for (i = 0; i < _usX2 - _usX1 + 1; i++)
	{
		LCD_RA8875_WRITE_DATA(_usColor);
	}

}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_DrawHColorLine
*	����˵��: ����һ����ɫˮƽ�� ����Ҫ����UCGUI�Ľӿں�����
*	��    �Σ�_usX1    ����ʼ��X����
*			  _usY1    ��ˮƽ�ߵ�Y����
*			  _usWidth ��ֱ�ߵĿ��
*			  _pColor : ��ɫ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_DrawHColorLine(uint16_t _usX1 , uint16_t _usY1, uint16_t _usWidth, uint16_t *_pColor)
{
	uint16_t i;

	RA8875_SetCursor(_usX1, _usY1);


	LCD_RA8875_REG(0x02); 		/* �����趨RA8875 �����ڴ�(DDRAM��CGRAM)��ȡ/д��ģʽ */

	for (i = 0; i < _usWidth; i++)
	{
		LCD_RA8875_WRITE_DATA(*_pColor++);
	}

}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_DrawVLine
*	����˵��: ����һ����ֱ���� ����Ҫ����UCGUI�Ľӿں�����
*	��    �Σ� _usX1    : ��ֱ�ߵ�X����
*			  _usY1    : ��ʼ��Y����
*			  _usY2    : ������Y����
*			  _usColor : ��ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_DrawVLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usY2 , uint16_t _usColor)
{
	uint16_t i;

	RA8875_SetCursor(_usX1, _usY1);

	for (i = 1; i <= _usY2 - _usY1 + 1; i++)
	{
		

		LCD_RA8875_REG(0x02); 		/* �����趨RA8875 �����ڴ�(DDRAM��CGRAM)��ȡ/д��ģʽ */
		LCD_RA8875_WRITE_DATA(_usColor);

		RA8875_SetCursor(_usX1, _usY1 + i);
	}

}


/**
 * @brief  Display_ON��Һ����
 * @param  ��
 * @retval ��
 */
void Display_ON(void)
{
	LCD_RA8875_REG(0x01);//PWRR
	LCD_RA8875_WRITE_DATA(0x80);
	RA8875_WaitStatus();
}


/**
 * @brief  Display_OFF�ر�Һ����
 * @param  ��
 * @retval ��
 */
void Display_OFF(void)
{
	LCD_RA8875_REG(0x01);//PWRR
	LCD_RA8875_WRITE_DATA(0x00);
}



/**
 * @brief  LCD �����λ
 * @param  ��
 * @retval ��
 */
void LCD_Rst(void)
{			
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);	 //�͵�ƽ��λ
    LCD_Delay(0xAFFf); 					   
    GPIO_SetBits(GPIOB, GPIO_Pin_1);		 	 
    LCD_Delay(0xAFFf); 	
}


/**
 * @brief  lcd��ʼ�������Ҫ�õ�lcd��һ��Ҫ�����������
 * @param  ��
 * @retval ��
 */
void LCD_Init(void)
{
	LCD_GPIO_Config();
	LCD_FSMC_Config();
	
	LCD_Rst();
  
  RA8875_Config();  

  
}

/*****************************************END OF FILE***************************/

