#ifndef H_MY_TOOLS
#define H_MY_TOOLS
#define PI 3.14159265359
void my_sleep ( volatile unsigned long Seconds );
void my_msleep ( volatile unsigned long MilliSeconds );
void my_usleep ( volatile unsigned long MicroSeconds );
void my_delay ( volatile unsigned long delay );
int signe(double a);
int my_abs ( int x );
long int ctol(char c);
double angle_modulo(double a);//mettre entre -PI et PI
double pic(double a, double seuil);
double satur(double a, double seuil);
#endif
