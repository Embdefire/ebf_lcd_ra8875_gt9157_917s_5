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
#include "GUI.h"
#include "DIALOG.h"

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	  
	uint16_t i;	
	
	/*��ʼ��Һ����*/
	LCD_Init();
	
	USART1_Config();
	printf("wildfire ISO board emWin test!\n");
	/*CRC��emWinû�й�ϵ��ֻ������Ϊ�˿�ı��������ģ�����STemWin�Ŀ�ֻ������ST��оƬ���棬���оƬ���޷�ʹ�õġ� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);	
	//��ʼ��emWin 
	GUI_Init();
	GUI_SetBkColor(0);
	GUI_Clear();
	GUI_SetFont(GUI_FONT_24_1);
	 
	/* ��ʾ���� */
	GUI_DrawGradientV(0, 0, 800-1, 480-1, 0x0000FF, 0x00FFFF);
	GUI_DispStringAt("wildfire ISO board emWin test!",30,10);
	
	
	while (1)
	{
		GUI_DispDecAt(i++,80,30,4);
		if(i>9999)
			i=0; 
	}

  

}

/* ------------------------------------------end of file---------------------------------------- */

