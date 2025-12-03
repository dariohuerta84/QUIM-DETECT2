#include <AccelStepper.h> //CODIGO BUENO
#include <SparkFun_AS7343.h>
#include <Wire.h>

// ==========================================
// 1. CONEXIONES (ESP32)
// ==========================================
// MOTOR Z (Actuador) -> Pines: 26, 25, 33, 32
#define Z_IN1 26
#define Z_IN2 25
#define Z_IN3 33
#define Z_IN4 32

// MOTOR R (Giro) -> Pines: 13, 12, 14, 27
// OJO: Si no prende, desconecta el pin 12 un momento.
#define R_IN1 13
#define R_IN2 12
#define R_IN3 14
#define R_IN4 27

// SENSOR I2C -> Pines: 21, 22
#define I2C_SDA 21
#define I2C_SCL 22

// ==========================================
// 2. OBJETOS Y VARIABLES
// ==========================================
AccelStepper actuator(AccelStepper::FULL4WIRE, Z_IN1, Z_IN3, Z_IN2, Z_IN4);
AccelStepper rotaryMotor(AccelStepper::FULL4WIRE, R_IN1, R_IN3, R_IN2, R_IN4);
SfeAS7343ArdI2C mySensor;

// CONFIGURACIÓN DE MOVIMIENTO
const int NIVELES_Z = 3;               // Total de alturas (Pisos)
const long PASOS_ENTRE_NIVELES = 120; // Distancia para subir de piso SE CAMBIO DE 2000 A 100 3/12
const int PUNTOS_POR_VUELTA = 12;      // Cuántas lecturas por giro completo
const int PASOS_POR_GIRO = 170;        // Pasos para girar al siguiente punto

// Variables internas
int nivelActual = 0;
int puntoActual = 0;
uint16_t data[18]; // Buffer para datos del sensor

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

  // Configuración de Motores
  actuator.setMaxSpeed(600);
  actuator.setAcceleration(200);
  rotaryMotor.setMaxSpeed(600);
  rotaryMotor.setAcceleration(200);

  // Pines como salida (para apagado total)
  pinMode(Z_IN1, OUTPUT); pinMode(Z_IN2, OUTPUT); pinMode(Z_IN3, OUTPUT); pinMode(Z_IN4, OUTPUT);
  pinMode(R_IN1, OUTPUT); pinMode(R_IN2, OUTPUT); pinMode(R_IN3, OUTPUT); pinMode(R_IN4, OUTPUT);

  Serial.println("\n\n===================================");
  Serial.println("   INICIANDO SISTEMA DE ESCANEO");
  Serial.println("===================================");

  // --- 1. TEST DE SENSOR ---
  Serial.print("-> Buscando Sensor AS7343... ");
  if (!mySensor.begin()) {
    Serial.println("[FALLO]");
    Serial.println("REVISA CABLES SDA (21) Y SCL (22)");
    while(1); // Se detiene aquí si falla
  }
  Serial.println("[OK]");
  
  // Configuración Sensor
  mySensor.setAgain((sfe_as7343_again_t)8); // Ganancia 256x
  mySensor.enableSpectralMeasurement(true);

  // --- 2. TEST DE MOTORES (Mover un poco y volver) ---
  Serial.print("-> Test Motor Giro... ");
  rotaryMotor.move(100); rotaryMotor.runToPosition();
  rotaryMotor.move(-100); rotaryMotor.runToPosition();
  Serial.println("[OK]");

  Serial.print("-> Test Actuador Z... ");
  actuator.move(100); actuator.runToPosition();
  actuator.move(-100); actuator.runToPosition();
  Serial.println("[OK]");
  
  apagarMotores(); // Enfriar
  delay(1000);
  Serial.println(">>> INICIANDO CICLO DE TRABAJO <<<");
}

void loop() {
  
  // ----------------------------------------
  // FASE 1: MOVER Y MEDIR (ROTACIÓN)
  // ----------------------------------------
  Serial.print("Nivel Z: "); Serial.print(nivelActual);
  Serial.print(" | Angulo: "); Serial.print(puntoActual);
  
  // 1. Mover Motor de Giro
  rotaryMotor.move(PASOS_POR_GIRO);
  rotaryMotor.runToPosition();
  apagarMotores(); // Apagar para que no vibre
  delay(200);      // Estabilizar
  
  // 2. Leer Sensor y Mostrar en Serial
  Serial.print(" -> Midiendo... ");
  leerSensor(); 

  // Avanzar contador de giro
  puntoActual++;

  // ----------------------------------------
  // FASE 2: CAMBIO DE NIVEL (ALTURA)
  // ----------------------------------------
  if (puntoActual >= PUNTOS_POR_VUELTA) {
    // Si terminamos una vuelta completa (12 puntos)
    puntoActual = 0;
    nivelActual++;
    
    if (nivelActual < NIVELES_Z) {
      Serial.println("\n>>> SUBIENDO AL SIGUIENTE NIVEL... <<<");
      
      // Calcular nueva altura absoluta
      long nuevaAltura = nivelActual * PASOS_ENTRE_NIVELES;
      actuator.moveTo(nuevaAltura);
      actuator.runToPosition();
      
      apagarMotores();
      delay(1000); // Esperar a que deje de temblar
      
    } else {
      // SI YA TERMINAMOS TODOS LOS NIVELES
      Serial.println("\n\n===================================");
      Serial.println("   ESCANEO COMPLETO - REGRESANDO");
      Serial.println("===================================");
      
      mySensor.ledOff();
      
      // Regresar a cero ambos motores
      actuator.moveTo(0);
      while(actuator.distanceToGo() != 0) actuator.run();
      
      rotaryMotor.moveTo(0);
      while(rotaryMotor.distanceToGo() != 0) rotaryMotor.run();
      
      apagarMotores();
      Serial.println("SISTEMA EN ESPERA.");
      while(1); // Detener ejecución aquí
    }
  }
}

// ==========================================
// FUNCIONES DE AYUDA
// ==========================================

void leerSensor() {
  // Encender LED, Leer, Apagar
  mySensor.setLedDrive(10); // 10mA
  mySensor.ledOn();
  delay(50);
  
  mySensor.readSpectraDataFromSensor();
  mySensor.getData(data);
  
  mySensor.ledOff();

  // IMPRIMIR DATOS VISIBLES
  // Solo mostramos 3 canales clave para no saturar la pantalla
  Serial.print("[F1-Azul: "); Serial.print(data[0]);
  Serial.print("] [F5-Verde: "); Serial.print(data[4]);
  Serial.print("] [F8-Rojo: "); Serial.print(data[7]);
  Serial.println("]");
}

void apagarMotores() {
  // Corta la energía para enfriar drivers y motor
  digitalWrite(Z_IN1, LOW); digitalWrite(Z_IN2, LOW); 
  digitalWrite(Z_IN3, LOW); digitalWrite(Z_IN4, LOW);
  digitalWrite(R_IN1, LOW); digitalWrite(R_IN2, LOW); 
  digitalWrite(R_IN3, LOW); digitalWrite(R_IN4, LOW);
}
