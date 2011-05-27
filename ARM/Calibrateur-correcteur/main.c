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
#include <math.h>



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

#define PROFIL_SIZE 1000
float profil_periode[PROFIL_SIZE];
long compte_moyennes[PROFIL_SIZE];

#define BUF_PERIODE_SIZE 2000
float buffer_periode[BUF_PERIODE_SIZE];
int cur_periode=0;

#define CODEURD

#ifdef CODEURG
#define MIN_X 73
#define MAX_X 1802
#define MIN_Y 101
#define MAX_Y 1746
#endif

#ifdef CODEURD
#define MIN_X 29
#define MAX_X 1000
#define MIN_Y 70
#define MAX_Y 1051
#endif
int main ( void )
{

	// Setup STM32 system (clock, PLL and Flash configuration)
	SystemInit();

	// Setup the GPIOs
	GPIO_Config();


	InitADC1();      // ADC1 Init


	COMInit ( 1, 115200 );
	
	
	int j;
	for(j=0;j<PROFIL_SIZE;++j)
	{
		profil_periode[j]=0.0;
		compte_moyennes[j]=0;
	}
	for(j=0;j<BUF_PERIODE_SIZE;++j)
	{
		buffer_periode[j]=0.0;
	}
	

	long periodes=0;
	int enregistrer=0;
	
	float angle,prevangle=0.0;
	int bam_prec=0;
	for ( ;; )
	{
#ifdef CODEURD
		int x = GetADC1Channel ( 2 );
		int y = GetADC1Channel ( 3 );
#endif
#ifdef CODEURG
		int x = GetADC1Channel ( 0 );
		int y = GetADC1Channel ( 1 );
#endif
		x-=MIN_X;
		y-=MIN_Y;
		
		x-=(MAX_X-MIN_X)/2;
		y-=(MAX_Y-MIN_Y)/2;
		
		float angle = atan2f(x,y);
		int startb=0;
			
		if(prevangle>2.5&&angle<-2.5)//Transition directe
		{
			if(enregistrer)
			{
			//mapper
				int i;
				int K;
				for(i=0;i<cur_periode;++i)
				{
					K = (long long)i*(long long)PROFIL_SIZE/cur_periode;
					profil_periode[K] = profil_periode[K]*compte_moyennes[K] + buffer_periode[i];
					compte_moyennes[K]++;
					profil_periode[K]/=compte_moyennes[K];
				}
				periodes++;
				if(periodes%1==0)
					my_printf("%ld, %ld\n",periodes, cur_periode);
				
				if(periodes>=10000)
				{
					//la fin
					my_printf("**********************************************\n");
					int i;
					for(i=0;i<PROFIL_SIZE;++i)
					{
					/*	float N1=profil_periode[i];
						float N2=0.0;
						if(i!=PROFIL_SIZE-1)
							N2=profil_periode[i+1];
						else
							N2=3.14159265359;
						int j=0;
						for(j=0;j<10;++j)
							my_printf("%f\n",N1+(N2-N1)*(j/10.0));
					*/
						my_printf("%f\n",profil_periode[i]);
					}
					my_printf("****DONE**************************************\n");
					while(42){}
				}
			
				enregistrer=0;
			}
			else
			{
				//my_printf("start %f -> %f\n",prevangle, angle);
				enregistrer = 1;
				startb=1;
				cur_periode=0;
			}
		}
		else if(prevangle>1.0&&angle<-1.0)//Transition directe sale
		{
			my_printf("sale %f -> %f\n",prevangle, angle);
			enregistrer=0;
		}
		
		
		if(enregistrer)
		{
		if(prevangle<-1.0&&angle>1.0)//Transition inverse
		{	
			//my_printf("inv %f -> %f\n",prevangle, angle);
			//enregistrer=0;
			angle = prevangle;
		}
		if(!startb)
		if(angle-prevangle<-.4)
		{	
			my_printf("inv %f -> %f\n",prevangle, angle);
			enregistrer=0;
		}
		
		if(enregistrer && cur_periode>=BUF_PERIODE_SIZE)
		{
			my_printf("long\n");
			enregistrer=0;
		}
		}
		if(enregistrer)
		{
			buffer_periode[cur_periode++]=angle;
		}
		//my_printf("%f\n",angle);
		prevangle=angle;
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
