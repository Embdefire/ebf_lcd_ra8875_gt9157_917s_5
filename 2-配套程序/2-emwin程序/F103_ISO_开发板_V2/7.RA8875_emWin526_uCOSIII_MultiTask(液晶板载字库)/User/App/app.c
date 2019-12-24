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
#include "includes.h"

extern void CreateTouch(void);

void emWinMainApp(void)
{
	/* ��ʾ���� */
	GUI_DispStringAt("wildfire ISO board emWin test!",50,10);
	GUI_DrawGradientV(100, 50, 750, 470, 0x0000FF, 0x00FFFF);
	CreateTouch();
}
/*************************** End of file ****************************/
