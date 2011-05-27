/*
*  C Implementation: my_codeurs
*
* Description: 
*
*
* Author: Thomas Kaczmarek,,, <tomk@tomk-laptop>, (C) 2010
*
* Copyright: See COPYING file that comes with this distribution
*
*/
#include "my_codeurs.h"
#include "my_io.h"
void refresh_codeur(Codeur* cd, char A, char B)
{
	//my_printf("codeur\n");
		
	if(!(cd->prevA^B))
	{
	//	my_printf("++\n");
		cd->tics++;
	}
	else
	{
	//	my_printf("--\n");
		cd->tics--;
	}
	cd->prevA=A;
	cd->prevB=B;
}

void init_codeur(Codeur* cd, char A, char B)
{
	cd->prevA=A;
	cd->prevB=B;
	cd->tics=0;
}

