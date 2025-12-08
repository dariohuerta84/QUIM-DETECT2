#include <AccelStepper.h>
#include <SparkFun_AS7343.h>
#include <Wire.h>

// ==========================================
// 1. CONFIGURACIÓN HARDWARE
// ==========================================
// MOTOR Z (Actuador)
#define Z_IN1 26
#define Z_IN2 25
#define Z_IN3 33
#define Z_IN4 32

// MOTOR R (Giro)
#define R_IN1 13
#define R_IN2 12
#define R_IN3 14
#define R_IN4 27

// SENSOR
#define I2C_SDA 21
#define I2C_SCL 22

// ==========================================
// 2. OBJETOS
// ==========================================
AccelStepper actuator(AccelStepper::FULL4WIRE, Z_IN1, Z_IN3, Z_IN2, Z_IN4);
AccelStepper rotaryMotor(AccelStepper::FULL4WIRE, R_IN1, R_IN3, R_IN2, R_IN4);
SfeAS7343ArdI2C mySensor;

// Buffer para los 18 canales (definido por la librería)
uint16_t spectralData[18]; 

// PARÁMETROS DE ESCANEO
const int NIVELES_Z = 3;            
const long PASOS_ENTRE_NIVELES = 120; 
const int PUNTOS_POR_VUELTA = 12;      
const int PASOS_POR_GIRO = 170;        

int nivelActual = 0;
int puntoActual = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

  // --- CONFIG MOTORES ---
  actuator.setMaxSpeed(600);
  actuator.setAcceleration(200);
  rotaryMotor.setMaxSpeed(600);
  rotaryMotor.setAcceleration(200);
  
  pinMode(Z_IN1, OUTPUT); pinMode(Z_IN2, OUTPUT); pinMode(Z_IN3, OUTPUT); pinMode(Z_IN4, OUTPUT);
  pinMode(R_IN1, OUTPUT); pinMode(R_IN2, OUTPUT); pinMode(R_IN3, OUTPUT); pinMode(R_IN4, OUTPUT);

  // --- INICIO SENSOR ---
  Serial.println("INICIANDO TANOSPECTRA...");
  
  if (!mySensor.begin()) {
    Serial.println("Error: Sensor AS7343 no detectado.");
    while(1);
  }

  if (!mySensor.powerOn()) {
    Serial.println("Error: No se pudo encender el sensor.");
    while(1);
  }

  
  // Configuramos el AutoSmux para que lea TODO (F1-F12 + NIR + Clear)
  mySensor.setAutoSmux(AUTOSMUX_18_CHANNELS);
  mySensor.enableSpectralMeasurement(true);
  
  // Ajuste de Ganancia y LED (Ajustar según pruebas)
  mySensor.setAgain((sfe_as7343_again_t)8); 
  Serial.println("Ganancia ajustada a 256x");
  Serial.println("Sensor Configurado en modo 18 CANALES.");
  
  // IMPRIMIR CABECERA CSV
  // Esto es vital para tu Excel / Python
  Serial.println("Nivel,Angulo,CH0,CH1,CH2,CH3,CH4,CH5,CH6,CH7,CH8,CH9,CH10,CH11,CH12,CH13,CH14,CH15,CH16,CH17");

  delay(1000);
}

void loop() {
  
  // 1. MOVER MOTOR GIRO
  rotaryMotor.move(PASOS_POR_GIRO);
  rotaryMotor.runToPosition();
  apagarMotores(); 
  delay(500); // Esperar que la fresa deje de moverse

  // 2. LEER DATOS ESPECTRALES (18 CANALES)
  leerYReportar();

  puntoActual++;

  // 3. CAMBIO DE NIVEL
  if (puntoActual >= PUNTOS_POR_VUELTA) {
    puntoActual = 0;
    nivelActual++;
    
    if (nivelActual < NIVELES_Z) {
      long nuevaAltura = nivelActual * PASOS_ENTRE_NIVELES;
      actuator.moveTo(nuevaAltura);
      actuator.runToPosition();
      apagarMotores();
      delay(1000);
    } else {
      // FIN DEL ESCANEO
      Serial.println("FIN_DE_ESCANEO");
      mySensor.ledOff();
      
      // 1. EL MOTOR Z (ELEVADOR) SÍ DEBE BAJAR
      Serial.println("Bajando actuador...");
      actuator.moveTo(0);
      while(actuator.distanceToGo() != 0) actuator.run();
      
      // 2. EL MOTOR DE GIRO: SOLO RESETEAMOS EL CEREBRO
      // En lugar de moverlo físicamente hacia atrás (moveTo),
      // le decimos: "Donde estás ahora es el nuevo Cero".
      rotaryMotor.setCurrentPosition(0); 
      
      apagarMotores();
      
      Serial.println("SISTEMA EN ESPERA.");
      while(1); 
    }
  }
}

void leerYReportar() {
  mySensor.setLedDrive(10);
  mySensor.ledOn();
  delay(100); // Dar tiempo al LED y al sensor para hacer los 3 ciclos de lectura
  
  // Esta función lee los 18 canales secuencialmente
  mySensor.readSpectraDataFromSensor();
  
  mySensor.ledOff();
  
  // Volcamos los datos al array
  int canalesLeidos = mySensor.getData(spectralData);

  // IMPRIMIR CSV
  Serial.print(nivelActual); Serial.print(",");
  Serial.print(puntoActual); Serial.print(",");
  
  for (int i = 0; i < canalesLeidos; i++) {
    Serial.print(spectralData[i]);
    if (i < canalesLeidos - 1) Serial.print(",");
  }
  Serial.println(); // Salto de línea
}

void apagarMotores() {
  digitalWrite(Z_IN1, LOW); digitalWrite(Z_IN2, LOW); 
  digitalWrite(Z_IN3, LOW); digitalWrite(Z_IN4, LOW);
  digitalWrite(R_IN1, LOW); digitalWrite(R_IN2, LOW); 
  digitalWrite(R_IN3, LOW); digitalWrite(R_IN4, LOW);
}
