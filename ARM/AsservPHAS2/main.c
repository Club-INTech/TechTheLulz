/*
 * A skeleton main.c
 * Add your own code!
 */

// Load CMSIS and peripheral library and configuration
#include "stm32f10x.h"
#include "my_io.h"
#define MY_ADC
#include "my_adc.h"
#include "my_usart.h"
#include "my_tools.h"
//#include "hw_config.h"
#include "stm32f10x_it.h"
volatile char my_buffer[1000];
volatile int buf_ptr=0;
//#include "my_codeurs.h"
#define FLOAT double
void filtres_codeurs();
FLOAT codeur_filtred1;
FLOAT codeur_filtred2;

#include "calibrations.h"

#include "asservPhase.lol"

#define BUFFERISER

void Init_bufa();

char rx_buffer1[BUF_LEN]="";
char rx_buffer2[BUF_LEN]="";
char* rx_buffer[2]={rx_buffer1,rx_buffer2};
int buf_cur=0;
int write_buf=0;
int recu=0;

int message_en_attente=0;

void traiter_message ( char* c );

char record[5000];
int rcur=0;

int couper=0;


//#define calibr_G calibr_1
//#define calibr_D calibr_2


void init_PWM()
{
	RCC_APB1PeriphClockCmd ( RCC_APB1Periph_TIM3, ENABLE );

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	TIM_OCInitTypeDef  TIM_OCInitStructure;



	TIM_TimeBaseStructure.TIM_Prescaler = 75;

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseStructure.TIM_Period = 4095;

	TIM_TimeBaseStructure.TIM_ClockDivision = 0;

	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;



	TIM_TimeBaseInit ( TIM3, &TIM_TimeBaseStructure );



	/* Channel 1, 2,3 and 4 Configuration in PWM mode */

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;

	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;

	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

	//TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;

	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;

	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;


	TIM_OCInitStructure.TIM_Pulse = 3080;

	TIM_OC1Init ( TIM3, &TIM_OCInitStructure );



	TIM_OCInitStructure.TIM_Pulse = 2047;

	TIM_OC2Init ( TIM3, &TIM_OCInitStructure );



	/* TIM3 counter enable */

	TIM_Cmd ( TIM3, ENABLE );



	/* TIM3 Main Output Enable */

	TIM_CtrlPWMOutputs ( TIM3, ENABLE );

}

#ifdef BUFFERISER
void my_buf ( const char* s )
{
	while ( *s!='\0' )
	{
		my_buffer[buf_ptr]=* ( s );
		buf_ptr++;
		s++;
		if ( buf_ptr>=9999 )
			buf_ptr=0;
	}
	my_buffer[buf_ptr]='\0';
}
#else
volatile void my_buf ( char* s )
{
	my_printf ( s );
}
#endif

// Peripheral configuration functions
void GPIO_Config();


// A simple busy wait loop
void Delay ( volatile unsigned long delay );

//necessaire pour my_buf():
void my_putc ( char c )
{
	while ( USART_GetFlagStatus ( USART1, USART_FLAG_TC ) == RESET ) {}
	USART_SendData ( USART1, ( uint8_t ) c );
}
/*
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
			v = GetADC1Channel ( chan );
			if ( v>vmax[chan] )
				vmax[chan]=v;
		}
	}
	my_buf ( "\n" );
}
 */
void RCC_roflol()
{
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig ( RCC_HSE_ON );

	/* Wait till HSE is ready */
	int	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if ( HSEStartUpStatus == SUCCESS )
	{
		/* Enable Prefetch Buffer */
		//		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* Flash 2 wait state */
		//		FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig ( RCC_SYSCLK_Div1 );

		/* PCLK2 = HCLK */
		RCC_PCLK2Config ( RCC_HCLK_Div1 );

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config ( RCC_HCLK_Div2 );

#ifdef STM32F10X_CL
		/* Configure PLLs *********************************************************/
		/* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
		RCC_PREDIV2Config ( RCC_PREDIV2_Div5 );
		RCC_PLL2Config ( RCC_PLL2Mul_8 );

		/* Enable PLL2 */
		RCC_PLL2Cmd ( ENABLE );

		/* Wait till PLL2 is ready */
		while ( RCC_GetFlagStatus ( RCC_FLAG_PLL2RDY ) == RESET )
		{}

		/* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */
		RCC_PREDIV1Config ( RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5 );
		RCC_PLLConfig ( RCC_PLLSource_PREDIV1, RCC_PLLMul_9 );
#else
		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig ( RCC_PLLSource_HSE_Div1, RCC_PLLMul_9 );
#endif

		/* Enable PLL */
		RCC_PLLCmd ( ENABLE );

		/* Wait till PLL is ready */
		while ( RCC_GetFlagStatus ( RCC_FLAG_PLLRDY ) == RESET )
		{
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig ( RCC_SYSCLKSource_PLLCLK );

		RCC_APB2PeriphClockCmd ( RCC_APB2Periph_AFIO, ENABLE );


		/* Wait till PLL is used as system clock source */
		while ( RCC_GetSYSCLKSource() != 0x08 )
		{
		}
	}
	else
	{
		while ( 1 );
	}
}



int main ( void )
{
	//my_buffer[9999]='\0';

	Init_bufa();



	// Setup STM32 system (clock, PLL and Flash configuration)
	SystemInit();




	//	InitADC1();      // ADC1 Init


	//Set_System();
	COMInit ( 1, 115200 );

	RCC_roflol();

	//my_buf ( "RESET\n" );


	int chan;
	int vmax[8];

	for ( chan=0;chan<8;chan++ )
	{
		vmax[chan]=0;
	}

	/*	//Map the external interrupts to port pins
		AFIO->EXTICR[0] = 0x00000000;
		//Enable External interrupt sources
		EXTI->IMR       = 0x00000001;
		//Enable wake up events
		EXTI->EMR       = 0x00000000;
		//Select falling edge trigger sources
		EXTI->FTSR      = 0x00000001;
		//Select Rising edge trigger sources
		EXTI->RTSR      = 0x00000000;
		//Enable interrupt sources in the NVIC
		NVIC->Enable[0] = 0x00000040;
		NVIC->Enable[1] = 0x00000000;
	 */
	// Setup the GPIOs
	GPIO_Config();


	/*

	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_ClearITPendingBit ( EXTI_Line0 );
	EXTI_ClearITPendingBit ( EXTI_Line1 );
	EXTI_ClearITPendingBit ( EXTI_Line2 );
	EXTI_ClearITPendingBit ( EXTI_Line3 );
	EXTI_InitStructure.EXTI_Line = EXTI_Line0|EXTI_Line1|EXTI_Line2|EXTI_Line3;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init ( &EXTI_InitStructure );


	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init ( &NVIC_InitStructure );

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init ( &NVIC_InitStructure );

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init ( &NVIC_InitStructure );


	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init ( &NVIC_InitStructure );

	init_PWM();

	//EXTI_GenerateSWInterrupt(EXTI_Line0);


//	my_printf("4 EXTI_IMR=%d\n",EXTI->IMR);

	// Enable the USARTx Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init ( &NVIC_InitStructure );
	 */
	InitADC1();
	USART_ITConfig ( USART1, USART_IT_RXNE, ENABLE );

	/*	init_codeur ( codeur,GPIO_ReadInputDataBit ( GPIOA, GPIO_Pin_0 ),GPIO_ReadInputDataBit ( GPIOA, GPIO_Pin_1 ) );
	init_codeur ( & ( codeur[1] ),GPIO_ReadInputDataBit ( GPIOA, GPIO_Pin_2 ),GPIO_ReadInputDataBit ( GPIOA, GPIO_Pin_3 ) );

	EXTI->IMR       = 0x0F;
	 */
	USART1->CR1|= ( 1<<4 );

	//my_printf ( "USART_CR1 : %d\n",USART1->CR1 );
	int k=0;
	int iteri=0;
	while ( 1 )
	{
		/*EXTI_ClearITPendingBit(EXTI_Line0);
		EXTI_GenerateSWInterrupt(EXTI_Line0);*/
		/*		if(!k--)
				{

					my_buf("Gros roflol \n");
					k=100000;
					//le_byte++;
				}
		 */
		//my_printf("bouton: %d, AFIO_EXTICR1=%d RCC_APB2ENR=%d, EXTI_IMR=%d\n",GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0), AFIO->EXTICR[0], RCC->APB2ENR,EXTI->IMR);
		//		my_printf("cG:%d, cD:%d, PA0:%d, PA1: %d, IMR:%d\n",codeur[0].tics,codeur[1].tics,GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0),GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1),EXTI->IMR);
		//		my_printf("cG:%d, cD:%d\n",codeur[0].tics,codeur[1].tics);

		// 		my_printf("x=%f y=%f angle=%f %d ",x,y,angle*180/3.14159265359,phase);
		//  		my_printf("c_r=%f c_t=%f\n",c_rot,c_trans);
		// 		my_printf("p_g=%d p_d=%d\n",(long int)(signe(c_g)*p_g),(long int)(signe(c_d)*p_d));

		// 		my_printf("P%d\n",phase);
		//my_printf ( "iteri%d\n", ( long ) ( iteri ) );

		refresh_codeurs();
		if(iteri++%10==0)
		{


			actualiser_coords();

		{
			my_printf ( "X%d#", ( long ) ( x*1000 ) );
			my_printf ( "Y%d#", ( long ) ( y*1000 ) );
			my_printf ( "A%d#\n", ( long ) ( angle*1000 ) );
		}
			if ( couper )
			{
				c_trans=0.0;
				c_rot=0.0;
				couper_moteurs=1;
			}
			else
			{
				calc_consignes();
			}
			app_consignes();


			//my_printf ( "SR=%d\n",USART1->SR );
			//my_printf ( ( char* ) my_buffer );
			//my_buffer[0]='\0';
			//buf_ptr=0;
			//my_msleep ( 500 );
		}
		if ( message_en_attente )
		{
			int read_buf= ( write_buf==0?1:0 );
			traiter_message ( rx_buffer[read_buf] );
		}

	}
}


void GPIO_Config()
{

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_11 |GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );

	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA, ENABLE );

	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );

}
long int lire_long ( char* s )
{
	int cur=0;
	char c=s[0];
	long nombre=0;
	long sign=1;
	if(c=='-')
	{
		sign=-1;
		cur++;
		c=s[cur];
	}
	while ( c!='\n' )
	{
		if ( c>='0'&&c<='9' )
		{
			nombre*=10;
			nombre+=sign*(c-'0');
		}
		else if ( c=='#' )
			break;
		cur++;
		c=s[cur];
	}
	return nombre;
}

double bufx=0.0;
double bufy=0.0;
double bufa=0.0;

#define PI 3.14159265359
FLOAT angle_align(FLOAT* calibr, FLOAT ang)
{
	int i=(CAL_SIZE-1)*(ang+PI)/(2*PI);
	//interpolation:
	FLOAT y=calibr[(int)i]+(calibr[(int)i+1]-calibr[(int)i])*(i-(int)i);
	//faster, no interpolation:
	//FLOAT y=calibr[i];
	return y;
}


#define BUFA_SIZE 50
FLOAT bufa1[BUFA_SIZE];
int bufa1_cur=0;
FLOAT bufa2[BUFA_SIZE];
int bufa2_cur=0;

void Init_bufa()
{
	int i;
	for(i=0;i<BUFA_SIZE;++i)
	{
		bufa1[i]=0;
		bufa2[i]=0;
	}
}

FLOAT angle1prec=0, angle2prec=0;
FLOAT total1prec=0, total2prec=0;
int iter=0;
void refresh_codeurs()
{
	///////////////////////// Codeur G 1
	{
		int x = GetADC1Channel ( 0 );
		int y = GetADC1Channel ( 1 );
		//	my_printf("lolol 1 %d %d\n", x,y);

		x-=MIN_X1+(MAX_X1-MIN_X1)/2;
		y-=MIN_Y1+(MAX_Y1-MIN_Y1)/2;

		FLOAT angle = atan2(x,y);
		angle=angle_align(calibr_1,angle);

		if(iter==0)angle1prec=angle;
		FLOAT diff = angle - angle1prec;
		angle1prec = angle;

		if(diff<PI)
			diff+=2*PI;
		if(diff>PI)
			diff-=2*PI;

		FLOAT newtot=total1prec+diff;
		bufa1[bufa1_cur++]=newtot;
		if(bufa1_cur>=BUFA_SIZE)
			bufa1_cur=0;
		total1prec=newtot;
	}
	///////////////////////// Codeur D 2
	{
	int x = GetADC1Channel ( 2 );
	int y = GetADC1Channel ( 3 );

	//	my_printf("x=%d  y=%d\n",x,y);

	x-=MIN_X2+(MAX_X2-MIN_X2)/2;
	y-=MIN_Y2+(MAX_Y2-MIN_Y2)/2;

	//	my_printf("scaled x=%d  y=%d\n",x,y);

	FLOAT angle = atan2(x,y);

	//	my_printf("angle %f\n",angle);

	angle=angle_align(calibr_2,angle);

	if(iter==0)angle2prec=angle;
	FLOAT diff = angle - angle2prec;
	angle2prec = angle;

	//	my_printf("align %f diff %f (angle2prec=%f)\n",angle,diff,angle2prec);

	if(diff<PI)
		diff+=2*PI;
	if(diff>PI)
		diff-=2*PI;

	FLOAT newtot=total2prec+diff;
	bufa2[bufa2_cur++]=newtot;
	if(bufa2_cur>=BUFA_SIZE)
		bufa2_cur=0;
	total2prec=newtot;
	}

	iter=1;
	//	if(iter++%1==0)
	//	{
	//	filtres_codeurs();
	//	my_printf("nyu %f  %f\n",codeur_filtred1,codeur_filtred2);
	//	}
	//	my_sleep(1);
}


void filtres_codeurs()
{
	int i;
	codeur_filtred1=0;
	codeur_filtred2=0;
	for(i=0;i<BUFA_SIZE;++i)
	{
		codeur_filtred1+=bufa1[i];
		codeur_filtred2+=bufa2[i];
	}
	codeur_filtred1/=BUFA_SIZE;
	codeur_filtred2/=BUFA_SIZE;

}

void traiter_char()
{
	char c = (USART_ReceiveData(USART1) & 0x7F);
	record[rcur++]=c;
	record[rcur]='\0';
	if(buf_cur<BUF_LEN)
	{
		rx_buffer[write_buf][buf_cur++] = c;
	}
	else
	{
		if(c=='#')
		{
			buf_cur=0;
			return;
		}
	}
	if(c=='#')
	{
		message_en_attente=1;
		buf_cur=0;
		write_buf=(write_buf==0?1:0);
	}
}

void traiter_message ( char* msg )
{
	char c=msg[0];

	if ( c=='?' )	//"Qui es-tu?", cette requete permet a l'hote d'identifier la carte. Reponse "?a" : "je suis la carte d'asservissement"
		my_printf ( "?a#" );
	else if ( c=='X' )	//Nouvelle consigne de position X (en cm)
	{
		bufx= ( double ) lire_long ( & ( msg[1] ) ) /100.0;
	}
	else if ( c=='Y' )	//Nouvelle consigne de position Y (en cm)
	{
		bufy= ( double ) lire_long ( & ( msg[1] ) ) /100.0;
	}
	else if ( c=='A' )	//Nouvelle consigne d'angle a l'arrivee (en rad/1000)
	{
		bufa= ( double ) lire_long ( & ( msg[1] ) ) /1000.0;
	}
	else if ( c=='G' )	//Appliquer les nouvelles consignes
	{
		c_x=bufx;
		c_y=bufy;
		c_angle=bufa;
	}
	else if ( c=='R')	//Requete des coordonnees actuelles.
	{
		recu+=1;
		if(recu)
			GPIO_WriteBit( GPIOB, GPIO_Pin_8, Bit_RESET);
		else
			GPIO_WriteBit( GPIOB, GPIO_Pin_8, Bit_SET);
		my_printf ( "X%d#", ( long ) ( x ) );
		my_printf ( "Y%d#", ( long ) ( y ) );
		my_printf ( "A%d#", ( long ) ( angle*1000 ) );
	}
	else if ( c=='C' ) //couper les moteurs
	{
		couper=1;
	}
	else if ( c=='S' ) //allumer les moteurs
	{
		couper=0;
	}
	else if ( c=='B' ) //allumer les moteurs
	{
		my_printf(record);
	}
	else if ( c=='P' ) //allumer les moteurs
	{
		my_printf("P%d#", phase);
	}
	else
	{
		traiter_message(&(msg[1]));
	}
	message_en_attente=0;
}
