/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "includes.h"
#include "usb_lib.h"
#include "usb_istr.h"

void bsp_USART2_IRQHandler(void);
// can 
__IO uint32_t ret = 0;

//Clock
char TimeDisplay=0;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  printf("HardFault\n");
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}
/**
  * @brief  This function handles RTC interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{

	  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	  {
	    /* Clear the RTC Second interrupt */
	    RTC_ClearITPendingBit(RTC_IT_SEC);	
		
		TimeDisplay = 1;

	    /* Wait until last write operation on RTC registers has finished */
	    RTC_WaitForLastTask();
	  }
}
/*
 * ��������SDIO_IRQHandler
 * ����  ����SDIO_ITConfig(���������������sdio�ж�	��
 *		     ���ݴ������ʱ�����ж�
 * ����  ����		 
 * ���  ����
 */
void SDIO_IRQHandler(void) 
{
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
}
/*
 * ��������USB_LP_CAN1_RX0_IRQHandler
 * ����  ��USB�����ȼ��ж�����
 * ����  ����		 
 * ���  ����
 */
#if 1
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
  
	if( CAN_GetITStatus(CAN1, CAN_IT_FMP0) == SET )
	{
		RxMessage.StdId=0x00;
		RxMessage.ExtId=0x00;
		RxMessage.IDE=0;
		RxMessage.DLC=0;
		RxMessage.FMI=0;
		RxMessage.Data[0]=0x00;
		RxMessage.Data[1]=0x00;
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

		if((RxMessage.ExtId==0x1234)  && 
		   (RxMessage.IDE==CAN_ID_EXT)&& 
		   (RxMessage.DLC==2)         && 
		   ((RxMessage.Data[1]|RxMessage.Data[0]<<8)==0xDECA))
		{
		  ret = 1; 
		}
		else
		{
		  ret = 0; 
		}
	}
	else
	{
		USB_Istr();
	}
}
#endif
/*
 * ��������USB_HP_CAN1_TX_IRQHandler
 * ����  ��USB�����ȼ��ж����� �����ж�
 * ����  ����		 
 * ���  ����
 */
void USB_HP_CAN1_TX_IRQHandler(void)
{
  CTR_HP();
}
/*
*����2�жϣ�����SIM900Aģ��
*/
void USART2_IRQHandler(void)
{
    bsp_USART2_IRQHandler();
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
