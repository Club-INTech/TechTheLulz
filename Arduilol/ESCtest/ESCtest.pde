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
  myservo1.attach(4);  // attaches the servo on pin 9 to the servo object 
  myservo2.attach(5);  // attaches the servo on pin 9 to the servo object 
  Serial.begin(9600);
} 
 
void transition(int valarrivee, int milli)
{
  int i;
  int valdepart1 = myservo1.readMicroseconds();
  int valdepart2 = myservo2.readMicroseconds();
  int val1,val2;
  for(i=0;i<milli;++i)
  {
    val1=valdepart1+(((long)valarrivee-(long)valdepart1)*(long)i/(long)milli);
    val2=valdepart2+(((long)valarrivee-(long)valdepart2)*(long)i/(long)milli);
    myservo1.writeMicroseconds(val1);              // tell servo to go to position in variable 'pos' 
    myservo2.writeMicroseconds(val2);              // tell servo to go to position in variable 'pos' 
    //Serial.println(val);
    delay(1);
  }
  myservo1.writeMicroseconds(valarrivee);   
  myservo2.writeMicroseconds(valarrivee);   
}

void loop() 
{ 
  Serial.println("INIT");
  myservo1.writeMicroseconds(1400);              // tell servo to go to position in variable 'pos' 
  myservo2.writeMicroseconds(1400);              // tell servo to go to position in variable 'pos' 
  delay(3000);
  transition(1800,1000);
  transition(1400,1000);
      
  Serial.println("GO");
  
    
  while(1)
  {
    digitalWrite(13,HIGH);
    Serial.println("arriere 1");
    transition(1100,50);
    Serial.println("stop");
    transition(1400,50);
    Serial.println("arriere 2");
    transition(1100,1000);
    delay(3000);
    digitalWrite(13,LOW);
    Serial.println("stop, attente");
    transition(1400,1000);
    //delay(1000);
    /*Serial.println("avant 1");
    transition(1800,500);
    Serial.println("stop");
    transition(1400,500);
    */Serial.println("avant 2");
    transition(1800,1000);
    delay(3000);
    Serial.println("stop");
    transition(1400,1000);
    /*digitalWrite(13,HIGH);
    myservo.writeMicroseconds(1400);              // tell servo to go to position in variable 'pos' 
    delay(1000);                       // waits 15ms for the servo to reach the position 
    myservo.writeMicroseconds(1500);              // tell servo to go to position in variable 'pos' 
    delay(1000);                       // waits 15ms for the servo to reach the position 
    myservo.writeMicroseconds(1600);              // tell servo to go to position in variable 'pos' 
    delay(1000);                       // waits 15ms for the servo to reach the position 
    myservo.writeMicroseconds(1700);              // tell servo to go to position in variable 'pos' 
    delay(1000);                       // waits 15ms for the servo to reach the position 
    myservo.writeMicroseconds(1800);              // tell servo to go to position in variable 'pos' 
    delay(1000);                       // waits 15ms for the servo to reach the position 
   digitalWrite(13,LOW);
    delay(1000);                       // waits 15ms for the servo to reach the position 
    */
    /*myservo.writeMicroseconds(1200);              // tell servo to go to position in variable 'pos' 
    delay(1000);                       // waits 15ms for the servo to reach the position 
    myservo.writeMicroseconds(1100);              // tell servo to go to position in variable 'pos' 
    delay(1000);                       // waits 15ms for the servo to reach the position 
    myservo.writeMicroseconds(1000);              // tell servo to go to position in variable 'pos' 
    delay(5000);                       // waits 15ms for the servo to reach the position 
  */}
} 
