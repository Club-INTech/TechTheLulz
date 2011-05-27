#include "my_tools.h"

#define MAX_LONG_1000 4294967UL
#define MAX_LONG_1000b 4294967000UL

void my_sleep ( volatile unsigned long Seconds )
{
	while(Seconds>MAX_LONG_1000)
	{
		my_msleep(MAX_LONG_1000b);
		Seconds-=MAX_LONG_1000;
	}
	my_msleep(Seconds*1000UL);
}

void my_msleep ( volatile unsigned long MilliSeconds )
{
	while(MilliSeconds>MAX_LONG_1000)
	{
		my_usleep(MAX_LONG_1000b);
		MilliSeconds-=MAX_LONG_1000;
	}
	my_usleep(MilliSeconds*1000UL);
}

#define MAX_LONG_6 715827882UL
#define MAX_LONG_6b 4294967292UL

void my_usleep ( volatile unsigned long MicroSeconds )
{
	while(MicroSeconds>MAX_LONG_6)
	{
		my_delay(MAX_LONG_6b);
		MicroSeconds-=MAX_LONG_6;
	}
	my_delay(MicroSeconds*6UL);
}

void my_delay ( volatile unsigned long delay )
{
	for ( ; delay; --delay );
}

long int ctol(char c)
{
	return (long int)c;
}

int my_abs ( int x )
{
	return x<0?-x:x;
}

 
double angle_modulo(double a)//mettre entre -PI et PI
{
	while(a>PI)
		a-=2*PI;
	while(a<-PI)
		a+=2*PI;
	return a;
}
 
double pic(double a, double seuil)
{
	if(abs(a)>seuil)
		return 0.0;
	return (seuil-abs(a))/seuil;
}

double satur(double a, double seuil)
{
	//my_printf("a=%f seuil=%f",a,seuil);
	if(a>seuil)
	{
	//	my_printf("seuil\n");
		return seuil;
	}
	//my_printf("a\n");
	return a;
}


int signe(double a)
{
	return a>=0.0?1:-1;
}
