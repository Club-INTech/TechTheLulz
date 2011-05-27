#include "my_io.h"
#include <stdarg.h>

void my_print_long(long int l)
{
	if(l==0)
	{
		my_putc('0');
		return;
	}
	//pour le debug
	int k=0;
	long nyu=l;
	long r=1000000000L;
	//printf("\nr=%lX\n",r);
	if(nyu<0L)
	{
		my_putc('-');
		nyu=-nyu;
	}
	while(r!=0&&nyu/r==0)
	{
		//my_putc('|');
		//nyu%=r;
		r/=10;
	}
	while(r!=0)
	{
		my_putc('0'+my_abs(nyu/r));
		nyu%=r;
		r/=10;
		k++;
	}
}

//version bricolée de my_print_long. Affiche les 0 de gauche et pas ceux de droite.
void my_print_decimal_part(long int l)
{
	if(l==0)
	{
		my_putc('0');
		return;
	}
	long nyu=l;
	long r=100000000L;
	while(r!=0)
	{
		my_putc('0'+(unsigned int)(nyu/r));
		nyu%=r;
		if(nyu==0)
			return;
		r/=10;
	}
}

void my_print_double(double f)
{
	if(f<0.0)
		if(f>=-1.0)
			my_putc('-');
	long int l=(long int)f;			//extraction de la partie entiere
	my_print_long(l);
	my_putc('.');
	l=(long int)((f-(double)l)*1000000000.0);	//extraction de la partie decimale
	l=l<0?-l:l;				//aussi connu sous le nom de l=abs(l);
	my_print_decimal_part(l);
}

void my_printf(const char* str,...)
{
	va_list ap;
	char* c=(char*)str;
	va_start(ap,str);
	while(*c!='\0')
	{
		if(*c!='%')
		{
			my_putc(*c);
			c++;
		}
		else
		{
			c++;
			if(*c=='d')
			{
				c++;
				my_print_long((long int)va_arg(ap,int));
			}
			if(*c=='c')
			{
				c++;
				//my_putc((char)va_arg(ap,char));
			}
			if(*c=='f')
			{
				c++;
				my_print_double(va_arg(ap,double));
			}
			if(*c=='l')
			{
				c++;
				if(*c=='d')
				{
					c++;
					my_print_long(va_arg(ap,long int));
				}
				if(*c=='f')
				{
					c++;
					my_print_double(va_arg(ap,double));
				}
			}
		}
	}
	va_end(ap);
}