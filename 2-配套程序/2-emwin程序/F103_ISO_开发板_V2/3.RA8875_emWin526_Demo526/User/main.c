/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   5.0�����Һ����������ʵ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
 
#include "stm32f10x.h"
#include "bsp_ra8875_lcd.h"
#include "bsp_usart1.h"
#include "gt9xx.h"
#include "GUI.h"
#include "GUIDEMO.h"
#include "bsp_SysTick.h"
#include "bsp_fsmc_sram.h"

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	  
//	uint16_t i;	
	FSMC_SRAM_Init();
	SysTick_Init();
	
	/*��ʼ��Һ����*/
	LCD_Init();
	
	/* ��ʼ������ */
	USART1_Config();
	
	/* ��ʼ�������� */
	GTP_Init_Panel();  
	
	/*CRC��emWinû�й�ϵ��ֻ������Ϊ�˿�ı��������ģ�����STemWin�Ŀ�ֻ������ST��оƬ���棬���оƬ���޷�ʹ�õġ� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);	
	//��ʼ��emWin 
	GUI_Init();
	GUI_SetDefaultFont(GUI_FONT_24B_ASCII);
	GUI_SetBkColor(0);
	GUI_Clear();
	//  GUI_CURSOR_Show();
	 
	/* ��ʾ���� */
//	GUI_DispStringAt("wildfire ISO board emWin test!",50,10);
//	GUI_DrawGradientV(100, 50, 750, 470, 0x0000FF, 0x00FFFF);
	
	while (1)
	{
//		GUI_DispDecAt(i++,80,30,4);
//		if(i>9999)
//			i=0; 
		GUIDEMO_Main();
	}

  

}

/* ------------------------------------------end of file---------------------------------------- */

