#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Dirección común: 0x27 (a veces es 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

int soilPin = A0;
int ldrPin  = A1;

int greenLED = 8;
int redLED   = 9;
int motorPin = 7;

int soilValue = 0;
int ldrValue  = 0;

void setup() {
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(motorPin, OUTPUT);

  Serial.begin(9600);

  lcd.init();          // Inicializa LCD
  lcd.backlight();     // Enciende luz de fondo

  lcd.setCursor(0, 0);
  lcd.print("Sistema Riego");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...");
  delay(2000);
  lcd.clear();
}

void loop() {
  ldrValue  = analogRead(ldrPin);
  soilValue = analogRead(soilPin);

  Serial.print("Light Level: ");
  Serial.println(ldrValue);

  Serial.print("Soil Moisture: ");
  Serial.println(soilValue);

  String estado = "";

  // 🌞 HIGH LIGHT CONDITION
  if (ldrValue > 600) {

    // 💧 Soil Wet
    if (soilValue > 600) {
      digitalWrite(motorPin, LOW);   // Motor OFF
      digitalWrite(greenLED, HIGH);  // Wet indicator
      digitalWrite(redLED, LOW);
      estado = "Suelo humedo";
    }

    // 🌱 Soil Dry
    else {
      digitalWrite(motorPin, HIGH);  // Motor ON
      digitalWrite(greenLED, LOW);
      digitalWrite(redLED, HIGH);    // Dry indicator
      estado = "Riego ON";
    }
  }

  // 🌑 LOW LIGHT → Everything OFF
  else {
    digitalWrite(motorPin, LOW);
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, LOW);
    estado = "Poca luz/OFF";
  }

  // ===== Mostrar en LCD =====
  lcd.clear();

  // Fila 1: L y S
  lcd.setCursor(0, 0);
  lcd.print("L:");
  lcd.print(ldrValue);
  lcd.print(" S:");
  lcd.print(soilValue);

  // Fila 2: Estado
  lcd.setCursor(0, 1);
  lcd.print(estado);

  delay(1000);
}