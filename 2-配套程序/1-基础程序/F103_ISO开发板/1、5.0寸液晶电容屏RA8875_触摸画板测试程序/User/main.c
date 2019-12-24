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
#include "bsp_SysTick.h"
#include "bsp_usart1.h"
#include "gt9xx.h"
#include "bsp_i2c_gpio.h"
#include "palette.h"
#include "bsp_led.h"


/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	  
  
  LED_GPIO_Config();
  
  LED2_ON;
   
  /*��ʼ��Һ����*/
  LCD_Init();	
  
  /*�����ȳ�ʼ��FSMC(Һ��)���ٳ�ʼ������*/
  USART1_Config(); 

  LCD_INFO("LCD chip ID = 0x%x", RA8875_ReadID());  
  

  /* ��ʼ�������� */
  GTP_Init_Panel();       

  /*��ʼ������Ӧ��*/
  Palette_Init();

  while(1);
  

}

/* ------------------------------------------end of file---------------------------------------- */

