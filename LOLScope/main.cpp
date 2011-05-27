#include <allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <vector>

//decommenter la ligne suivante pour desactiver le serie et acquerir un faux signal a la place
//#define FAKE

#include <SerialStream.h>
using namespace LibSerial ;
using namespace std ;
#ifndef FAKE
SerialPort Serial("/dev/ttyUSB0") ;
#endif
BITMAP* buffer;


#define PI 3.14159265359

int coulBG;
int coulSIG;
int coulTXT;
int coulBAR;
int coulDIV;


#define TRAME_SIZE 10000000
#define NCH 4

#define IDX_SIZE 32767
double idx_time[IDX_SIZE][NCH];
//int cidx[NCH];

typedef struct pointI{
	double t;
	int v;
}PointI;

typedef PointI* TrameI;

TrameI TrameNow[NCH];
int curNow[NCH];

TrameI TrameVueA[NCH];
TrameI TrameVueB[NCH];

int idxNow[NCH];
int idxA[NCH];
int idxB[NCH];

double minV=0;
double maxV=4095;

double tempsVue=-1;

int nbdiv = 10;

void saveTrameI(TrameI tr, int ch, int tidx)
{
//return;
	FILE *file;
	char buf[1000];
	sprintf(buf,"trameCh%d-%d.csv",ch,tidx);
	file = fopen(buf, "w");
	int i;
	PointI* p1;
	for(i=0;i<TRAME_SIZE;++i)
	{
		p1=&(tr[i]);
		fprintf(file, "%lf,%d\n",p1->t,p1->v);
	}
	fclose(file);
}

TrameI loadTrameI(int ch, int tidx)
{
	TrameI ret = (TrameI) malloc (TRAME_SIZE*sizeof(PointI));
	FILE *file;
	char buf[1000];
	sprintf(buf,"trameCh%d-%d.csv",ch,tidx);
	file = fopen("trameCh%d-%d.csv", "r");
	int i;
	PointI* p1;
	for(i=0;i<TRAME_SIZE;++i)
	{
		p1=&(ret[i]);
		fscanf(file,"%lf,%d\n",&(p1->t),&(p1->v));
	}
	fclose(file);
	return ret;
}


struct timespec tp;
struct timespec orig;
double gettime()
{
	clock_gettime(CLOCK_REALTIME, &tp);
	tp.tv_sec-=orig.tv_sec;
	return (double)(((double)tp.tv_sec))+(double)tp.tv_nsec/1e9;
}

double prevt;
double tnow;
#ifdef FAKE
void acquerir()
{
	int val[NCH];
	
	tnow=gettime();
	//...
	int i=0;
	double k=0;
	double N=10;
	for(k=0;k<N;k+=1)
	{
	  double leT=prevt+(tnow-prevt)*k/N;
	for(i=0;i<NCH;++i)
		val[i]=2048+2048*sin(10.0*leT+PI*i/2.0);
	
	for(i=0;i<NCH;++i)
	{
		if(curNow[i]>=TRAME_SIZE)
		{
			saveTrameI(TrameNow[i],i,idxNow[i]++);
			if(TrameVueA[i]!=NULL)
				free(TrameVueA[i]);
			TrameVueA[i]=TrameNow[i];
			TrameNow[i]=(TrameI) malloc (TRAME_SIZE*sizeof(PointI));
			curNow[i]=0;
		}
		TrameNow[i][curNow[i]].t=leT;
		TrameNow[i][curNow[i]++].v=val[i];
	}
	}
	prevt=tnow;
}
#else
char rbuf[2*NCH+2];
int rbcur=0;
void acquerir()
{
	vector<PointI> pi[4];
	int val[NCH];
	
	//...
	
	while(Serial.IsDataAvailable ())
	{
		char c=Serial.ReadByte();
		rbuf[rbcur++]=c;
		//printf("recu: %c = 0x%x\n",c,(int)c&0xFF);
		if(rbcur>=NCH*2+2)
    		rbcur=0;
  		if(c=='\xFF')
		{
//		printf("*****\n");
		
			rbcur=0;
			
			char k;
			int cur2=0;
			int i;
			for(i=0;i<NCH;++i)
			{
	  			unsigned int cL=0xFF&rbuf[cur2++];
	  			unsigned int cH=0xFF&rbuf[cur2++];
 	 
  				val[i]=(((int) cH)<<8)+(int)cL;
  			//	printf("décodé : %d   -   %x\n",val[i],val[i]);
				k*=val[i];
			}
			char rk=(char)rbuf[cur2++];
			char ff=(char)rbuf[cur2++];
			if(rk!=k || ff!='\xFF')//Donnees corrompues
			{
				printf("CORRUPTION!!\n");
				continue;
			}
			for(i=0;i<NCH;++i)
			{
				if(minV<val[i]&&val[i]<maxV)
					pi[i].push_back((PointI){0.0,val[i]});
				else if(pi[i].size()>1)
					pi[i].push_back((PointI){0.0,pi[i][pi[i].size()-1].v});
				else
					pi[i].push_back((PointI){0.0,0});
			}
		}
	}
	tnow=gettime();
	
	double steptime=(tnow-prevt)/pi[0].size();
	
	int k;
	int i;
	for(i=0;i<NCH;++i)
	{
	for(k=0;k<(signed int)pi[i].size();++k)
	{
	
		if(curNow[i]>=TRAME_SIZE)
		{
			saveTrameI(TrameNow[i],i,idxNow[i]++);
			if(TrameVueA[i]!=NULL)
				free(TrameVueA[i]);
			TrameVueA[i]=TrameNow[i];
			TrameNow[i]=(TrameI) malloc (TRAME_SIZE*sizeof(PointI));
			curNow[i]=0;
		}
		TrameNow[i][curNow[i]].t=prevt+k*steptime;
		TrameNow[i][curNow[i]++].v=pi[i][k].v;
	}
	}
	
	prevt=tnow;
}
#endif
int getidx(int channel, double time)
{
	int i;
	for(i=idxNow[channel];i>=0;--i)
		if(time>idx_time[i][channel])
			return i;
		
	return 0;
}

void drawTrame(BITMAP* bmp, int channel, double t, double nano_par_div)
{
	
	double tgauche, tdroite;
	tdroite=t+(double)(nbdiv/2.0*nano_par_div);
	if(tdroite>tnow)
		tdroite=tnow;
	tgauche=tdroite-nano_par_div*nbdiv;
	
	int idA=getidx(channel,tgauche);
	int idB=getidx(channel,tdroite);
	
	clear_to_color ( bmp,coulBG );
	int j;
	for(j=0;j<nbdiv+1;++j)
	{
		vline(bmp,j*bmp->w/nbdiv,0,bmp->h,coulDIV);
	}
	//...
	long i;
	
	double coeff=bmp->w/(tdroite - tgauche);
	double coeff2=bmp->h/(maxV-minV);
	
	
	
	//printf("%f -> %f   %d\n",tgauche*coeff, tdroite*coeff,bmp->w);
		
	double x1,x2,y1,y2;
	
	if(idA==idxNow[channel])
	{
		long max = curNow[channel];
		for(i=0;i<max;++i)
		{
			if(TrameNow[channel][i].t>=tgauche)
			{
				if(i>0)
					--i;
				if(i<0)
				  i=0;
				break;
			}
		}
		
		//if(max==TRAME_SIZE-1)
		max--;
			
		for(;i<max;++i)
		{
			
			PointI* p1=&(TrameNow[channel][i]);
			//printf("A%ld tg=%lf t=%lf td=%lf tpoint=%lf\n",i,tgauche,(double)t,tdroite,(double)(p1->t));
			if(p1->t>tdroite)
				break;
			PointI* p2=&(TrameNow[channel][i+1]);
			x1=(p1->t-tgauche)*coeff;
			y1=bmp->h-(p1->v-minV)*coeff2;
			
			x2=(p2->t-tgauche)*coeff;
			y2=bmp->h-(p2->v-minV)*coeff2;
			
			line(bmp, x1, y1, x2, y2, coulSIG);
		}
	}
	else
	{
		printf("B\n");
		if(idA!=idxA[channel])
		{
		  	if(TrameVueA[channel]!=NULL)
				free(TrameVueA[channel]);
			TrameVueA[channel]=loadTrameI(channel,idA);
			idxA[channel]=idA;

		}
		for(i=0;i<TRAME_SIZE;++i)
		{
			if(TrameVueA[channel][i].t>=tgauche)
			{
				--i;
				if(i<0)
				  i=0;
				//printf("breakat %ld\n",i);
				break;
			}
		}
		
		long max = TRAME_SIZE - 1;
		for(;i<max;++i)
		{
			PointI* p1=&(TrameVueA[channel][i]);
			if(p1->t>tdroite)
				break;
			PointI* p2=&(TrameVueA[channel][i+1]);
			x1=(p1->t-tgauche)*coeff;
			y1=bmp->h-(p1->v-minV)*coeff2;
			
			x2=(p2->t-tgauche)*coeff;
			y2=bmp->h-(p2->v-minV)*coeff2;
			
			line(bmp, x1, y1, x2, y2, coulSIG);
		}
		
	}
	//...
	if (idA != idB)
	{
		x1=x2;
		y1=y2;
		i=1;
		
		if(idB==idxNow[channel])
		{
			printf("C\n");
			x2=(TrameNow[channel][0].t-tgauche)*coeff;
			y2=(TrameNow[channel][0].v-minV)*coeff2;
			line(bmp, x1, y1, x2, y2, coulSIG);
			
			
			long max = curNow[channel]-1;
			if(max==TRAME_SIZE-1)
				max--;
			for(;i<max;++i)
			{
				PointI* p1=&(TrameNow[channel][i]);
				if(p1->t>tdroite)
					break;
				PointI* p2=&(TrameNow[channel][i+1]);
				x1=(p1->t-tgauche)*coeff;
				y1=bmp->h-(p1->v-minV)*coeff2;
				
				x2=(p2->t-tgauche)*coeff;
				y2=bmp->h-(p2->v-minV)*coeff2;
				
				line(bmp, x1, y1, x2, y2, coulSIG);
			}
		}
		else
		{
			printf("D\n");
			x2=(TrameVueB[channel][i].t-tgauche)*coeff;
			y2=(TrameVueB[channel][i].v-minV)*coeff2;
			line(bmp, x1, y1, x2, y2, coulSIG);
			
			if(idB!=idxB[channel])
			{
				if(TrameVueB[channel]!=NULL)
					free(TrameVueB[channel]);
				TrameVueB[channel]=loadTrameI(channel,idB);
				idxB[channel]=idB;
			}
			long max = TRAME_SIZE - 2;
			for(;i<max;++i)
			{
				PointI* p1=&(TrameVueB[channel][i]);
				if(p1->t>tdroite)
					break;
				PointI* p2=&(TrameVueB[channel][i+1]);
				x1=(p1->t-tgauche)*coeff;
				y1=bmp->h-(p1->v-minV)*coeff2;
				
				x2=(p2->t-tgauche)*coeff;
				y2=bmp->h-(p2->v-minV)*coeff2;
				
				line(bmp, x1, y1, x2, y2, coulSIG);
			}
			
		}
		
	}
	
}

#define TRAINEE 1.0
void drawXY(BITMAP* bmp,int chX, int chY, double t)
{
	
	long x,y;
	double coeff1=bmp->w/(maxV-minV);
	double coeff2=bmp->h/(maxV-minV);
	clear_to_color ( bmp,coulBG );
	int j;
	for(j=0;j<nbdiv+1;++j)
	{
		vline(bmp,j*bmp->w/nbdiv,0,bmp->h,coulDIV);
	}
	for(j=0;j<nbdiv+1;++j)
	{
		hline(bmp,0,j*bmp->h/nbdiv,bmp->w,coulDIV);
	}
	y=curNow[chY]-1;
	int prevx=TrameNow[chX][curNow[chX]-1].v*coeff1;
	int prevy=bmp->h-TrameNow[chY][curNow[chY]-1].v*coeff2;
	
	drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
	set_add_blender(0,0,0,255);
	
	double t1 = TrameNow[chX][curNow[chX]-1].t-TRAINEE;
	double k;
	for(x=curNow[chX]-1;x>1&&y>1;--x)
	{
		k=(TrameNow[chX][x].t-t1)/TRAINEE;
		//printf("%f\n",k);
		if(k<0.0)break;
		int px=(TrameNow[chX][x].v-minV)*coeff1;
		int py=bmp->h-(TrameNow[chY][y].v-minV)*coeff2;
		
		line(bmp, prevx, prevy, px, py, makeacol(0,255*k ,0,255));
		
		prevx=px;
		prevy=py;
		--y;
	}
	solid_mode();
}

void initg()
{
	int depth, res;
	allegro_init();
	srand ( time ( 0 ) );
	
	depth = desktop_color_depth();
	if ( depth == 0 )
		depth = 32;
	set_color_depth ( depth );
	//res = set_gfx_mode ( GFX_AUTODETECT_FULLSCREEN, 1650, 1080, 0, 0 );
	res = set_gfx_mode ( GFX_AUTODETECT_WINDOWED, 800, 400, 0, 0 );
	/*if ( res == 0 )
	 *	{
	 *	allegro_message ( "Echec initialisation de l'affichage!" );
	 *	exit ( -1 );
}*/
	
	//install_timer();
	install_keyboard();
	install_mouse();
	
	//drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
	
	/* add other initializations here */
}


void init()
{
#ifndef FAKE
	Serial.Open() ;
	Serial.SetBaudRate( SerialPort::BAUD_115200 ) ;
	//Serial.Open("/dev/ttyUSB0");
#endif
	initg();
	
	clock_gettime(CLOCK_REALTIME, &orig);
	
	coulBG = makeacol ( 0,20,0,255 );
	coulSIG = makeacol ( 0,255,0,255 );
	coulTXT = makeacol ( 0,200,0,255 );
	coulBAR = makeacol ( 200,200,200,255 );
	coulDIV = makeacol ( 0,50,50,255 );
	
	int i;
	for(i=0;i<NCH;++i)
	{
		TrameNow[i]=(TrameI) malloc (TRAME_SIZE*sizeof(PointI));
		curNow[i]=0;
		idxNow[i]=0;
	}
}

#define R1 .7

int main()
{
	
	double tps_par_div=.3;
	// INITIALISATION
	init();
	buffer=create_bitmap(screen->w,screen->h);
	
	//	BITMAP* cible=load_bitmap("cible_m.bmp",NULL);
	
	int i;
	BITMAP* trace[NCH];
	BITMAP* traceXY[NCH/2];
	
	for(i=0;i<NCH;++i)
	{
		trace[i]=create_bitmap(screen->w*R1,screen->h/NCH);
		clear_to_color ( trace[i],coulBG );
	}
	for(i=0;i<NCH/2;++i)
	{
		traceXY[i]=create_bitmap(screen->w*(1-R1),screen->h*2/NCH);
		clear_to_color ( traceXY[i],coulBG );
	}
	
	//BOUCLE
	prevt=gettime();
	double t = 1;
	while(!key[KEY_ESC])
	{
		if(key[KEY_Z])
			tps_par_div*=1.2;
		if(key[KEY_A])
			tps_par_div/=1.2;
		
		//dessin à l'ecran
		/*if(I++>=saut_frame_point_de_position)
		 *		{
		 *			putpixel(trace,ttw.x*100,ttw.y*100,makecol(0,0,255));
		 *			I=0;
		 *			//while(key[KEY_SPACE]&&!key[KEY_ESC]){}
		 *			//while(!key[KEY_SPACE]&&!key[KEY_ESC]){}
		 *			//printf("\n********************************\n\n\n");
	}*/
		clear_to_color(buffer,coulBG);
		
		t=gettime();
		acquerir();
		//determiner t;
		
		
		for(i=0;i<NCH;++i)
		{
			drawTrame ( trace[i], i,t, tps_par_div);
			blit ( trace[i],buffer,0,0,0,i*trace[0]->h,trace[i]->w,trace[i]->h);
		}
		for(i=0;i<NCH/2;++i)
		{
			drawXY ( traceXY[i], 2*i,2*i+1,t);
			blit ( traceXY[i],buffer,0,0,trace[0]->w,i*traceXY[0]->h,traceXY[i]->w,traceXY[i]->h);
		}
		
		{
			for(i=0;i<NCH/2+1;++i)
				hline(buffer,trace[0]->w,i*traceXY[0]->h,buffer->w,coulBAR);
			for(i=0;i<NCH+1;++i)
				hline(buffer,0,i*trace[0]->h,trace[0]->w,coulBAR);
			vline(buffer,0,0,buffer->h,coulBAR);
			vline(buffer,trace[0]->w,0,buffer->h,coulBAR);
			vline(buffer,buffer->w-1,0,buffer->h,coulBAR);
			
		}
		
		//		rotate_sprite(buffer, ttw.bmp, ttw.x*100-ttw.bmp->w/2, ttw.y*100-ttw.bmp->h/2, ftofix((ttw.angle+1.5708)*128.0/3.14159265359));
		//		rotate_sprite(buffer, cible, c_x*100-cible->w/2, c_y*100-cible->h/2, ftofix((c_angle+PI/2)*128.0/PI));
		blit ( buffer,screen,0,0,0,0,screen->w,screen->h);
	}
	return EXIT_SUCCESS;
}

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
