#include <Stepper.h>
#include <SparkFun_AS7343.h>
#include <Wire.h>

// ==========================================
// CONFIGURACIÓN DEL MOTOR
// ==========================================
const int PASOS_POR_REVOLUCION = 2048; 

// Nota sobre la librería Stepper: El orden de los pines es IN1, IN3, IN2, IN4
// Según tu conexión: IN1=D5, IN3=D3, IN2=D4, IN4=D2
Stepper myMotor(PASOS_POR_REVOLUCION, 5, 3, 4, 2);

// ==========================================
// CONFIGURACIÓN DEL SENSOR
// ==========================================
SfeAS7343ArdI2C mySensor;
uint16_t myData[ksfAS7343NumChannels]; // Array para guardar los datos

void setup() {
  // Iniciamos Serial rápido para que no frene la lectura de datos
  Serial.begin(115200);
  //while (!Serial) { delay(100); };

  Serial.println(">>> INICIANDO SISTEMA COMBINADO <<<");

  // 1. INICIAR MOTOR
  myMotor.setSpeed(10); // 10 RPM (velocidad segura para este motor)

  // 2. INICIAR SENSOR (I2C usa A4 y A5 automáticamente en Arduino Uno)
  Wire.begin();

  if (mySensor.begin() == false) {
    Serial.println("Error: Sensor AS7343 no encontrado. Revisa conexiones A4/A5.");
    while (1); // Bloqueo de seguridad
  }

  Serial.println("Sensor AS7343 conectado correctamente.");

  // Configuración del sensor
  if (!mySensor.powerOn()) Serial.println("Error al encender sensor.");
  if (!mySensor.setAutoSmux(AUTOSMUX_18_CHANNELS)) Serial.println("Error en AutoSmux.");
  if (!mySensor.enableSpectralMeasurement()) Serial.println("Error al activar medicion.");

  Serial.println(">>> SISTEMA LISTO <<<");
  delay(1000);
}

void loop() {
  
  // PASO 1: Mover el motor (Sentido Horario)
  Serial.println("--- Moviendo Motor (Posición 1) ---");
  myMotor.step(PASOS_POR_REVOLUCION / 2); // Gira media vuelta (180 grados)
  
  // PASO 2: Detener y Tomar Lectura
  tomarLecturaSensor();
  
  delay(1000); // Pequeña pausa antes de volver

  // PASO 3: Regresar el motor (Sentido Anti-Horario)
  Serial.println("--- Regresando Motor (Posición 0) ---");
  myMotor.step(-PASOS_POR_REVOLUCION / 2); // Regresa media vuelta
  
  // PASO 4: Tomar Lectura en el origen
  tomarLecturaSensor();

  delay(2000); // Espera 2 segundos antes de reiniciar el ciclo
}

// ==========================================
// FUNCIÓN AUXILIAR PARA LEER EL SENSOR
// ==========================================
void tomarLecturaSensor() {
    Serial.println("-> Tomando muestra espectral...");
    
    mySensor.ledOn();
    delay(100); // Tiempo para estabilizar la luz

    // Leer datos
    if (mySensor.readSpectraDataFromSensor() == false) {
        Serial.println("Error leyendo datos espectrales.");
    }

    mySensor.ledOff();

    // Obtener y mostrar datos
    int channelsRead = mySensor.getData(myData);

    Serial.print("DATOS: ");
    for (int i = 0; i < channelsRead; i++) {
        Serial.print(myData[i]);
        if (i < channelsRead - 1) Serial.print(",");
    }
    Serial.println(); // Salto de línea al final
}
