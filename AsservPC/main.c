#include <allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "gestion_carte_audio.c"

#define COURBE 0
#define DROIT 1

#define MODE DROIT
//#define MODE COURBE

#define saut_frame_point_de_position 	1
#define coeff_lenteur			0.01

BITMAP* buffer;
/////////////////////////////CODE COPIE ARDUINO

#define PI 3.14159265359

int p_g,p_d,s_g,s_d;

double c_x=6.50;
double c_y=3.50;
double c_angle=0;

int phase=1;

double c_rot=0;
double c_trans=0;

float dist(float x1,float y1,float x2,float y2);
float angl(float x1,float y1,float x2,float y2);
void calc_consignes();
void app_consignes();

/////////////////////////////

double en_degres(double rad)
{
	return rad*180.0/PI;
}

typedef struct robot{
// données PC
	double x;
	double y;
	BITMAP* bmp;
	double angle;

// caracteristiques
	
	double espacement_roues;
	double diametre_roues;
	double ticks_par_tour;

// données arduino
	double delta_deplacement;
	double delta_angle;
	
	double x_reel;
	double y_reel;
}Robot;

Robot ttw;

void init()
{
	int depth, res;
	allegro_init();
	srand ( time ( 0 ) );
	
	depth = desktop_color_depth();
	if ( depth == 0 )
		depth = 32;
	set_color_depth ( depth );
	res = set_gfx_mode ( GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0 );
	/*if ( res == 0 )
	{
	allegro_message ( "Echec initialisation de l'affichage!" );
	exit ( -1 );
}*/

	//install_timer();
	set_alpha_blender();
	install_keyboard();
	install_mouse();

	/* add other initializations here */
}

#define delta_t .25

int max(int a, int b)
{
	return a>b?a:b;
}
int min(int a, int b)
{
	return a<b?a:b;
}

int main()
{
	// INITIALISATION
	init();
	buffer=create_bitmap(screen->w,screen->h);
	
	ttw.x=2.0;
	ttw.y=.350;
	
	ttw.x_reel=.10;
	ttw.y_reel=.10;
	
	ttw.angle=-PI/2;
	
	ttw.diametre_roues=0.10;
	ttw.ticks_par_tour=360.0;
	ttw.espacement_roues=0.38;
	
	ttw.delta_deplacement=0.0;
	ttw.delta_angle=0.0;
	
	ttw.bmp=load_bitmap("robot.bmp",NULL);
	BITMAP* cible=load_bitmap("cible_m.bmp",NULL);
	
	BITMAP* trace=create_bitmap(screen->w,screen->h);
	clear_to_color ( trace,makecol ( 255,255,255 ) );
	
	//BOUCLE
	
	int I=0;
	long int J=0;
	while(!key[KEY_ESC])
	{
//		printf("phase:%d\n",phase);
		if(J++>800) //on change d'objectif de temps en temps...
		{
			c_x=((double)rand()*5.0)/RAND_MAX;
			c_y=((double)rand()*5.0)/RAND_MAX;
			c_angle=((double)rand()*2*PI)/RAND_MAX-PI;
			J=0;
		}
		int consigneA=25, consigneT=50;
		int consigneD=min(consigneT+consigneA,255), consigneG=min(consigneT-consigneA,255);
		
		/////////////////////////////// INJECTION CODE ARDUINO
		calc_consignes();
		if(I==saut_frame_point_de_position-1)
			app_consignes();
		consigneG=p_g*s_g;
		consigneD=p_d*s_d;
		///////////////////////////////
		
		//...les consignes sont envoyées...
		
		
		
		//ttw.x=0;
		//ttw.y=0;
		
		//printf("c_rot=%f,c_trans=%f,consigneG=%d, consigneD=%d\n",c_rot,c_trans,consigneG,consigneD);
		
		//un pas de simulation : les roues tournent un peu, le robot bouge et change d'orientation
		
		//////////////////////////////////LOL
		/*double dD;
		dD = consigneD*delta_t*ttw.diametre_roues*coeff_lenteur;
		double dG;
		dG = consigneG*delta_t*ttw.diametre_roues*coeff_lenteur;

		ttw.delta_deplacement=(dD+dG)/2.0;
		ttw.delta_angle=(dD-dG)/2.0;
		
		ttw.angle+=ttw.delta_angle/2.0;
		ttw.x+=ttw.delta_deplacement*cos(ttw.angle);
		ttw.y+=ttw.delta_deplacement*sin(ttw.angle);
		ttw.angle+=ttw.delta_angle/2.0;//oui, encore.
		*/
		////////////////////////////////// /LOL
		
		ttw.x=odo_x;
		ttw.y=odo_y;
		ttw.angle=odo_a;
		
		//dessin à l'ecran
		if(I++>=saut_frame_point_de_position)
		{
			putpixel(trace,ttw.x*100,ttw.y*100,makecol(0,0,255));
			I=0;
			//while(key[KEY_SPACE]&&!key[KEY_ESC]){}
			//while(!key[KEY_SPACE]&&!key[KEY_ESC]){}
			//printf("\n********************************\n\n\n");
		}
		blit ( trace,buffer,0,0,0,0,screen->w,screen->h);
		rotate_sprite(buffer, ttw.bmp, ttw.x*100-ttw.bmp->w/2, ttw.y*100-ttw.bmp->h/2, ftofix((ttw.angle+1.5708)*128.0/3.14159265359));
		rotate_sprite(buffer, cible, c_x*100-cible->w/2, c_y*100-cible->h/2, ftofix((c_angle+PI/2)*128.0/PI));
		blit ( buffer,screen,0,0,0,0,screen->w,screen->h);
	}
	return EXIT_SUCCESS;
}

float dist_seuil_autorisation_marche_arriere=0.05;
float pas_accel=0.1;
float coefftrans1=20;
float coefftrans2=10;
float coeffrot=200.0;
float maxtrans=255;
float dist_seuil_avant_tourner=0.15;
float coeff_courbure_but=0.01;

double mabs(double a)
{
	return a>0.0?a:-a;
}

float angle_modulo(float a)//mettre entre -PI et PI
{
	while(a>PI)
		a-=2*PI;
	while(a<-PI)
		a+=2*PI;
	return a;
}

float pic(float a, float seuil)
{
	if(abs(a)>seuil)
		return 0.0;
	return (seuil-abs(a))/seuil;
}

float satur(float a, float seuil)
{
	if(a>seuil)
		return seuil;
	return a;
}

int signe(float a)
{
	return a>=0.0?1:-1;
}

#if MODE == COURBE
void calc_consignes()
{ 
	
	/////////////////NE PAS METTRE DANS L'ARDUINO
	float x=ttw.x;
	float y=ttw.y;
	float angle=ttw.angle;
	
	/////////////////FIN
	int sens=1;
        //translation
	float temptrans = dist(x,y,c_x,c_y);
	/////////////////nouvelle technique secrete de la cible fake
	float xfake=c_x-cos(c_angle)*temptrans*coeff_courbure_but;
	float yfake=c_y-sin(c_angle)*temptrans*coeff_courbure_but;
	
	/////////////////
	float tempangle;
	float angle_vers_cible= angl(x,y,xfake,yfake);
	float diff=mabs(angle_modulo(angle_vers_cible-angle));
	//printf("angle vers cible:%f, angle:%f, diff:%f\n",en_degres(angle_vers_cible),en_degres(angle),en_degres(diff));
	//printf("modulo:%f dif=%f\n",en_degres(angle_modulo(angle_vers_cible-angle)),en_degres((angle_vers_cible-angle)));
	if(diff>1.57079633&&temptrans<dist_seuil_autorisation_marche_arriere)
	{
		//printf("On roule a l'envers!\n");
                //il faut rouler à l'envers!
		angle_vers_cible+=PI;
		sens=-1;
	}
	if(temptrans>=dist_seuil_avant_tourner)
		tempangle=angle_vers_cible;
	else
		tempangle=(angle_vers_cible*temptrans+c_angle*(dist_seuil_avant_tourner-temptrans))/dist_seuil_avant_tourner;
	c_rot=angle_modulo(tempangle-angle);
	temptrans*=coefftrans1;
	temptrans*=pic(c_rot,PI);
  
	if(c_trans>=0)
	{
		if(c_trans>temptrans)
			c_trans=temptrans;
		else if (c_trans<temptrans)
			c_trans+=pas_accel;
	}
	else
	{
		if(c_trans<temptrans)
			c_trans=temptrans;
		else if(c_trans>temptrans)
			c_trans-=pas_accel;
	}
	if(c_trans>maxtrans)
		c_trans=maxtrans;
	if(c_trans<-maxtrans)
		c_trans=-maxtrans;
	c_trans*=sens;
}
#endif
#if MODE == DROIT
void calc_consignes()
{ 
	/////////////////NE PAS METTRE DANS L'ARDUINO
	float x=ttw.x;
	float y=ttw.y;
	float angle=ttw.angle;
	
	/////////////////FIN

	int sens=1;
        //translation
	float temptrans = dist(x,y,c_x,c_y);
	float tempangle;
	float angle_vers_cible= angl(x,y,c_x,c_y);
	float diff;
	if(temptrans<0.01)
	{
		phase=3;
		tempangle=c_angle;
		temptrans=0;
	}
	else 
	{
		diff=mabs(angle_modulo(angle_vers_cible-angle));
	/*	if(diff>1.57079633)//le coup de la marche arriere
		{
			angle_vers_cible+=PI;
			sens=-1;
			diff=PI-diff;
		}*/
		tempangle=angle_vers_cible;
	
		if(diff>.06)
		{          
			phase=1;
			temptrans=0;
		}
		else
		{
			phase=2;
		}
	}
	c_rot=angle_modulo(tempangle-angle);
	temptrans*=coefftrans1;
	temptrans*=sens;
	//c_trans=temptrans;
	int s1=signe(c_trans);
	int s2=signe(temptrans);
	
	if((s1*s2)<0)
	{
		if(s2==-1)
			c_trans=-0.000001;
		else
			c_trans=0.000001;
	}
	if(c_trans>=0.0)
	{
		if(c_trans>temptrans)
			c_trans=temptrans;
		else if (c_trans<temptrans)
			c_trans+=pas_accel;
	}
	else
	{
		if(c_trans<temptrans)
			c_trans=temptrans;
		else if(c_trans>temptrans)
			c_trans-=pas_accel;
	}
	
	//printf("temptrans=%f, trans=%f , cas=%d\n", temptrans, c_trans, cas);
	
	if(c_trans>maxtrans)
		c_trans=maxtrans;
	if(c_trans<-maxtrans)
		c_trans=-maxtrans;
}
#endif

void app_consignes()
{

	float c_g=(c_trans*coefftrans2 - c_rot*coeffrot);
	float c_d=(c_trans*coefftrans2 + c_rot*coeffrot);
	p_g=satur(abs(c_g),255);
	p_d=satur(abs(c_d),255);
	s_g=signe(c_g);
	s_d=signe(c_d);
	
	/*int sens_g=signe(c_g)==1?HIGH:LOW;
	int sens_d=signe(c_d)==1?HIGH:LOW;
	digitalWrite(PIN_DIR_G,sens_g);
	digitalWrite(PIN_DIR_D,sens_d);
	analogWrite(PIN_PWM_G,p_g);
	analogWrite(PIN_PWM_D,p_d);*/
}

float dist(float x1,float y1,float x2,float y2)
{
	float t1=x1-x2;
	float t2=y1-y2;
	return sqrt(t1*t1+t2*t2);
}

float angl(float x1,float y1,float x2,float y2)
{
	return atan2(y2-y1,x2-x1);
}
