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
#include "GUI.h"
#include "bsp_usart1.h"

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
    
    
    p.FSMC_AddressSetupTime = 0x01;	 //��ַ����ʱ��
    p.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ��
    p.FSMC_DataSetupTime = 0x03;		 //���ݽ���ʱ��
    p.FSMC_BusTurnAroundDuration = 0x00;
    p.FSMC_CLKDivision = 0x00;
    p.FSMC_DataLatency = 0x00;
    p.FSMC_AccessMode = FSMC_AccessMode_B;	 // һ��ʹ��ģʽB������LCD
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
//	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;	/* ע��ɿ��������Ա */
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
uint16_t RA8875_ReadReg(uint16_t RegAddr)
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

uint8_t RA8875_ReadStatus(void)
{
	return  LCD_RA8875_READ_STATUS();
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
}

void RA8875_Config(void)
{

		/* ��ʼ��PLL.  ����Ƶ��Ϊ25M */
		LCD_RA8875_REG(0x88);
		LCD_Delay(10);		/* �����ӳ� */
		LCD_RA8875_WRITE_DATA(12);	/* PLLDIVM [7] = 0 ;  PLLDIVN [4:0] = 10 */

		LCD_Delay(10000);  

		LCD_RA8875_REG(0x89);
		LCD_Delay(10);		/* �ӳ�1us */
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
	//RA8875_WriteReg(0x20, 0x04);	

    /* 	REG[41h] Memory Write Control Register1 (MWCR1)
      д��Ŀ��λ�ã�ѡ��ͼ��1
    */
    RA8875_WriteReg(0x41, 0x01);	/* ѡ���ͼģʽ, Ŀ��ΪCGRAM */
	RA8875_SetDispWin(0, 0, 480, 800);
}


/**
 * @brief  LCD �����λ
 * @param  ��
 * @retval ��
 */
void LCD_Rst(void)
{			
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);	 //�͵�ƽ��λ
    LCD_Delay(0xAFFff); 					   
    GPIO_SetBits(GPIOB, GPIO_Pin_1);		 	 
    LCD_Delay(0xAFFff); 	
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

/*************** emWin ***********************/

u16 GUIColorToRGB(GUI_COLOR color)
{
	u32 Color_R;
	u32 Color_G;
	u32 Color_B;
	u32 tempcolor;
	tempcolor=color;
	Color_B=(tempcolor&0x00f80000)>>19;//ȡ��5BIT
	Color_G=(tempcolor&0x0000fc00)>>5;
	Color_R=(tempcolor&0x000000f8)<<8;
	
	return((u16)((Color_R)|(Color_G)|Color_B));
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
	//s_ucRA8875Busy = 1;
	RA8875_WriteReg(0x63, (_usColor & 0xF800) >> 11);	/* R5  */
	RA8875_WriteReg(0x64, (_usColor & 0x07E0) >> 5);	/* G6 */
	RA8875_WriteReg(0x65, (_usColor & 0x001F));			/* B5 */
	//s_ucRA8875Busy = 0;
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
	//s_ucRA8875Busy = 1;
	RA8875_WriteReg(0x60, (_usColor & 0xF800) >> 11);	/* R5  */
	RA8875_WriteReg(0x61, (_usColor & 0x07E0) >> 5);	/* G6 */
	RA8875_WriteReg(0x62, (_usColor & 0x001F));			/* B5 */
	//s_ucRA8875Busy = 0;
}


/*
*********************************************************************************************************
*	�� �� ��: RA8875_SetFont
*	����˵��: �ı�ģʽ�������������塢�о���־�
*	��    �Σ�
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
	//s_ucRA8875Busy = 1;
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
	//s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: RA8875_DispStr
*	����˵��: ��ʾ�ַ������ַ�����������RA8875��ӵ��ֿ�оƬ��֧�ֺ���.
*			������ɫ��������ɫ���Ƿ�ͨ͸�������ĺ�����������
*			void RA8875_SetFrontColor(uint16_t _usColor);
*			void RA8875_SetBackColor(uint16_t _usColor);
*	��    �Σ�_usX, _usY : ��ʾλ�ã����꣩
*			 _ptr : AascII�ַ�������0����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RA8875_DispChar(U16P c)
{
//	u8 i=0;
	unsigned char ch[2];
	ch[0]=(c>>8)&0x00ff;
	ch[1]=c&0xff;
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
//	RA8875_SetTextCursor(100, 100);

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
	RA8875_WriteReg(0x2F, (4 << 5) | (0 << 2) | (0 << 0));

	
	LCD_RA8875_REG(0x02); 			/* �����趨RA8875 �����ڴ�(DDRAM��CGRAM)��ȡ/д��ģʽ */
	//printf("rx1=%x,rx2=%x\n",RA8875_ReadReg(0x2a),RA8875_ReadReg(0x2b));
	if(ch[0])
	{
		LCD_RA8875_WRITE_DATA(ch[0]);
		while ((RA8875_ReadStatus() & 0x80) == 0x80);
	}
	LCD_RA8875_WRITE_DATA(ch[1]);
	while ((RA8875_ReadStatus() & 0x80) == 0x80);
//	/* ��ʼѭ�������ַ� */
//	for(i=0;i<2;i++)
//	{
//		//RA8875_WriteData("��");
//		//ch[0]="��";
//		LCD_RA8875_WRITE_DATA(ch[i]);
//		while ((RA8875_ReadStatus() & 0x80) == 0x80);

//		//i++;
//	}
	//s_ucRA8875Busy = 0;

	RA8875_WriteReg(0x40, (0 << 7));	/* ��ԭΪͼ��ģʽ */
	while ((RA8875_ReadStatus() & 0x80) == 0x80);
	
	while((RA8875_ReadReg(0xf0)&0x01)==0x01) ;
}

void RA8875_SetTextMode(uint8_t Mode)
{
	/*
		pdf ��22ҳ		[22H]
		bit7 ���ڶ��룬һ�㲻�ã�ȱʡ��0
		bit6 ����ͨ͸��һ�㲻�ã�ȱʡ��0
		bit4 ������ת90����һ�㲻�ã�ȱʡ��0
		bit3-2 ˮƽ�Ŵ���
		bit1-0 ��ֱ�Ŵ���
	*/
	RA8875_WriteReg(0x22, (Mode<<6));
}
/*
*********************************************************************************************************
*	�� �� ��: RA8875_SetTextCursor
*	����˵��: �����ı�д����λ��
*	��    �Σ�_usX : X����; _usY: Y����
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
********************************************************************
*  �ر�˵������һ��ʹ�ã������Լ�����������һ�´������֣���������������
*  һ�����������������ģ�һ�������ڹٷ�DEMO��ʾ�ġ�
********************************************************************
*/
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
	//s_ucRA8875Busy = 1;
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
	//s_ucRA8875Busy = 0;
}

void BTE_SetSourse(U16 x0,U16 y0,U16 Layer)
{
	RA8875_WriteReg(0x54,x0);
	RA8875_WriteReg(0x55,x0>>8);
	RA8875_WriteReg(0x56,y0);
	if(Layer==1)
			RA8875_WriteReg(0x57,(y0>>8)|0x80);
	else
			RA8875_WriteReg(0x57,(y0>>8)&0x7f);
	
}
void BTE_SetDist(U16 x0,U16 y0,U16 Layer)
{
	RA8875_WriteReg(0x58,x0);
	RA8875_WriteReg(0x59,x0>>8);
	RA8875_WriteReg(0x5A,y0);
	if(Layer==1)
			RA8875_WriteReg(0x5B,(y0>>8)|0x80);
	else
			RA8875_WriteReg(0x5B,(y0>>8)&0x7f);
	
}

void BTE_SetArea(U16 x0,U16 y0)
{
	RA8875_WriteReg(0x5C,x0);
	RA8875_WriteReg(0x5D,x0>>8);
	RA8875_WriteReg(0x5E,y0);
	RA8875_WriteReg(0x5F,y0>>8);
}




//static U8 FontDataBuf[128];
void BTE_DRAW(U16 x,U16 y,U16 ch,U8 u_Font)
{
	RA8875_SetFrontColor(GUIColorToRGB(GUI_GetColor()));
	RA8875_SetBackColor(GUIColorToRGB(GUI_GetBkColor()));

	RA8875_SetFont(u_Font,1,1);
	RA8875_DispChar(ch);	
	BTE_SetSourse(x,y,0);
	BTE_SetDist(x,y,0);
	BTE_SetArea(32,32);
	//RA8875_SetFrontColor(GUIColorToRGB(GUI_CYAN)); //ȡ����ɫ  
	RA8875_WriteReg(0x51,0x82);
	BTE_Start();
	BTE_Wait();	
}

/*****************************************END OF FILE***************************/

