#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// a maximum of eight servo objects can be created

int velocity = 1500;
const int counter = 67;
int acceleration_clockwise[counter] = {1,2,3,4,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,5,4,3,2,1,0,0,0,0,0,0,
  -1,-2,-3,-4,-5,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-6,-5,-4,-3,-2,-1,0};
void setup()
{
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}


void loop()
{
  for(int i = 0; i < counter; i++){                   //clockwise
    velocity = velocity + acceleration_clockwise[i];
    myservo.writeMicroseconds(velocity);
    Serial.println(velocity);
    delay(500);
  } 
  delay(500);                                         
  for(int i = 0; i < counter; i++){                  //Counterclockwise
    velocity = velocity - acceleration_clockwise[i];
    myservo.writeMicroseconds(velocity);
    Serial.println(velocity);
    delay(500);
  }
  delay(500);
}




