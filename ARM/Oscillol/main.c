/*
 * A skeleton main.c
 * Add your own code!
 */

// Load CMSIS and peripheral library and configuration
#include "stm32f10x.h"
#include "my_io.h"
#include "my_adc.h"
#include "my_usart.h"
#include "my_tools.h"



// Peripheral configuration functions
void GPIO_Config();


// A simple busy wait loop
void Delay ( volatile unsigned long delay );

//necessaire pour my_printf():
void my_putc ( char c )
{
	while ( USART_GetFlagStatus ( USART1, USART_FLAG_TC ) == RESET ) {}
	USART_SendData ( USART1, ( uint8_t ) c );
}

void scan_vmax ( long int* vmax,int nbloop )
{
	int chan;
	int v;

	for ( chan=0;chan<8;chan++ )
	{
		vmax[chan]=0;
	}
	int i;
	for ( i=0;i<nbloop;++i )
	{
		for ( chan=0;chan<8;chan++ )
		{
			v = GetADC1Channel ( chan ) ;
			if ( v>vmax[chan] )
				vmax[chan]=v;
		}
	}
	//my_printf ( "\n" );
}

int main ( void )
{

	// Setup STM32 system (clock, PLL and Flash configuration)
	SystemInit();

	// Setup the GPIOs
	GPIO_Config();


	InitADC1();      // ADC1 Init


	COMInit ( 1, 115200 );

	long int vmax[8];
	int bam_prec=0;
	for ( ;; )
	{


		// Turn on PB0, turn off PB1
		//GPIO_SetBits ( GPIOB, GPIO_Pin_8 );
		//GPIO_ResetBits ( GPIOB, GPIO_Pin_9 );
		//my_msleep ( 10 );
		// Turn off PB0, turn on PB1
		//GPIO_SetBits ( GPIOB, GPIO_Pin_9 );
		//GPIO_ResetBits ( GPIOB, GPIO_Pin_8 );
		//my_msleep ( 10 );
		//scan_vmax ( vmax,100 );
		/*int chan;
		for ( chan=0;chan<4;chan++ )
		{
			//my_printf ( "%d:%ld \t",chan,vmax[chan] );
			//my_printf ( "%ld ",GetADC1Channel ( chan ) );
		}*/
		
		int i;

		char k;
		/*for(i=0;i<10;++i)
			my_putc(0xAA);
		*/
		
		int vall[4];
		for(i=0;i<4;++i)
		{vall[i] = GetADC1Channel ( i );
		}
		for(i=0;i<4;++i)
		{int v = vall[ i ];
			
		/*if(i==0){
		//	my_printf("%ld",v);
		//	my_putc(0xAA);
		}*/
  			if((v&0xFF)==0xFF)//valeur interdite, caractere de separation
   				v++;
  			my_putc((char)(v&0xFF));
  			my_putc((char)((v>>8)&0xFF));
  			k*=v;
		}
		my_putc(k);
		my_putc(0xFF);

		
		/*if ( vmax[1]>100 )
		{
			if ( bam_prec<=0 )
			{
				bam_prec=2;
				my_printf ( "BAM!\n\n" );
			}
		}
		else
		{
			if ( bam_prec>0 )
				bam_prec--;
		}*/
		//my_printf("bp:%d, v:%d\n",bam_prec,vmax[1]);
		//my_printf ( "\n" );
	}
}


void GPIO_Config()
{

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );

}
