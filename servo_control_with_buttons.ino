/*
This code demonstrates how to control three servo motors using pushbuttons.
For instructions to build your own robotic arm, see:

*/

// include the servo library, required for controlling servos
#include <Servo.h>

// declare variables for the different pins
// servo 1 is controlled by buttons 1A and 1B, etc
const int servo1pin = 10;
const int servo2pin = 7;
const int servo3pin = 4;
const int button1Apin = 9;
const int button1Bpin = 8;
const int button2Apin = 6;
const int button2Bpin = 5;
const int button3Apin = 3;
const int button3Bpin = 2;

// declare variables for button states
int button1Astate;
int button1Bstate;
int button2Astate;
int button2Bstate;
int button3Astate;
int button3Bstate;

// declare variables for servo angles
// start each servo in the middle at 90 degrees
int servo1angle = 90;
int servo2angle = 90;
int servo3angle = 90;

// delay variable to make sure the servos don't
// move too fast. The Tinkercad simulation can
// move pretty slowly, you may want to make this
// delay longer on a physical Arduino.
int delayTime = 1;

// create servo objects
Servo servo1;
Servo servo2;
Servo servo3;

void setup() // code that only runs once
{
  // set button pins as inputs with pullup resistors enabled
  pinMode(button1Apin,INPUT_PULLUP);
  pinMode(button1Bpin,INPUT_PULLUP);
  pinMode(button2Apin,INPUT_PULLUP);
  pinMode(button2Bpin,INPUT_PULLUP);
  pinMode(button3Apin,INPUT_PULLUP);
  pinMode(button3Bpin,INPUT_PULLUP);
  
  // attach servo objects to pins
  servo1.attach(servo1pin);
  servo2.attach(servo2pin);
  servo3.attach(servo3pin);
  
  // initialize serial communication for debugging
  // Note: serial communication is slow. If you comment
  // out all the serial print commands, your motors
  // may move faster on a physical Arduino.
  Serial.begin(9600);
}

void loop()
{
  // read button states
  button1Astate = digitalRead(button1Apin);
  button1Bstate = digitalRead(button1Bpin);
  button2Astate = digitalRead(button2Apin);
  button2Bstate = digitalRead(button2Bpin);
  button3Astate = digitalRead(button3Apin);
  button3Bstate = digitalRead(button3Bpin);
  
  // check if buttons are pushed, if so change servo angles accordingly
  // note that since internal pullup resistors are enabled,
  // pins will go LOW when the button is pushed
  if(button1Astate == LOW){ // increase angle for servo 1
    servo1angle = servo1angle + 1;
  }
  if(button1Bstate == LOW){ // decrease angle for servo 1
    servo1angle = servo1angle - 1;
  }
  if(button2Astate == LOW){ // increase angle for servo 2
    servo2angle = servo2angle + 1;
  }
  if(button2Bstate == LOW){ // decrease angle for servo 2
    servo2angle = servo2angle - 1;
  }
  if(button3Astate == LOW){ // increase angle for servo 3
    servo3angle = servo3angle + 1;
  }
  if(button3Bstate == LOW){ // decrease angle for servo 3
    servo3angle = servo3angle - 1;
  }
  
  // make sure servo angles do not go outside 0-180 range
  // if you hold the button down too long
  servo1angle = min(servo1angle,180);
  servo1angle = max(servo1angle,0);
  servo2angle = min(servo2angle,180);
  servo2angle = max(servo2angle,0);
  servo3angle = min(servo3angle,180);
  servo3angle = max(servo3angle,0);
  
  // write the new angle values to the servos
  servo1.write(servo1angle);
  servo2.write(servo2angle);
  servo3.write(servo3angle);
  
  Serial.print(servo1angle);
  Serial.print(" ");
  Serial.print(servo2angle);
  Serial.print(" ");
  Serial.println(servo3angle);
  
  // delay before next loop
  delay(delayTime);
  
}