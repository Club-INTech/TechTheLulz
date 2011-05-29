//defines

#define AV_ZERO 1400*2
#define AV_MIN 1445*2
#define AV_MAX 1760*2
#define AV_ECART (AV_MAX-AV_ZERO)
#define AV_ZM (AV_MIN-AV_ZERO)
//ecart=315 ms

#define NEUTRE 1400*2

#define AR_ZERO 1360*2
#define AR_MIN 1125*2
#define AR_MAX 1315*2
#define AR_ECART (AR_ZERO-AR_MIN)
#define AR_ZM (AR_ZERO-AR_MAX)
//ecart=190 ms

#define SCALE 10000
#define N_PULSES 5

//1 = 1 positif pour 10 frames
//3150 niveaux en avant
//1900 niveaux en arriere

void setup()
{
  //set prescaler /8 -> 65536 tics @ 16MHz = 32.768 ms, 1 tic=.5us
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
  TCCR1B |= _BV(CS11);
  
  //set waveform generation mode 14 (fast PWM, clear counter when count=ICR1) (WGM1 = 1110)
  TCCR1A &= ~_BV(WGM10);   //clear
  TCCR1A |= _BV(WGM11);    //set
  TCCR1B |= _BV(WGM12);    //set
  TCCR1B |= _BV(WGM13);    //set
  
  //set compare output mode 3 (clear OC1A/OC1B when reaching BOTTOM, set on compare match with OCR1A/OCR1B
  TCCR1A |= _BV(COM1A0);   //set
  TCCR1A |= _BV(COM1A1);   //set
  TCCR1A |= _BV(COM1B0);   //set
  TCCR1A |= _BV(COM1B1);   //set
  
  //clear counter every 20ms (40 000 clock ticks with prescaler /8)
  int valICR=40000;
  ICR1H = (char)(valICR>>8);
  ICR1L = (char)(valICR&0x00FF);

  //initial duty : pulse of 1400 uS.
  int timeA = 2800;//1 tick=.5 us
  int timeB = 2800;
  setPulse(timeA,timeB);
  
  TIMSK1 |= _BV(TOIE1);
  
  //activate interrupts
  sei();
  
  //port direction pin 9 and 10 OUT -> PWM
  DDRB|=B00000110;
  
  //activate serial
  Serial.begin(57600);
}

void setPulse(int timeA, int timeB)
{
  //(inverter mode, so we can set the pulse length during the long clear time)
  unsigned int valOCR1A = (unsigned int)39999-(unsigned int)timeA;
  unsigned int valOCR1B = (unsigned int)39999-(unsigned int)timeB;
  OCR1A = valOCR1A;
  OCR1B = valOCR1B;
}

int pulseA=0;
int pulseB=0;

int tab[10]={0,0,0,0,0,0,0,0,0,0};

int idxpulse=0;
ISR(TIMER1_OVF_vect)          // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
  unsigned int forcepulseA,forcepulseB;
  
  if(pulseA>0)
  {
    int ticksA=(long)pulseA*(long)AV_ECART*N_PULSES/(long)SCALE;
    int npulses;
    if(ticksA<AV_ZM*N_PULSES)
    {
      npulses=ticksA / AV_ZM;
    }
    else
    {
      npulses=N_PULSES;
    }
    //  Serial.println(npulses);
    if(npulses==0||idxpulse>=npulses)
    {
      forcepulseA=NEUTRE;
    }
    else if(idxpulse<npulses)
    //if(idxpulse%(N_PULSES/npulses)==0)
    {
      forcepulseA=ticksA/npulses;
      forcepulseA+=(idxpulse<(ticksA%npulses)?1:0);
      forcepulseA+=AV_ZERO;
    }
  }
  else if(pulseA==0)
  {
    forcepulseA=NEUTRE;
  }
  else //if(pulseA<0)
  {
    int ticksA=(long)-pulseA*(long)AR_ECART*N_PULSES/(long)SCALE;
    int npulses;
    if(ticksA<AR_ZM*N_PULSES)
    {
      npulses=ticksA / AR_ZM;
    }
    else
    {
      npulses=N_PULSES;
    }
    //  Serial.println(npulses);
    if(npulses==0||idxpulse>=npulses)
    {
      forcepulseA=NEUTRE;
    }
    else if(idxpulse<npulses)
    //if(idxpulse%(N_PULSES/npulses)==0)
    {
      forcepulseA=ticksA/npulses;
      forcepulseA+=(idxpulse<(ticksA%npulses)?1:0);
      forcepulseA=AR_ZERO-forcepulseA;
    }
  }  
  
  if(pulseB>0)
  {
    int ticksB=(long)pulseB*(long)AV_ECART*N_PULSES/(long)SCALE;
    int npulses;
    if(ticksB<AV_ZM*N_PULSES)
    {
      npulses=ticksB / AV_ZM;
    }
    else
    {
      npulses=N_PULSES;
    }
    //  Serial.println(npulses);
    if(npulses==0||idxpulse>=npulses)
    {
      forcepulseB=NEUTRE;
    }
    else if(idxpulse<npulses)
   // if(idxpulse%(N_PULSES/npulses)==0)
    {
      forcepulseB=ticksB/npulses;
      forcepulseB+=(idxpulse<(ticksB%npulses)?1:0);
      //forcepulseB+=(idxpulse%(N_PULSES/npulses)<(ticksB%npulses)?1:0);
      forcepulseB+=AV_ZERO;
    }
  }
  else if(pulseB==0)
  {
    forcepulseB=NEUTRE;
  }
  else //if(pulseA<0)
  {
    int ticksB=(long)-pulseB*(long)AR_ECART*N_PULSES/(long)SCALE;
    int npulses;
    if(ticksB<AR_ZM*N_PULSES)
    {
      npulses=ticksB / AR_ZM;
    }
    else
    {
      npulses=N_PULSES;
    }
    //  Serial.println(npulses);
    if(npulses==0||idxpulse>=npulses)
    {
      forcepulseB=NEUTRE;
    }
    else if(idxpulse<npulses)
    //if(idxpulse%(N_PULSES/npulses)==0)
    {
      forcepulseB=ticksB/npulses;
      forcepulseB+=(idxpulse<(ticksB%npulses)?1:0);
      forcepulseB=AR_ZERO-forcepulseB;
    }
  }    
  setPulse(forcepulseA,forcepulseB);
  
  tab[idxpulse]=forcepulseA;
  
  idxpulse++;
  if(idxpulse>=N_PULSES)
  {
    idxpulse=0;
  }
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
char buf[20];
void recv()
{
  int i;
  for(i = 0 ; i < 18 ; ++i)
  {
    while(!Serial.available())
    {}
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
  Serial.print("recu ");
  Serial.println(buf);
  
}

void traiter()
{
    if(buf[0]=='G')
    {
      long int t=lire_long(buf+1);
      recv();
      if(buf[0]=='C')
      {  
        int ck=lire_long(buf+1);
        if((t*t)%91==ck)
        {
          pulseA=t;
          //Serial.println("CK OK");
        }
        else
        {
           //Serial.print("CK BAD ");
           //Serial.print((t*t)%91); 
        }
      }
      else if(buf[0]=='G'||buf[0]=='D')
        traiter;
    }
    if(buf[0]=='D')
    {
      long int t=lire_long(buf+1);
      recv();
      if(buf[0]=='C')
      {  
        int ck=lire_long(buf+1);
        if((t*t)%91==ck)
        {
          pulseB=t;
          Serial.println("CK OK");
        }
        else
        {
           Serial.print("CK BAD ");
           //Serial.print((t*t)%91); 
        }
      }
      else if(buf[0]=='G'||buf[0]=='D')
        traiter;
    }
    if(buf[0]=='?')
    {
      Serial.println("?e#");
    }
}

void recv_consignes()
{
  while(Serial.available())
  {
    recv();
    traiter();
  }
}

long int k=0;
void loop()
{
  recv_consignes();
  
 /* if(k++>=25000)
    k=0;
  int i;
  if(k==0)
  {
    Serial.print("PULSE");
  for(i=0;i<N_PULSES;++i)
  {
    Serial.print(tab[i]);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.println("---------------------");
  Serial.print("OCR1A ");
  Serial.println(OCR1A,BIN);
  Serial.print("OCR1B ");
  Serial.println(OCR1B,BIN);
  Serial.print("TCCR1A ");
  Serial.println(TCCR1A,BIN);
  Serial.print("TCCR1B ");
  Serial.println(TCCR1B,BIN);
  Serial.print("DDRB ");
  Serial.println(DDRB,BIN);
  
  Serial.print("ConsG ");
  Serial.println(pulseA);
  
  
  }
*/}
