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
**/ 


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "includes.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 TCB
*********************************************************************************************************
*/

static  OS_TCB		AppTaskStartTCB;
static	OS_TCB		AppTaskScreenShotTCB;
static	OS_TCB		AppTaskGuiKeyPadTCB;
static	OS_TCB		AppTaskGuiTCB;


/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

__align(8)	static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];
static  CPU_STK  AppTaskScreenShotStk[APP_TASK_SCREENSHOT_STK_SIZE];
static  CPU_STK  AppTaskGuiKeyPadStk[APP_TASK_GUIKEYPAD_STK_SIZE];
static  CPU_STK  AppTaskGuiStk[APP_TASK_GUI_STK_SIZE];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskCreate (void);
static  void  AppObjCreate  (void);
static  void  AppTaskStart  (void *p_arg);
static	void  AppTaskScreenShot(void *p_arg);
static	void  AppTaskGuiKeyPad(void *p_arg);
static	void  AppTaskGui(void *p_arg);
static void cbTimer(OS_TMR *p_tmr, void *p_arg);

static  OS_SEM     	SEM_KEY2;
KEY Key2;	
static 	uint8_t		Pic_Name = 0;
__IO uint8_t key_flag=1;//1����������ͼ��0�����ð�����ͼ
/*
************************************************************************
*						  FatFs
************************************************************************
*/
FRESULT result;
FIL file;
UINT bw;

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  err;
    OSInit(&err);                                               /* Init uC/OS-III.                                      */
	
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    OS_ERR      err;
	OS_TMR      MyTmr;
   (void)p_arg;	
	
    BSP_Init();                                                 /* Initialize BSP functions                             */
	CPU_Init();

    //Mem_Init();                                                 /* Initialize Memory Management Module                  */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

	OS_DEBUG_FUNC();
    
	OS_INFO("Creating Application Tasks...\n\r");
    AppTaskCreate();                                            /* Create Application Tasks                             */
    
    OS_INFO("Creating Application Events...\n\r");
    AppObjCreate();                                             /* Create Application Objects                           */
    

	OSTmrCreate((OS_TMR              *)&MyTmr,
               (CPU_CHAR            *)"MyTimer",          
               (OS_TICK              )2,                
               (OS_TICK              )2,                
               (OS_OPT               )OS_OPT_TMR_PERIODIC,
               (OS_TMR_CALLBACK_PTR  )cbTimer,          
               (void                *)0,
               (OS_ERR              *)err);
  
	OSTmrStart((OS_TMR *)&MyTmr,(OS_ERR *)err);
	
	OSTaskDel(&AppTaskStartTCB,&err);	
//    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
//		OSTimeDlyHMSM(0, 0, 5, 0,
//                      OS_OPT_TIME_HMSM_STRICT,
//                      &err);
//    }
}


//������ʱɨ��Ķ�ʱ��
static void cbTimer(OS_TMR *p_tmr, void *p_arg)
{
	OS_ERR      err;
	(void)p_arg;
	GT9xx_GetOnePiont();//��������ʱɨ��

	/*������ʱɨ��*/
	Key_RefreshState(&Key2);//ˢ�°���״̬
	if(Key_AccessTimes(&Key2,KEY_ACCESS_READ)!=0 && key_flag)//���������¹�
	{
		LED3_TOGGLE;
		OSSemPost ((OS_SEM  *)&SEM_KEY2,
               (OS_OPT   )OS_OPT_POST_ALL,
               (OS_ERR  *)&err);
		Key_AccessTimes(&Key2,KEY_ACCESS_WRITE_CLEAR);
	 }
}

/*
*********************************************************************************************************
*	�� �� ��: _WriteByte2File()
*	����˵��: 
*	��    �Σ�  	
*	�� �� ֵ: 
*********************************************************************************************************
*/
void _WriteByte2File(uint8_t Data, void * p) 
{
	result = f_write(p, &Data, 1, &bw);
}

/*
*********************************************************************************************************
*                                          APP TASK FUN
*
* Description : 
*
* Arguments   : p_arg   is the argument passed to 'AppTask***()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
static	void  AppTaskScreenShot(void *p_arg)
{
	OS_ERR  err;
	CPU_TS 	Ts;
	char 	buf[20];
	(void)p_arg;
	OS_DEBUG_FUNC();
	while(1)
	{
		//��������ֱ�����������£�
		OSSemPend((OS_SEM   *)&SEM_KEY2,
						 (OS_TICK   )0,                     //��������������Ϊ0��һֱ�ȴ���ȥ
						 (OS_OPT    )OS_OPT_PEND_BLOCKING,  //���û���ź������þ͵ȴ�
						 (CPU_TS   *)&Ts,             //���������ָ�����ź������ύ��ǿ�ƽ���ȴ�״̬�������ź�����ɾ����ʱ���        
						 (OS_ERR   *)&err);
		sprintf(buf,"0:/picture%d.bmp",Pic_Name);
		OSSchedLock(&err);
		printf("start shot picture\n\r");
		/* ������ͼ */
		result = f_open(&file,buf, FA_WRITE|FA_CREATE_ALWAYS);
		/* ��SD������BMPͼƬ */
		GUI_BMP_Serialize(_WriteByte2File,&file);
		
		/* ������ɺ�ر�file */
		result = f_close(&file);
		printf("shot picture success\n\r");
		Pic_Name++;
		OSSchedUnlock(&err);
		OSTimeDlyHMSM(0, 0, 0, 10,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);		
	}
}

static	void  AppTaskGuiKeyPad(void *p_arg)
{
	OS_ERR      err;
	(void)p_arg;
	OS_DEBUG_FUNC();
	while(1)
	{
		//LED2_TOGGLE;
		OSTimeDlyHMSM(0, 0, 5, 0,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
	}
}

static	void  AppTaskGui(void *p_arg)
{
	(void)p_arg;
	OS_DEBUG_FUNC();
	while(1)
	{
		emWinMainApp();	
	}
}





/*
*********************************************************************************************************
*                                      CREATE APPLICATION TASKS
*
* Description:  This function creates the application tasks.
*
* Arguments  :  none
*
* Returns    :  none
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
	OS_ERR  err;
	OSTaskCreate((OS_TCB     *)&AppTaskScreenShotTCB,                /* Create the ScreenShot task                                */
                 (CPU_CHAR   *)"App Task Screen Shot",
                 (OS_TASK_PTR ) AppTaskScreenShot,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_SCREENSHOT_PRIO,
                 (CPU_STK    *)&AppTaskScreenShotStk[0],
                 (CPU_STK_SIZE) APP_TASK_SCREENSHOT_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_SCREENSHOT_STK_SIZE,
                 (OS_MSG_QTY  ) 0u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
	
	OSTaskCreate((OS_TCB     *)&AppTaskGuiKeyPadTCB,                /* Create the GuiKeyPad task                                */
                 (CPU_CHAR   *)"App Task GuiKeyPad",
                 (OS_TASK_PTR ) AppTaskGuiKeyPad,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_GUIKEYPAD_PRIO,
                 (CPU_STK    *)&AppTaskGuiKeyPadStk[0],
                 (CPU_STK_SIZE) APP_TASK_GUIKEYPAD_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_GUIKEYPAD_STK_SIZE,
                 (OS_MSG_QTY  ) 2u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
	
	OSTaskCreate((OS_TCB     *)&AppTaskGuiTCB,                /* Create the GuiKeyPad task                                */
                 (CPU_CHAR   *)"App Task Gui",
                 (OS_TASK_PTR ) AppTaskGui,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_GUI_PRIO,
                 (CPU_STK    *)&AppTaskGuiStk[0],
                 (CPU_STK_SIZE) APP_TASK_GUI_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_GUI_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
}


/*
*********************************************************************************************************
*                                      CREATE APPLICATION EVENTS
*
* Description:  This function creates the application kernel objects.
*
* Arguments  :  none
*
* Returns    :  none
*********************************************************************************************************
*/

static  void  AppObjCreate (void)
{
	OS_ERR      err;
	
	//����һ�������������ľ���ϸ��������Ȥ�ɲ���
	KeyCreate(&Key2,GetPinStateOfKey2);
	//����һ���ź������������µ�ʱ��postһ���ź�������������
	OSSemCreate ((OS_SEM      *)&SEM_KEY2,     //ָ���ź���������ָ��
			   (CPU_CHAR    *)"SEM_KEY2",    //�ź���������
			   (OS_SEM_CTR   )0,             //�ź���������ָʾ�¼����������Ը�ֵΪ0����ʾ�¼���û�з���
			   (OS_ERR      *)&err);         //��������
}

/* ------------------------------------------end of file---------------------------------------- */

