#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin setup
const int irSensorPin = 2; // IR sensor connected to digital pin 2

// RPM calculation variables
volatile int count = 0; // Counts the number of white strip detections
unsigned long lastMillis = 0;
int rpm = 0; // RPM value to display as an integer
int divisor = 1; // To handle multiple strips (1 for single strip, 2 for diameter)

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD at I2C address 0x27, 16 columns, 2 rows

void setup() {
  pinMode(irSensorPin, INPUT);  // Set IR sensor pin as input
  attachInterrupt(digitalPinToInterrupt(irSensorPin), countWhiteStripe, RISING); // Interrupt on rising edge

  lcd.begin();              // Start LCD
  lcd.setBacklight(1);      // Turn on backlight
  lcd.clear();              // Clear the screen
  lcd.print("RPM Counter");
  delay(2000);
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Calculate RPM every 1 second (1000 milliseconds)
  if (currentMillis - lastMillis >= 1000) {
    lastMillis = currentMillis;
    
    // Calculate RPM as an integer: (count * 60) / time in seconds
    rpm = (count * 60) / divisor; // Adjust RPM based on divisor (1 or 2)
    
    // Display RPM on the LCD
    lcd.clear();
    lcd.print("RPM: ");
    lcd.print(rpm); // Display RPM as an integer
    count = 0; // Reset count for next calculation
  }
}

void countWhiteStripe() {
  // Increment count every time the white tape passes the IR sensor
  count++;
}
