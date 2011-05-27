/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : stm32f10x_it.c
* Author             : MCD Application Team
* Version            : V3.1.0
* Date               : 10/30/2009
* Description        : Main Interrupt Service Routines.
*                      This file provides template for all exceptions handler
*                      and peripherals interrupt service routine.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
//#include "usb_istr.h"
//#include "usb_lib.h"
//#include "usb_pwr.h"
//#include "platform_config.h"
//#include "stm32_eval.h"
#include "my_io.h"
#include "my_codeurs.h"


void USART1_IRQHandler(void)

{
	USART_GetFlagStatus(USART1,0xFFFF);
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)

	{
		traiter_char();
	}
	else
	{
		USART_ReceiveData(USART1);
	}
	//my_printf("!\n");
	//USART_ClearFlag(USART1, USART_FLAG_RXNE);
}


extern void my_buf(const char*);

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{
	my_printf("*NMI*\n");
}

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
	my_printf("*HF*\n");
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManage_Handler
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManage_Handler(void)
{
	my_printf("*MM*\n");
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFault_Handler
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFault_Handler(void)
{
	my_printf("*BF*\n");
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFault_Handler
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFault_Handler(void)
{
	my_printf("*UF*\n");
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : SVC_Handler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVC_Handler(void)
{
	my_printf("*SVC*\n");
}

/*******************************************************************************
* Function Name  : DebugMon_Handler
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMon_Handler(void)
{
}

/*******************************************************************************
* Function Name  : PendSV_Handler
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSV_Handler(void)
{
	my_printf("*PSV*\n");
}

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

#ifndef STM32F10X_CL
/*******************************************************************************
* Function Name  : USB_LP_CAN1_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void USB_LP_CAN1_RX0_IRQHandler(void)
//{
  //USB_Istr();
//}
#endif /* STM32F10X_CL */

/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI0_IRQHandler(void)
{
//	my_printf("inter1\n");
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		refresh_codeur(codeur,GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0),GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}
void EXTI1_IRQHandler(void)
{
//	my_printf("inter2\n");
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		refresh_codeur(codeur,GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0),GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1));
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}
Codeur* codeur1=&(codeur[1]);
void EXTI2_IRQHandler(void)
{
//	my_printf("inter3\n");
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		refresh_codeur(codeur1,GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2),GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3));
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}
void EXTI3_IRQHandler(void)
{
//	my_printf("inter4\n");
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		refresh_codeur(codeur1,GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2),GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3));
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}
void EXTI9_5_IRQHandler(void)
{
	/*	my_buf("lmao\n");
		EXTI_ClearITPendingBit(EXTI_Line0);*/
//   if (EXTI_GetITStatus(KEY_BUTTON_EXTI_LINE) != RESET)
//   {
//     /* Check if the remote wakeup feature is enabled (it could be disabled 
//         by the host through ClearFeature request) */
//     if (pInformation->Current_Feature & 0x20) 
//     {      
//       /* Exit low power mode and re-configure clocks */
//       Resume(RESUME_INTERNAL);
//       
//   #ifdef STM32F10X_CL
//       /* Send resume signaling to the host */    
//       Resume(RESUME_ESOF);
//       
//       mDELAY(3);
//       /* Stop resume signaling to the host */
//       Resume(RESUME_ESOF);
//   #endif /* STM32F10X_CL */
//     }
//   
//     /* Clear the EXTI line pending bit */
//     EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);
//   }
}
/*
#ifdef STM32F10X_CL
/*******************************************************************************
* Function Name  : OTG_FS_WKUP_IRQHandler
* Description    : This function handles OTG WakeUp interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*void OTG_FS_WKUP_IRQHandler(void)
{
  /* Initiate external resume sequence (1 step) */
 /* Resume(RESUME_EXTERNAL);  

  EXTI_ClearITPendingBit(EXTI_Line18);
}
#else 
/*******************************************************************************
* Function Name  : USBWakeUp_IRQHandler
* Description    : This function handles USB WakeUp interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*void USBWakeUp_IRQHandler(void)
{
  //EXTI_ClearITPendingBit(EXTI_Line18);
}
#endif*/ /* STM32F10X_CL */
void refresh_codeurs();
void TIM3_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET)
  {
	  refresh_codeurs();
  }
}
#ifdef STM32F10X_CL
/*******************************************************************************
* Function Name  : OTG_FS_IRQHandler
* Description    : This function handles USB-On-The-Go FS global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void OTG_FS_IRQHandler(void)
//{
  //STM32_PCD_OTG_ISR_Handler(); 
//}
#endif /* STM32F10X_CL */

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/*******************************************************************************
* Function Name  : PPP_IRQHandler
* Description    : This function handles PPP interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*void PPP_IRQHandler(void)
{
}*/

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

