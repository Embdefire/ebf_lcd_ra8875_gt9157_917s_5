/*********************************************************************
*
*	��̳    :http://www.firebbs.cn
*	�Ա�    :http://fire-stm32.taobao.com
*	����	:wildfireteam@163.com
*
*
**********************************************************************
*/
/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.22                          *
*        Compiled Jul  4 2013, 15:16:01                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END
#include "includes.h"
#include  "app.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/



// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreateLED[] = {
  { FRAMEWIN_CreateIndirect, "LED", 0, 0, 0, 800, 480, 0, 0x64, 0 },
  { CHECKBOX_CreateIndirect, "LED1", GUI_ID_CHECK0, 80, 60, 120, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED2", GUI_ID_CHECK1, 280, 60, 120, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "LED3", GUI_ID_CHECK2, 480, 60, 120, 30, 0, 0x0, 0 },
  { SLIDER_CreateIndirect, "LED_Slider", GUI_ID_SLIDER0, 60, 200, 550, 60, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialogLED(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  uint16_t i,v;
  // USER START (Optionally insert additional variables)
  // USER END
  switch (pMsg->MsgId) {
	  case WM_DELETE:
		    OS_DEBUG("LEDapp delete\n");
		    OnICON0 = 0;
			app_prevent_refresh = 0;
			LED1_OFF;LED2_OFF;LED3_OFF;
		    break;
	  case WM_INIT_DIALOG:
		//
		// Initialization of 'LED TEST'
		//
	    hItem = pMsg->hWin;
		FRAMEWIN_SetTextColor(hItem, GUI_DARKGRAY);
		FRAMEWIN_SetFont(hItem, GUI_FONT_32B_ASCII);
		FRAMEWIN_SetTitleHeight(hItem, 40);
		FRAMEWIN_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);
		//
		// Initialization of 'LED1'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_CHECK0);
		CHECKBOX_SetText(hItem, "LED1");
		CHECKBOX_SetFont(hItem, GUI_FONT_24B_ASCII);
		//
		// Initialization of 'LED2'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_CHECK1);
		CHECKBOX_SetText(hItem, "LED2");
		CHECKBOX_SetFont(hItem, GUI_FONT_24B_ASCII);
		//
		// Initialization of 'LED3'
		//
		hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_CHECK2);
		CHECKBOX_SetText(hItem, "LED3");
		CHECKBOX_SetFont(hItem, GUI_FONT_24B_ASCII);
		
		hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_SLIDER0);
        SLIDER_SetRange(hItem, 10,200);
        //SLIDER_SetValue(hItem, 500);
		// USER START (Optionally insert additional code for further widget initialization)
		// USER END
		break;
	  case WM_NOTIFY_PARENT:
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
		case GUI_ID_CHECK0: // Notifications sent by 'LED1'
		  switch(NCode){
			  case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				break;
			  case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
			    LED1_TOGGLE;
				break;
			  case WM_NOTIFICATION_VALUE_CHANGED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END			    
				break;
		  }
		  break;
		case GUI_ID_CHECK1: // Notifications sent by 'LED2'
		  switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
			// USER START (Optionally insert code for reacting on notification message)
			// USER END
			break;
		  case WM_NOTIFICATION_RELEASED:
			// USER START (Optionally insert code for reacting on notification message)
			// USER END
			LED2_TOGGLE;
			break;
		  case WM_NOTIFICATION_VALUE_CHANGED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				break;
		  }	  
		  break;
		case GUI_ID_CHECK2: // Notifications sent by 'LED3'
		  switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
			// USER START (Optionally insert code for reacting on notification message)
			// USER END
			break;
		  case WM_NOTIFICATION_RELEASED:
			// USER START (Optionally insert code for reacting on notification message)
			// USER END
			LED3_TOGGLE;
			break;
		  case WM_NOTIFICATION_VALUE_CHANGED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END			    
				break;
		  }
		  break;
		case GUI_ID_SLIDER0: // Notifications sent by 'LED_Slider'
		  switch(NCode) 
		  {
			  case WM_NOTIFICATION_CLICKED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				break;
			  case WM_NOTIFICATION_RELEASED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_SLIDER0);
				v = SLIDER_GetValue(hItem);
				WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_CHECK0));
				WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_CHECK1));
				WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_CHECK2));
			    WM_DisableWindow(hItem);
			    //printf("delay time=%d\n",v);
				i=20;
				while(i--)
				{
					LED1_ON;
					GUI_Delay(v);
					LED1_OFF;
					LED2_ON;
					GUI_Delay(v);
					LED2_OFF;
					LED3_ON;
					GUI_Delay(v);
					LED3_OFF;
					if(OnICON0==0)return;
				}
				WM_EnableWindow(hItem);
				WM_EnableWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_CHECK0));
				WM_EnableWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_CHECK1));
				WM_EnableWindow(WM_GetDialogItem(pMsg->hWin, GUI_ID_CHECK2));
				LED1_OFF;LED2_OFF;LED3_OFF;
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, GUI_ID_CHECK0),0);
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, GUI_ID_CHECK1),0);
				CHECKBOX_SetState(WM_GetDialogItem(pMsg->hWin, GUI_ID_CHECK2),0);
				break;
				
			  case WM_NOTIFICATION_VALUE_CHANGED:
				// USER START (Optionally insert code for reacting on notification message)
				// USER END
				break;
		  }
		  break;
		// USER START (Optionally insert additional code for further Ids)
		// USER END
		}
		break;
	  // USER START (Optionally insert additional message handling)
		case WM_PAINT:	
			GUI_SetBkColor(APPBKCOLOR);
			GUI_SetColor(APPTEXTCOLOR);
			GUI_Clear();
			GUI_SetFont(&GUI_FontHZ32);
			GUI_DispStringHCenterAt("Led�������",340,10);
			GUI_DispStringHCenterAt("��ˮ��",340,170);
			break;
	  // USER END
	  default:
		WM_DefaultProc(pMsg);
		break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateLED TEST
*/
void OnICON0Clicked(void)
{

	GUI_CreateDialogBox(_aDialogCreateLED, GUI_COUNTOF(_aDialogCreateLED), _cbDialogLED, WM_HBKWIN, 0, 0);
	OS_DEBUG("LEDapp create\n");
	while(OnICON0)
	{					 				
        GUI_Delay(10); 				
	}
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
