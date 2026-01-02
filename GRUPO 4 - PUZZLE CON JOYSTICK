#include <Servo.h>

Servo servo1;
Servo servo2;
int joyX = 0; // Analog pin for X-axis
int joyY = 1; // Analog pin for Y-axis

int servoVal;
unsigned long previousMillis = 0; // Timer variable for Serial output
const long interval = 500;        // Interval for Serial output in milliseconds

void setup() 
{
  Serial.begin(9600); // Initialize serial communication at 9600 bps
  servo1.attach(3);    // Connect servo1 to digital pin 3
  servo2.attach(5);    // Connect servo2 to digital pin 5
}

void loop()
{
  // Read and map X-axis joystick value for servo1
  servoVal = analogRead(joyX);
  int servo1Pos = map(servoVal, 0, 1023, 180, 130);
  servo1.write(servo1Pos);

  // Read and map Y-axis joystick value for servo2
  servoVal = analogRead(joyY);
  int servo2Pos = map(servoVal, 0, 1023, 130, 180);
  servo2.write(servo2Pos);

  // Check if the interval has elapsed for Serial output
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis; // Update the timer

    // Print joystick and servo positions to Serial Monitor
    Serial.print("Joystick X: ");
    Serial.print(analogRead(joyX));
    Serial.print(" -> Servo1 Position: ");
    Serial.println(servo1Pos);

    Serial.print("Joystick Y: ");
    Serial.print(analogRead(joyY));
    Serial.print(" -> Servo2 Position: ");
    Serial.println(servo2Pos);
  }

  delay(50); // Small delay for stable servo movement
}
