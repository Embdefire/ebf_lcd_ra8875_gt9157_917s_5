/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   5.0��Һ����ʾ����ʵ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO-mini STM32 ������ 
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
#include "bsp_led.h"

#include "ff.h"
#include "bsp_sdio_sdcard.h"
#include "bsp_bmp.h"
#include "palette.h"


FIL fnew;													/* file objects */
FATFS fs[2];													/* Work area (file system object) for logical drives */
FRESULT res_sd; 
UINT br, bw;            					/* File R/W count */


void Delay(__IO u32 nCount); 
void RA8875_Show_BMP(void);

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	  
  
  LED_GPIO_Config();
  
   
  /*��ʼ��Һ����*/
  LCD_Init();	
  
  /*�����ȳ�ʼ��FSMC(Һ��)���ٳ�ʼ������*/
  USART1_Config(); 
  
  
  /* Sdio Interrupt Config */
  NVIC_Configuration();
  
  /*Һ��ͼ�κ�����ʾ����*/
  
  LCD_INFO("LCD chip ID = 0x%x", RA8875_ReadID());  

  RA8875_ClrScr(CL_BLACK);	/* ��������ʾȫ�� */


  RA8875_SetFrontColor(CL_WHITE);
  
  RA8875_SetFont( RA_FONT_32,  10,  0);

  RA8875_DispStr(100,240,"Ұ��5.0��Һ������ʾBMPͼƬ���Գ���...");
  
  Delay(0xFFFFFF);


  while(1)
  {
      RA8875_Show_BMP();

    }
  

}



void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}




/* ��ʾbmpͼƬ���� */
void RA8875_Show_BMP(void)
{
  
  RA8875_ClrScr(CL_BLACK);	/* ��������ʾȫ�� */


    res_sd = f_mount(&fs[1],"1:",1);
    
  	if(res_sd !=FR_OK)
		{
      RA8875_ClrScr(CL_BLACK);	/* ��������ʾȫ�� */

      RA8875_SetFont( RA_FONT_24,  10,  0);

      RA8875_SetFrontColor(CL_RED);
      RA8875_DispStr(0,100,"SD���ļ�ϵͳ����ʧ�ܣ�������������SD���ٽ��в���..");
      
      RA8875_SetFrontColor(CL_WHITE);
      RA8875_DispStr(0,200,"����˵����");
      RA8875_DispStr(0,250,"1.��������Ҫ����������뱣�����ض�����BMPͼƬ��SD��");
      RA8875_DispStr(0,290,"2.�ڵ����ϰѱ����̹���Ŀ¼�µġ�SD�������ļ����ļ����е����ݸ��Ƶ�SD����Ȼ���SD�����뵽�����弴��");
      
      RA8875_SetFrontColor(CL_BLUE);
      RA8875_DispStr(0,360,"3.�����Ը�SD�����������йأ���ͨ��������ʾҺ���������⣬ֻҪ����������������������ʾҺ����������");
      
      RA8875_DispStr(0,430,"�������봥��������Գ���...");
      Delay(0x1FFFFFF);    
		
    }
    else
    {
      
        /* ������Ϊ��ɫ */
     RA8875_ClrScr(CL_WHITE); 
     Delay(0xFFFFF);      

      Lcd_show_bmp(0,0,"1:test_picture/tree.bmp"); 
      Delay(0xFFFFFF);     

      Lcd_show_bmp(0,0,"1:test_picture/moutain.bmp"); 
      Delay(0xFFFFFF);
      
      Lcd_show_bmp(0,0,"1:test_picture/car.bmp"); 
      Delay(0xFFFFFF);    

      Lcd_show_bmp(0,0,"1:test_picture/lake.bmp"); 
      Delay(0xFFFFFF);     
   
      Lcd_show_bmp(0,0,"1:test_picture/couple.bmp"); 
      Delay(0xFFFFFF);     
      
      Lcd_show_bmp(0,0,"1:test_picture/sea.bmp"); 
      Delay(0xFFFFFF);  

      Lcd_show_bmp(0,0,"1:test_picture/army.bmp"); 
      Delay(0xFFFFFF);   

    }
}



/* ------------------------------------------end of file---------------------------------------- */

