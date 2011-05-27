//
// C++ Interface: my_codeurs
//
// Description: 
//
//
// Author: Thomas Kaczmarek,,, <tomk@tomk-laptop>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MY_CODEURS_H
#define MY_CODEURS_H

#ifndef N_CODEURS
#define N_CODEURS 2
#endif

typedef struct codeur{
	signed long int tics;
	char prevA;
	char prevB;
}Codeur;

volatile Codeur codeur[N_CODEURS];

void refresh_codeur(Codeur* cd, char A, char B);
void init_codeur(Codeur* cd, char A, char B);



#endif