#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// ----- LCD -----
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ----- RGB LED -----
const int RED_PIN   = 6;
const int GREEN_PIN = 3;
const int BLUE_PIN  = 5;

// ----- BUZZER -----
const int BUZZER = 2;

// ----- SERVO -----
Servo servoMotor;
const int SERVO_PIN = 4;

// Ajusta estos ángulos según tu caja
const int ANGULO_CERRADO = 0;   // cerrado (prueba 0, 10, 20)
const int ANGULO_ABIERTO = 90;   // abierto (prueba 90, 120, 180)

// ----- CONTRASEÑA -----
String password = "1234";
String entrada  = "";

// ----- KEYPAD -----
const byte filas = 4;
const byte columnas = 3;

byte pinesFilas[] = {13, 12, 11, 10};
byte pinesColumnas[] = {9, 8, 7};

char teclas[4][3] = {
  { '1','2','3'},
  { '4','5','6'},
  { '7','8','9'},
  { '*','0','#'}
};

Keypad teclado = Keypad(makeKeymap(teclas), pinesFilas, pinesColumnas, filas, columnas);

// ----- ESTADOS -----
enum Estado { BLOQUEADA, DESBLOQUEADA };
Estado estado = BLOQUEADA;

// ---------------- COLORES ----------------
void ledApagado() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
}

// NARANJA = Rojo + Verde (ACCESO PERMITIDO)
void ledNaranja() {
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, LOW);
}

// MORADO = Rojo + Azul (ACCESO DENEGADO)
void ledMorado() {
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);
}

// ---------------- SONIDOS ----------------
void sonidoDesbloqueo() {
  // “Tirín” corto: sube tonos
  tone(BUZZER, 700); delay(120);
  tone(BUZZER, 900); delay(120);
  tone(BUZZER, 1100); delay(150);
  noTone(BUZZER);
}

void sonidoBloqueo() {
  // “Tún” corto: baja tonos
  tone(BUZZER, 1000); delay(120);
  tone(BUZZER, 800);  delay(120);
  tone(BUZZER, 600);  delay(150);
  noTone(BUZZER);
}

void alarma() {
  for (int i = 0; i < 3; i++) {
    tone(BUZZER, 900);
    delay(200);
    noTone(BUZZER);
    delay(200);
  }
}

// ---------------- PANTALLAS ----------------
void mensajeInicio() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   CAJA FUERTE");
  delay(1500);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ingrese la clave");
  lcd.setCursor(0,1);
  lcd.print("y presione #");
  delay(2500);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("* = Borrar");
  lcd.setCursor(0,1);
  lcd.print("# = Aceptar");
  delay(2500);

  lcd.clear();
}

void pedirClave() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ingrese clave:");
  lcd.setCursor(0,1);
  lcd.print("Clave:");
}

void pantallaDesbloqueada() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CAJA: ABIERTA");
  lcd.setCursor(0,1);
  lcd.print("* Cerrar  #OK");
  // * = cerrar/bloquear
  // # = dejar abierta (no hace nada, solo confirma)
}

// ---------------- ACCIONES CAJA ----------------
void abrirCaja() {
  ledNaranja();
  sonidoDesbloqueo();
  servoMotor.write(ANGULO_ABIERTO);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ACCESO PERMITIDO.");
  lcd.setCursor(0,1);
  lcd.print("Abriendo...");
  delay(1200);

  estado = DESBLOQUEADA;
  pantallaDesbloqueada();
}

void cerrarCaja() {
  servoMotor.write(ANGULO_CERRADO);
  sonidoBloqueo();
  ledApagado();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Cerrando...");
  delay(900);

  entrada = "";
  estado = BLOQUEADA;
  pedirClave();
}

// ---------------- SETUP ----------------
void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  servoMotor.attach(SERVO_PIN);
  servoMotor.write(ANGULO_CERRADO);  // inicia cerrada

  ledApagado();
  mensajeInicio();
  pedirClave();
}

// ---------------- LOOP ----------------
void loop() {
  char tecla = teclado.getKey();
  if (!tecla) return;

  // ====== ESTADO BLOQUEADA ======
  if (estado == BLOQUEADA) {

    // Mostrar lo escrito (puedes cambiar a **** si quieres)
    lcd.setCursor(7,1);
    lcd.print(entrada + "    "); // limpia residuos

    if (tecla == '#') {
      // Validar
      lcd.clear();

      if (entrada == password) {
        abrirCaja();
      } else {
        ledMorado();
        lcd.setCursor(0,0);
        lcd.print("ACCESO DENEG.");
        lcd.setCursor(0,1);
        lcd.print("Intente otra");
        alarma();
        delay(1800);

        entrada = "";
        ledApagado();
        pedirClave();
      }
      return;
    }

    if (tecla == '*') {
      // Borrar
      entrada = "";
      pedirClave();
      return;
    }

    // Agregar dígitos
    if (isDigit(tecla)) {
      if (entrada.length() < 4) entrada += tecla;
    }
  }

  // ====== ESTADO DESBLOQUEADA ======
  else if (estado == DESBLOQUEADA) {

    if (tecla == '*') {
      // Cerrar y bloquear
      cerrarCaja();
      return;
    }

    if (tecla == '#') {
      // Confirmación (opcional)
      lcd.setCursor(0,1);
      lcd.print("* Cerrar  #OK");
      tone(BUZZER, 1200); delay(70); noTone(BUZZER);
      return;
    }

    // Si presionan números, ignoramos (o podrías hacer otra opción)
  }
}
