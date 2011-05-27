// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.

int t=10;
int minpos=28;
int maxpos=170;

#include <Servo.h> 
 
Servo myservo1;  // create servo object to control a servo 
Servo myservo2;                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  pinMode(13,OUTPUT);
  myservo1.attach(9);  // attaches the servo on pin 9 to the servo object 
  myservo2.attach(10);  // attaches the servo on pin 9 to the servo object 
  Serial.begin(4800);
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

void transition(int valarrivee1, int valarrivee2, int milli)
{
  int i;
  int valdepart1 = myservo1.readMicroseconds();
  int valdepart2 = myservo2.readMicroseconds();
  int val1,val2;
  for(i=0;i<milli;++i)
  {
    val1=valdepart1+(((long)valarrivee1-(long)valdepart1)*(long)i/(long)milli);
    val2=valdepart2+(((long)valarrivee2-(long)valdepart2)*(long)i/(long)milli);
    myservo1.writeMicroseconds(val1);              // tell servo to go to position in variable 'pos' 
    myservo2.writeMicroseconds(val2);              // tell servo to go to position in variable 'pos' 
    //Serial.println(val);
    delay(1);
  }
  myservo1.writeMicroseconds(valarrivee1);   
  myservo2.writeMicroseconds(valarrivee2);   
}

long c_g=1400;
long c_d=1400;

char buf[20];
void recv()
{
  int i;
  for(i = 0 ; i < 18 ; ++i)
  {
    char c = Serial.read();
    buf[i] = c;
    if(c=='#')
    {
      buf[i+1] = '\0';
      break;
    }
  }
  buf[18]='#';
  buf[19]='\0';
}

void recv_consignes()
{
  while(Serial.available())
  {
    recv();
    if(buf[0]=='G')
    {
      c_g=lire_long(buf+1);
    }
    if(buf[0]=='D')
    {
      c_d=lire_long(buf+1);
    }
  }
}

void appli_consignes()
{/*
  if(c_g<1400 && myservo1.read()>=1400 && c_d<1400 && myservo2.read()>=1400)
  {
    transition(1000,1000,100);
    transition(1400,1400,100); 
  }
  else if(c_g<1400 && myservo1.read()>=1400)
  {
    transition(1000,c_d,100);
    transition(1400,c_d,100);   
  }
  else if(c_d<1400 && myservo2.read()>=1400)
  {
    transition(c_g,1000,100);
    transition(c_g,1400,100);   
  }
  else*/
    {
      //transition(c_g,c_d,100);
      myservo1.writeMicroseconds(c_g);   
      myservo2.writeMicroseconds(c_d);   
    }
  }

void loop() 
{ 
  Serial.println("INIT");
  myservo1.writeMicroseconds(1400);              // tell servo to go to position in variable 'pos' 
  myservo2.writeMicroseconds(1400);              // tell servo to go to position in variable 'pos' 
  delay(2000);
  //transition(1800,1000);
  //transition(1400,1000);
      
  Serial.println("GO");
  
    
  while(1)
  {
    recv_consignes();
    Serial.print("c_g ");
    Serial.print(c_g);
    Serial.print(" c_d ");
    Serial.println(c_d);
    appli_consignes();
  }
} 
