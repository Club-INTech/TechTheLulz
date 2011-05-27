/*
*  C Implementation: my_adc
*
* Description: 
*
*
* Author: Thomas Kaczmarek,,, <tomk@tomk-laptop>, (C) 2010
*
* Copyright: See COPYING file that comes with this distribution
*
*/
//#ifdef MY_ADC
#include "stm32f10x.h"
#include "my_adc.h"

void InitADC1_Only4 ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef   ADC_InitStructure;

	// ADC init
	// ADC Deinit
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_ADC1, ENABLE );
	ADC_DeInit ( ADC1 );

	// PA1 - analog input
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = ( GPIOSpeed_TypeDef ) 0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );

	// ADC Structure Initialization
	ADC_StructInit ( &ADC_InitStructure );

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 4;
	ADC_Init ( ADC1, &ADC_InitStructure );

	// Enable the ADC
	ADC_Cmd ( ADC1, ENABLE );

	// ADC calibration
	// Enable ADC1 reset calibaration register
	ADC_ResetCalibration ( ADC1 );

	// Check the end of ADC1 reset calibration register
	while ( ADC_GetResetCalibrationStatus ( ADC1 ) == SET );

	// Start ADC1 calibaration
	ADC_StartCalibration ( ADC1 );

	// Check the end of ADC1 calibration
	while ( ADC_GetCalibrationStatus ( ADC1 ) == SET );

}
void InitADC1 ( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef   ADC_InitStructure;

	// ADC init
	// ADC Deinit
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_ADC1, ENABLE );
	ADC_DeInit ( ADC1 );

	// PA1 - analog input
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = ( GPIOSpeed_TypeDef ) 0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );

	// ADC Structure Initialization
	ADC_StructInit ( &ADC_InitStructure );

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 8;
	ADC_Init ( ADC1, &ADC_InitStructure );

	// Enable the ADC
	ADC_Cmd ( ADC1, ENABLE );

	// ADC calibration
	// Enable ADC1 reset calibaration register
	ADC_ResetCalibration ( ADC1 );

	// Check the end of ADC1 reset calibration register
	while ( ADC_GetResetCalibrationStatus ( ADC1 ) == SET );

	// Start ADC1 calibaration
	ADC_StartCalibration ( ADC1 );

	// Check the end of ADC1 calibration
	while ( ADC_GetCalibrationStatus ( ADC1 ) == SET );

}

unsigned int GetADC1Channel ( unsigned char chanel )
{
	// Configure channel
	ADC_RegularChannelConfig ( ADC1, chanel, 1, ADC_SampleTime_55Cycles5 );

	// Start the conversion
	ADC_SoftwareStartConvCmd ( ADC1, ENABLE );

	// Wait until conversion completion
	while ( ADC_GetFlagStatus ( ADC1, ADC_FLAG_EOC ) == RESET );

	// Get the conversion value
	return ADC_GetConversionValue ( ADC1 );
}

//#endif
