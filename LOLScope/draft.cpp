 
 //envoi:
short int val[NCH];

int i;

char k;
for(i=0;i<NCH;++i)
{
  if((char)((val[i]>>8)&0xFF)==0xFF)//valeur interdite, caractere de separation
    val[i]++;
  send((char)(val[i]&0xFF));
  send((char)((val[i]>>8)&0xFF));
  k*=val[i];
}
send(k);
send(0xFF);


//reception

char buf[NCH*2+2]
global int cur=0;

while(available())
{
  char c = recv();
  buf[cur++] = c;
  if(cur>=NCH*2+2)
    cur=0;
  if(c==0xFF)
  {
    parse();
    cur=0;
  }
}

//parse

short int val[NCH];

int i;

char k;
int cur2=0;
for(i=0;i<NCH;++i)
{
  char cL=buf[cur2++];
  char cH=buf[cur2++];
  
  val[i]=((int cH)<<8)+cL;
  k*=val[i];
}
char rk=buf[cur2++];
char ff=buf[cur2++];
if(rk==k && ff=0xFF)
{
  //tenir compte, sinon rien.
}

