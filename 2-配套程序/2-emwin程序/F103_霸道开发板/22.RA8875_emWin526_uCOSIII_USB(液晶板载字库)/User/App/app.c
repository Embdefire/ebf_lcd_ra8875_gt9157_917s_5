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

/*
*********************************************************************************************************
*                                         �궨�� 
*********************************************************************************************************
*/
#define ICONVIEW_Width     70   /* �ؼ�ICONVIEW�Ŀ� */  
#define ICONVIEW_Height    70   /* �ؼ�ICONVIEW�ĸߣ�����ͼ���Y������ */  
#define ICONVIEW_XSpace    30   /* �ؼ�ICONVIEW��X������ */ 
#define ICONVIEW_YSpace    20   /* �ؼ�ICONVIEW��Y������ */ 
#define ICONVIEW_XPos	   10    /* �ؼ�ICONVIEW��X�����������*/
#define BOTWIN_YSpace	   60   /* ��Ļ�·����ִ���Y������*/
#define MIDWIN_yPos		   40   /* �м䴰��Y������ʼλ�ü��������ֿ��ø߶�*/
#define MIDWIN_xPos		   0 	/* �м䴰��Y������ʼλ��*/

/*
*********************************************************************************************************
*                                      ����
*********************************************************************************************************
*/ 
extern GUI_CONST_STORAGE GUI_BITMAP bmled;
extern GUI_CONST_STORAGE GUI_BITMAP bmbutton;
extern GUI_CONST_STORAGE GUI_BITMAP bmsram;
extern GUI_CONST_STORAGE GUI_BITMAP bmCAN;

extern GUI_CONST_STORAGE GUI_BITMAP bmflash;
extern GUI_CONST_STORAGE GUI_BITMAP bmadc;
extern GUI_CONST_STORAGE GUI_BITMAP bmWeather;
extern GUI_CONST_STORAGE GUI_BITMAP bmWeather2;

extern GUI_CONST_STORAGE GUI_BITMAP bmbln;
extern GUI_CONST_STORAGE GUI_BITMAP bmclock;
extern GUI_CONST_STORAGE GUI_BITMAP bmmusic;
extern GUI_CONST_STORAGE GUI_BITMAP bmcamera;

extern GUI_CONST_STORAGE GUI_BITMAP bmPhone;
extern GUI_CONST_STORAGE GUI_BITMAP bmmessage;
extern GUI_CONST_STORAGE GUI_BITMAP bmnote;
extern GUI_CONST_STORAGE GUI_BITMAP bmusb;

extern GUI_CONST_STORAGE GUI_BITMAP bmcalculator;
extern GUI_CONST_STORAGE GUI_BITMAP bmpicture;
extern GUI_CONST_STORAGE GUI_BITMAP bmbrowser;

/*ʱ��ṹ��*/
extern struct rtc_time systmtime;
extern char TimeDisplay;
//GUI_XBF_DATA XBF_Data;
//GUI_FONT     XBF_Font;

uint8_t OnICON0  = 0;   /* ICONVIEW�ؼ����µı�־��0��ʾδ���£�1��ʾ���� */
uint8_t OnICON1  = 0;
uint8_t OnICON2  = 0;
uint8_t OnICON3  = 0;
uint8_t OnICON4  = 0;
uint8_t OnICON5  = 0;
uint8_t OnICON6  = 0;
uint8_t OnICON7  = 0;
uint8_t OnICON8  = 0;
uint8_t OnICON9  = 0;
uint8_t OnICON10  = 0;
uint8_t OnICON11  = 0;
uint8_t OnICON12  = 0;
uint8_t OnICON13  = 0;
uint8_t OnICON14  = 0;
uint8_t OnICON15  = 0;
uint8_t OnICON16  = 0;
uint8_t OnICON17  = 0;
uint8_t OnICON18  = 0;


/* ��������ICONVIEWͼ��Ĵ��� */
typedef struct {
  const GUI_BITMAP * pBitmap;
  const char       * pText;
} BITMAP_ITEM;

typedef struct WIN_PARA{			//����ʹ�õ����û���������������ڻص�������ʹ��
	int xSizeLCD, ySizeLCD;			//LCD��Ļ�ߴ�
	int xPosWin,  yPosWin;			//���ڵ���ʼλ��
	int xSizeWin, ySizeWin;			//���ڳߴ�	
	int xSizeBM,  ySizeBM;
	int ySizeBotWin;
	WM_HWIN hWinBot;				//���ƴ��ڵľ�����ײ���
	WM_HWIN hWinMid;				//����ʾ���ľ�����в���	
}WIN_PARA;

WIN_PARA WinPara;			//�û�ʹ�õĴ��ڶ�������
uint8_t app_prevent_refresh = 0;  /* ��������Ƚ���Ҫ�����ڷ�ֹICON�ؼ������ʱ�ʹ����ص�������WM_PAINT��Ϣ*/

/* ��������ICONVIEW0ͼ��Ĵ��� */
static const BITMAP_ITEM _aBitmapItem[] = 
{
	{&bmled,  	 	"LED"			},
	{&bmbutton,  	"KEY"			},  
	{&bmsram,      	"SRAM"			},
	{&bmCAN,  	 	"CAN"			},

	{&bmflash,    	"EEPROM" 		},
	{&bmadc,  	 	"ADC"			},
	{&bmWeather,   	"DS18B20" 		},
	{&bmWeather2,  	"TEMP" 			},

	{&bmbln, 		"Breathing"		},  
	{&bmclock,     	"Clock" 		},
//	{&bmmusic,     	"Music" 		},
//	{&bmcamera,    	"Camera"		},

	{&bmPhone,     	"Phone" 		},
	{&bmmessage,  	"Message"		},
	{&bmnote,      	"Note" 			},
	{&bmusb,  	   	"USB"			},

//	{&bmcalculator,	"Calculator" 	},
//	{&bmpicture,   	"Picture" 		},
//	{&bmbrowser,   	"Browser"		},
};




//void OnICON0Clicked(void)  {printf("OnICON0Clicked\n");}
//void OnICON1Clicked(void)  {printf("OnICON1Clicked\n");}
//void OnICON2Clicked(void)  {printf("OnICON2Clicked\n");}
//void OnICON3Clicked(void)  {printf("OnICON3Clicked\n");}

//void OnICON4Clicked(void)  {printf("OnICON4Clicked\n");}
//void OnICON5Clicked(void)  {printf("OnICON5Clicked\n");}
//void OnICON6Clicked(void)  {printf("OnICON6Clicked\n");}
//void OnICON7Clicked(void)  {printf("OnICON7Clicked\n");}

//void OnICON8Clicked(void)  {printf("OnICON8Clicked\n");}
//void OnICON9Clicked(void)  {printf("OnICON9Clicked\n");}
//void OnICON10Clicked(void) {printf("OnICON10Clicked\n");}
//void OnICON11Clicked(void) {printf("OnICON11Clicked\n");}

//void OnICON12Clicked(void) {printf("OnICON12Clicked\n");}
//void OnICON13Clicked(void) {printf("OnICON13Clicked\n");}
void OnICON14Clicked(void) {printf("OnICON14Clicked\n");}
void OnICON15Clicked(void) {printf("OnICON15Clicked\n");}

void OnICON16Clicked(void) {printf("OnICON16Clicked\n");}
void OnICON17Clicked(void) {printf("OnICON17Clicked\n");}
void OnICON18Clicked(void) {printf("OnICON18Clicked\n");}
	
/*
*********************************************************************************************************
*	�� �� ��: _cbBkWindow
*	����˵��: ���洰�ڵĻص����� 
*	��    �Σ�WM_MESSAGE * pMsg
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void _cbBkWindow(WM_MESSAGE * pMsg) 
{
	char text_buffer[20]={0};
	WM_HWIN hText;
	switch (pMsg->MsgId) 
	{
		case WM_TIMER://����ʱ����ʾ����Ϣ��ÿ�����޸�һ��ʱ��
			if(!TimeDisplay || app_prevent_refresh)
			{
				WM_RestartTimer(pMsg->Data.v, 1000);
				break;
			}
			/* ��ȡtext��� */      
			hText = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1);
			
			/* ת��rtcֵ������ʱ�� */
			RTC_TimeCovr(&systmtime);
  
			/* ת�����ַ��� */
			sprintf(text_buffer,"%02d:%02d:%02d",systmtime.tm_hour,systmtime.tm_min,systmtime.tm_sec);
			/* ���ʱ�� */
			TEXT_SetText(hText,text_buffer);
			WM_RestartTimer(pMsg->Data.v, 1000);
		break;
		/* �ػ���Ϣ*/
		case WM_PAINT:		
				GUI_SetBkColor(GUI_LIGHTCYAN+0x60);
				GUI_Clear();
//				GUI_SetFont(&GUI_FontHZ32);
//				GUI_SetColor(GUI_RED);
//				GUI_DispStringHCenterAt("APPͼ����ʾ",400,300);			
				ICONVIEW_SetSel(WM_GetDialogItem(WinPara.hWinMid, GUI_ID_ICONVIEW0),-1);
		break;
			
	 default:
		WM_DefaultProc(pMsg);
		break;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: _cbMidWin
*	����˵��: 
*	��    �Σ�WM_MESSAGE * pMsg
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void _cbMidWin(WM_MESSAGE * pMsg) 
{
	int NCode, Id;
	switch (pMsg->MsgId) 
	{
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
			NCode = pMsg->Data.v;                 /* Notification code */
			switch (Id) 
			{
				case GUI_ID_ICONVIEW0:
					switch (NCode) 
					{
						/* ICON�ؼ������Ϣ */
						case WM_NOTIFICATION_CLICKED:
							app_prevent_refresh = 1;
							break;
						
						/* ICON�ؼ��ͷ���Ϣ */
						case WM_NOTIFICATION_RELEASED: 
							
							/* ����Ӧѡ�� */
							switch(ICONVIEW_GetSel(pMsg->hWinSrc))
							{
								
								/* Phone *******************************************************************/
								case 0:	
									OnICON0 = 1;
									//printf("this is the NO.1 ICON\n");
									OnICON0Clicked();			
									break;	
								
								/* Message ***********************************************************************/
								case 1:
									OnICON1 = 1;
									//printf("this is the NO.2 ICON\n");
									OnICON1Clicked();
									break;
								
								/* Note *********************************************************************/
								case 2:
									OnICON2 = 1;
									//printf("this is the NO.3 ICON\n");
									OnICON2Clicked();	
									break;
								
								/* Ethernet *******************************************************************/
								case 3:
									OnICON3 = 1;
									//printf("this is the NO.4 ICON\n");
									OnICON3Clicked();
									break;
								
								/* LED  ******************************************************************/
								case 4:	
									OnICON4 = 1;
									OnICON4Clicked();			
									break;	
								
								/* KEY ***********************************************************************/
								case 5:
									OnICON5 = 1;
									OnICON5Clicked();
									break;
								
								/* SRAM *********************************************************************/
								case 6:
									OnICON6 = 1;
									OnICON6Clicked();	
									break;
								
								/* CAN ********************************************************************/
								case 7:
									OnICON7 = 1;
									OnICON7Clicked();
									break;
								
								/* EEPROM **********************************************************************/
								case 8:
									OnICON8 = 1;
									OnICON8Clicked();
									break;
								
								/* ADC ********************************************************************/
								case 9:
									OnICON9 = 1;
									OnICON9Clicked();
									break;
								
								/* DS18B20 ******************************************************************/
								case 10:					
									OnICON10 = 1;
									OnICON10Clicked();
									break;
								
								/* TEMP *******************************************************************/
								case 11:
									OnICON11 = 1;
									OnICON11Clicked();
									break;
								
								/* Breathing ******************************************************************/
								case 12:
									OnICON12 = 1;
									OnICON12Clicked();
									break;
								 
								 /* Clock ******************************************************************/
								case 13:
									OnICON13 = 1;
									OnICON13Clicked();
									break;
								 
								 /* FM ******************************************************************/
								case 14:
									OnICON14 = 1;
									OnICON14Clicked();
									break;
								 
								/* Music *******************************************************************/
								case 15:
									OnICON15 = 1;
									OnICON15Clicked();
									break;
								/* Music *******************************************************************/
								case 16:
									OnICON16 = 1;
									OnICON16Clicked();
									break;
								/* Music *******************************************************************/
								case 17:
									OnICON17 = 1;
									OnICON17Clicked();
									break;
								/* Music *******************************************************************/
								case 18:
									OnICON18 = 1;
									OnICON18Clicked();
									break;
								}
							 break;
						}
					break;
			}
			break;
			
		/* �ػ���Ϣ*/
		case WM_PAINT:
			{
				GUI_SetBkColor(GUI_WHITE);
				GUI_Clear();
			}			
		break;			
	 default:
		WM_DefaultProc(pMsg);
		break;
	}
}

/**
  * @brief  _cbBotWin,�������ص�����
  * @param  none
  * @retval none
  */
static void _cbBotWin(WM_MESSAGE * pMsg)
{
	switch (pMsg->MsgId) 
	{			 		 
		case WM_PAINT:	                             //�ػ汳��	
			GUI_SetBkColor(GUI_LIGHTCYAN+0x30);
			GUI_Clear();
			GUI_SetFont(&GUI_FontHZ24);//GUI_SetFont(GUI_FONT_32B_ASCII);
			GUI_SetColor(GUI_BLUE);
			GUI_DispStringHCenterAt("�Ա�:http://fire-stm32.taobao.com",200,25);	
			GUI_DispStringHCenterAt("��̳:http://www.firebbs.cn",600,25);	
		break;
		default:		
			WM_DefaultProc(pMsg);
		break;		 
	}
}

/**
  * @brief  CreateTopWin�����������Ĵ���
  * @param  none
  * @retval none
  */
static void CreateTopWin(void)
{
	WM_HWIN hText;
	/* ������ "wildfire"�ı� */
	hText = TEXT_CreateEx(0, 5, 120 , 30, WM_HBKWIN, WM_CF_SHOW, GUI_TA_LEFT|TEXT_CF_VCENTER, GUI_ID_TEXT0, "");
	TEXT_SetFont(hText, &GUI_FontHZ24);
	TEXT_SetText(hText,"Ұ��5�������");
	TEXT_SetTextColor(hText,GUI_BLUE);
	
	/* ״̬����ʱ����ʾ�ı� */
	hText = TEXT_CreateEx(WinPara.xSizeLCD/2-50,5,100,30,WM_HBKWIN,WM_CF_SHOW,GUI_TA_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT1,"11:56:00");
	//TEXT_SetBkColor(hText,GUI_INVALID_COLOR);
	TEXT_SetTextColor(hText,GUI_DARKGRAY);
	TEXT_SetFont(hText,GUI_FONT_24B_ASCII);
}
/**
  * @brief  CreateMidWin�������м�Ĵ���
  * @param  none
  * @retval none
  */
static void CreateMidWin(void)
{
	uint8_t i=0;
	WM_HWIN MIDWIN;
	WinPara.hWinMid= WM_CreateWindowAsChild(
											WinPara.xPosWin,											
											WinPara.yPosWin,
											WinPara.xSizeWin,
											WinPara.ySizeWin,	
											WM_HBKWIN, 
											WM_CF_SHOW|WM_CF_MEMDEV,
											_cbMidWin, 
											sizeof(WIN_PARA *)
											);
/*-------------------------------------------------------------------------------------*/
					/*��ָ��λ�ô���ָ���ߴ��ICONVIEW1 С����*/
					MIDWIN=ICONVIEW_CreateEx(
											 ICONVIEW_XPos, 					/* С���ߵ��������أ��ڸ������У�*/
											 0, 								/* С���ߵ��������أ��ڸ������У�*/
											 WinPara.xSizeLCD-ICONVIEW_XPos,	/* С���ߵ�ˮƽ�ߴ磨��λ�����أ�*/
											 3*(WinPara.ySizeBM+35), 			/* С���ߵĴ�ֱ�ߴ磨��λ�����أ�*/
											 WinPara.hWinMid, 				    /* �����ڵľ�������Ϊ0 ������С���߽���Ϊ���棨�������ڣ����Ӵ��� */
											 WM_CF_SHOW|WM_CF_HASTRANS,       /* ���ڴ�����ǡ�ΪʹС���������ɼ���ͨ��ʹ�� WM_CF_SHOW */ 
											 0,//ICONVIEW_CF_AUTOSCROLLBAR_V, 	/* Ĭ����0�����������ʵ������������ֱ������ */
											 GUI_ID_ICONVIEW0, 			        /* С���ߵĴ���ID */
											 ICONVIEW_Width, 				    /* ͼ���ˮƽ�ߴ� */
											 ICONVIEW_Height+20);				/* ͼ��Ĵ�ֱ�ߴ� */
											 
		/* ��ICONVIEW С���������ͼ�� */
		for (i = 0; i < GUI_COUNTOF(_aBitmapItem); i++) 
		{
			ICONVIEW_AddBitmapItem(MIDWIN, _aBitmapItem[i].pBitmap,_aBitmapItem[i].pText);
			ICONVIEW_SetTextColor(MIDWIN,i,GUI_DARKGRAY);
		}	
		/* ����С���ߵı���ɫ 32 λ��ɫֵ��ǰ8 λ������alpha��ϴ���Ч��*/
		ICONVIEW_SetBkColor(MIDWIN, ICONVIEW_CI_SEL, GUI_WHITE | 0x80000000);
		/* �������� */
		ICONVIEW_SetFont(MIDWIN, GUI_FONT_16B_ASCII);
		/* ���ó�ʼѡ���ͼ��Ϊ -1 (��ʾ��δѡ��)*/
		ICONVIEW_SetSel(MIDWIN,-1);
		/* ����ͼ����x ��y �����ϵļ�ࡣ*/
		ICONVIEW_SetSpace(MIDWIN, GUI_COORD_X, ICONVIEW_XSpace);
		ICONVIEW_SetSpace(MIDWIN, GUI_COORD_Y, ICONVIEW_YSpace);
	//	ICONVIEW_SetSpace(hWin, GUI_COORD_Y, ICONVIEW_YSpace);
		/* ���ö��뷽ʽ ��5.22�汾�����¼���� */
		ICONVIEW_SetIconAlign(MIDWIN, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
		
/*-------------------------------------------------------------------------------------*/
}
/**
  * @brief  CreateBotWin�������ײ��Ĵ���
  * @param  none
  * @retval none
  */
static void CreateBotWin(void)
{
	WinPara.hWinBot= WM_CreateWindowAsChild(
											0,											
											480-60,	//�ײ�λ��
											800,
											60,	//�ײ�ʣ����
											WM_HBKWIN, 
											WM_CF_SHOW,
											_cbBotWin, 
											0
											);
}
/*
*********************************************************************************************************
*
*       _cbGetData
*
* Function description
*   Callback function for getting font data
*
* Parameters:
*   Off      - Position of XBF file to be read
*   NumBytes - Number of requested bytes
*   pVoid    - Application defined pointer
*   pBuffer  - Pointer to buffer to be filled by the function
*
* Return value:
*   0 on success, 1 on error
*********************************************************************************************************
*/
//static int _cbGetData(U32 Offset, U16 NumBytes, void * pVoid, void * pBuffer)
//{
//	SPI_FLASH_BufferRead(pBuffer,Offset,NumBytes);
//	return 0;
//}

//static void _CreateXBF(void) 
//{
//	//
//	// Create XBF font
//	//
//	GUI_XBF_CreateFont(&XBF_Font,       // Pointer to GUI_FONT structure in RAM
//					   &XBF_Data,         // Pointer to GUI_XBF_DATA structure in RAM
//					   GUI_XBF_TYPE_PROP_EXT, 		// Font type to be created
//					   _cbGetData,        // Pointer to callback function
//					   0);        // Pointer to be passed to GetData function
//}
/*
*********************************************************************************************************
*	�� �� ��: emWinMainApp
*	����˵��: GUI������ 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void emWinMainApp(void)
{
	app_prevent_refresh = 0;
	//׼������3�����ڣ�������ʹ�õ����û���������������ڻص�������ʹ��
	WinPara.xSizeLCD = LCD_GetXSize();				//LCD��Ļ�ߴ�
	WinPara.ySizeLCD = LCD_GetYSize();				//LCD��Ļ�ߴ�
	WinPara.xSizeBM  = ICONVIEW_Width;				//ͼ����
	WinPara.ySizeBM  = ICONVIEW_Height;				//ͼ��߶�
	WinPara.ySizeBotWin=BOTWIN_YSpace;				//�����·����ڸ߶�
	WinPara.xPosWin	 = MIDWIN_xPos;							//���ڵ���ʼλ��
	WinPara.yPosWin  = MIDWIN_yPos;							//���ڵ���ʼλ��
	WinPara.xSizeWin = WinPara.xSizeLCD;						//���ڳߴ�
	WinPara.ySizeWin = WinPara.ySizeLCD-WinPara.ySizeBotWin-WinPara.yPosWin;	//���ڳߴ�
	
	/***************************����Ƥ��ɫ*****************************/
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	/***************************����Ĭ������**********************************/
//	_CreateXBF();
//	GUI_UC_SetEncodeUTF8();
//  GUI_SetDefaultFont(&GUI_FontHZ32);
	/*************************************************************************/
	WM_SetCallback(WM_HBKWIN, _cbBkWindow);
	WM_CreateTimer(WM_HBKWIN, 0, 1000, 0);
	/* ������������ ״̬������������������*/
	CreateMidWin();
	CreateTopWin();	
	CreateBotWin();
	//WM_InvalidateWindow(WM_HBKWIN);
	while(1)
	{
		GUI_Delay(5);
	}
}
/*************************** End of file ****************************/
