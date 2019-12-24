/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����5.0��lcd��ʾӢ���ַ�
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
#include "bsp_usart1.h"


void Delay(__IO u32 nCount); 
void RA8875_Test(void);

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{  
  /*��ʼ��Һ����*/
  LCD_Init();	
  
  /*�����ȳ�ʼ��FSMC(Һ��)���ٳ�ʼ������*/
  USART1_Config();   
 
  
  printf("id =%x ",RA8875_ReadID());
  
  
	while( 1 )
  {
    RA8875_Test();
    }
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
  
  RA8875_SetFont( RA_FONT_24,  0,  0);

  RA8875_DispStr(0,0,"Ұ��5.0��Һ��������:");
  RA8875_DispStr(0,30,"�ֱ��ʣ�800x480 ����");
  RA8875_DispStr(0,60,"���ݴ�������֧��5�㴥��");
  RA8875_DispStr(0,90,"��Ļ�Դ������ֿ⣬�Դ���ͼ���棬�ɳ�����������2Dͼ��");
  RA8875_DispStr(0,120,"Һ������������8080�ӿ�ͨѶ��16λ�����ߣ���ֱ�Ӳ���FSMC����");
  RA8875_DispStr(0,150,"����������������IIC�ӿ�ͨѶ");


  /* ��ֱ�� */
  RA8875_SetFrontColor(CL_BLUE);
  
  
  RA8875_SetFont( RA_FONT_32,  0,  0);
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

/*********************************************END OF FILE**********************/

