/*
  ASCII table
 
 Prints out byte values in all possible formats:  
 * as raw binary values
 * as ASCII-encoded decimal, hex, octal, and binary values
 
 For more on ASCII, see http://www.asciitable.com and http://en.wikipedia.org/wiki/ASCII
 
 The circuit:  No external hardware needed.
 
 created 2006
 by Nicholas Zambetti 
 modified 18 Jan 2009
 by Tom Igoe
 
 This example code is in the public domain.

 <http://www.zambetti.com> 
 
 */
void setup() 
{ 
  Serial.begin(9600); 

  // prints title with ending line break 
  Serial.println("ASCII Table ~ Character Map"); 
} 

const int moy=1024*1/5;

float anew=0;
float aprev=0;

int first = 10;

const float PI2=2*PI;

float ang=1;

int i=0;
void loop() 
{ 
  int k1=analogRead(0)-moy;
  int k2=analogRead(1)-moy;
  
  anew = atan2(k1,k2);
  float diff = anew - aprev;
  // prints value unaltered, i.e. the raw binary version of the 
  // byte. The serial monitor interprets all bytes as 
  // ASCII, so 33, the first number,  will show up as '!' 
  
  if(diff<PI)
    diff+=PI2;
  if(diff>PI)
    diff-=PI2;
  
  aprev=anew;
  ang+=diff;
  if(first)
  {
    ang=0;
    first--;
  }
  if(i++%1000==0)
    Serial.println(ang/PI2/34);  
 /*
   Serial.print("a1 ");
   Serial.print(k1);
   Serial.print(" a2 ");
   Serial.println(k2);
   */
} 
