// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.

int t=1;
int minpos=28;
int maxpos=120;

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  myservo.attach(2);  // attaches the servo on pin 9 to the servo object 
} 
 
 
void loop() 
{ 
  for(pos = minpos; pos < maxpos; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(t);                       // waits 15ms for the servo to reach the position 
  } 
  delay(1000);
  for(pos = maxpos; pos>minpos; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(t);                       // waits 15ms for the servo to reach the position 
  } 
  delay(1000);
} 
