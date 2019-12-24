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
#include "bsp_led.h"
#include "palette.h"

#include "ff.h"
#include "bsp_sdio_sdcard.h"
#include "bsp_bmp.h"


FIL fnew;													/* file objects */
FATFS fs[2];													/* Work area (file system object) for logical drives */
FRESULT res_sd; 
UINT br, bw;            					/* File R/W count */


void Delay(__IO u32 nCount); 
void RA8875_Test(void);
void RA8875_Palette(void);
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

  RA8875_DispStr(200,240,"Ұ��5.0��Һ�������Գ���...");
  
  Delay(0xFFFFFF);

    
  RA8875_Test();
  
  RA8875_Show_BMP();
  
  RA8875_Palette();
  

  while(1);
  

}



void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}



/*���ڲ��Ը���RA8875�ĺ���*/
void RA8875_Test(void)
{  
  RA8875_ClrScr(CL_BLACK);	/* ��������ʾȫ�� */

  RA8875_SetFrontColor(CL_WHITE);
  
  RA8875_SetFont( RA_FONT_32,  10,  0);

  RA8875_DispStr(200,240,"Һ����ʾ����...");
  
  Delay(0xFFFFFF);
  
  
  RA8875_ClrScr(CL_BLACK);	/* ��������ʾȫ�� */

  RA8875_SetFrontColor(CL_WHITE);
  
  RA8875_SetFont( RA_FONT_24,  10,  0);

  RA8875_DispStr(0,0,"Ұ��5.0��Һ��������:");
  RA8875_DispStr(0,30,"�ֱ��ʣ�800x480 ����");
  RA8875_DispStr(0,60,"���ݴ�������֧��5�㴥��");
  RA8875_DispStr(0,90,"��Ļ�Դ������ֿ⣬�Դ���ͼ���棬�ɳ�����������2Dͼ��");
  RA8875_DispStr(0,120,"Һ������������8080�ӿ�ͨѶ��16λ�����ߣ���ֱ�Ӳ���FSMC����");
  RA8875_DispStr(0,150,"����������������IIC�ӿ�ͨѶ");


  /* ��ֱ�� */
  RA8875_SetFrontColor(CL_BLUE);
  
  
  RA8875_SetFont( RA_FONT_32,  10,  0);
  RA8875_DispStr(10,200,"���ߣ�");
  
  RA8875_DrawLine(50,250,750,250,CL_RED);
  
  RA8875_DrawLine(50,300,750,300,CL_RED);
  
  RA8875_DrawLine(400,250,200,400,CL_GREEN);
  
  RA8875_DrawLine(600,250,250,400,CL_GREEN);
  
  Delay(0xFFFFFF);
  
  RA8875_FillRect(0,200,LCD_HEIGHT-200,LCD_WIDTH,CL_BLACK);
  

  
  /*������*/
  RA8875_SetFrontColor(CL_BLUE);
  RA8875_DispStr(10,200,"�����Σ�");
  
  RA8875_DrawRect(200,250,200,100,CL_RED);
  RA8875_DrawRect(350,250,200,50,CL_GREEN);
  RA8875_DrawRect(200,350,50,200,CL_BLUE);
  
  Delay(0xFFFFFF);
  
  RA8875_FillRect(0,200,LCD_HEIGHT-200,LCD_WIDTH,CL_BLACK);

  

  /*������*/
  RA8875_SetFrontColor(CL_BLUE);
  RA8875_DispStr(10,200,"������:");
 
  RA8875_FillRect(200,250,200,100,CL_RED);
  RA8875_FillRect(350,250,200,50,CL_GREEN);
  RA8875_FillRect(200,350,50,200,CL_BLUE);
  
  Delay(0xFFFFFF);
  
  RA8875_FillRect(0,200,LCD_HEIGHT-200,LCD_WIDTH,CL_BLACK);

  /* ��Բ */
  RA8875_SetFrontColor(CL_BLUE);
  RA8875_DispStr(10,200,"��Բ:");
  
  RA8875_DrawCircle(200,350,50,CL_RED);
  RA8875_DrawCircle(350,350,75,CL_GREEN);
  
  Delay(0xFFFFFF);
  
  RA8875_FillRect(0,200,LCD_HEIGHT-200,LCD_WIDTH,CL_BLACK);


  /*���Բ*/
  RA8875_SetFrontColor(CL_BLUE);
  RA8875_DispStr(10,200,"���Բ��");
  
  RA8875_FillCircle(300,350,50,CL_RED);
  RA8875_FillCircle(450,350,75,CL_GREEN);
  
  Delay(0xFFFFFF);
  
  RA8875_FillRect(0,200,LCD_HEIGHT-200,LCD_WIDTH,CL_BLACK);


}

/* ��ʾbmpͼƬ���� */
void RA8875_Show_BMP(void)
{
  
  RA8875_ClrScr(CL_BLACK);	/* ��������ʾȫ�� */

  RA8875_SetFrontColor(CL_WHITE);
  
  RA8875_SetFont( RA_FONT_32,  10,  0);

  RA8875_DispStr(200,240,"��ʾBMPͼƬ����...");
  Delay(0xFFFFFF);    

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


/*�����������*/
void RA8875_Palette(void)
{
      
  RA8875_ClrScr(CL_BLACK);	/* ��������ʾȫ�� */

  RA8875_SetFrontColor(CL_WHITE);
  
  RA8875_SetFont( RA_FONT_32,  10,  0);

  RA8875_DispStr(200,240,"����������...");
  
  Delay(0xFFFFFF);
  
    /* ��ʼ�������� */
  GTP_Init_Panel();       

  /*��ʼ������Ӧ��*/
  Palette_Init();

}

/* ------------------------------------------end of file---------------------------------------- */

