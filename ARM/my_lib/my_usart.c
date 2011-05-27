
#include "stm32f10x.h"
#include "my_usart.h"


void COMInit ( int USART, unsigned long int BaudRate )
{
	
	USART_InitTypeDef USART_InitStruct;

	
	USART_InitStruct.USART_BaudRate = BaudRate;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	GPIO_InitTypeDef GPIO_InitStructure;

	if ( USART==1 )
	{
		/* Enable GPIO & USART clock */
		RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE );
		/* Configure USART1 Rx (PA10) as input floating */
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
		GPIO_Init ( GPIOA, &GPIO_InitStructure );

		/* Configure USART1 Tx (PA9) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
		GPIO_Init ( GPIOA, &GPIO_InitStructure );

		USART_Init ( USART1, &USART_InitStruct );
		USART_Cmd ( USART1, ENABLE ); // Enable USART1


	}
	else if ( USART==2 )
	{
		RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA , ENABLE );
		RCC_APB1PeriphClockCmd ( RCC_APB1Periph_USART2, ENABLE );

		/* Configure USART2 Rx (PA.3) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init ( GPIOA, &GPIO_InitStructure );

		/* Configure USART2 Tx (PA.2) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init ( GPIOA, &GPIO_InitStructure );

		USART_Init ( USART2, &USART_InitStruct );
		USART_Cmd ( USART2, ENABLE ); // Enable USART2
	}
}