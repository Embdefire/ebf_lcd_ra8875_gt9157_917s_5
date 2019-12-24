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
#include "bsp_SysTick.h"
#include "bsp_usart1.h"
#include "bsp_ra8875_lcd.h"
#include "gt9xx.h"
#include "bsp_i2c_gpio.h"
#include "GUI.h"
#include "GUIDEMO.h"

extern void CreateTouch(void);
/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	  
	/*��ʼ��Һ����*/
	LCD_Init();
	
	/* ��ʼ������ */
	USART1_Config();
	
	/* ��ʼ�������� */
	GTP_Init_Panel();  
	
	/* ��ʼ���δ�ʱ�� 1ms�ж�һ��*/
	SysTick_Init();
	
	
	
	/*CRC��emWinû�й�ϵ��ֻ������Ϊ�˿�ı��������ģ�����STemWin�Ŀ�ֻ������ST��оƬ���棬���оƬ���޷�ʹ�õġ� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);	
	
	//��ʼ��emWin 
	GUI_Init();
	
	GUI_SetFont(GUI_FONT_24B_ASCII);
	GUI_SetBkColor(GUI_YELLOW);
	GUI_Clear();
	 
//	/* ��ʾ���� */
//	GUI_DispStringAt("wildfire ISO board emWin test!",50,10);
//	GUI_DrawGradientV(100, 50, 750, 470, 0x0000FF, 0x00FFFF);
//	
//	CreateTouch();
	while (1)
	{
		GUIDEMO_Main();
	}
}

/* ------------------------------------------end of file---------------------------------------- */

