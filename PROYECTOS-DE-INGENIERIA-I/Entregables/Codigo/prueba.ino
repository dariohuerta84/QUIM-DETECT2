#include <AccelStepper.h>
#include <SparkFun_AS7343.h>
#include <Wire.h>

// ================================================================
// >>> CONFIGURACIÓN MOTOR (28BYJ-48 / Actuador) <<<
// ================================================================
#define MOTOR_INTERFACE_TYPE 4
// Pines del motor: IN1=10, IN2=9, IN3=8, IN4=7
AccelStepper stepper(MOTOR_INTERFACE_TYPE, 11, 10, 9, 8);

// He movido estos pines para que no choquen con el pin 7 del motor
const int ENA = 6; 
const int ENB = 7; 

// AJUSTES DE MOVIMIENTO
// NOTA: Para un 28BYJ-48, 200 pasos es muy poco (apenas un girito).
// Una vuelta completa son aprox 2048 pasos. Ajusta DISTANCIA_TOTAL si es necesario.
const long DISTANCIA_TOTAL = 200; 
const unsigned long TIEMPO_PARADA_MS = 1125; 

const float VELOCIDAD_MAX = 2000.0;
const float ACELERACION   = 4000.0;

// Puntos de parada
const long POSICION_INICIO = 0;
const long POSICION_MEDIA = DISTANCIA_TOTAL / 2; 
const long POSICION_FINAL = DISTANCIA_TOTAL;    

// Estados del sistema
enum Estado { INICIO_AVANCE, PARADA_MEDIA, AVANCE_FINAL, PARADA_FINAL, RETORNO, COMPLETADO };
Estado estadoActual = INICIO_AVANCE;
unsigned long tiempoParadaInicio = 0; 
bool medicionRealizada = false; // Control para medir solo una vez por parada

// ================================================================
// >>> CONFIGURACIÓN SENSOR (AS7343) <<<
// ================================================================
SfeAS7343ArdI2C mySensor;
uint16_t myData[ksfAS7343NumChannels]; // Array para datos espectrales

// ================================================================
// >>> SETUP <<<
// ================================================================
void setup() {
  Serial.begin(115200); // Subí la velocidad para que la lectura de datos sea rápida
  while (!Serial) delay(100);

  // --- SETUP MOTOR ---
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);

  stepper.setMaxSpeed(VELOCIDAD_MAX);      
  stepper.setAcceleration(ACELERACION);  
  
  // --- SETUP SENSOR ---
  Wire.begin();
  if (mySensor.begin() == false) {
    Serial.println("Error: No se detecta el sensor AS7343. Revisa cables.");
    while (1); // Se queda aquí si falla
  }
  
  if (!mySensor.powerOn()) Serial.println("Fallo al encender LED sensor");
  if (!mySensor.setAutoSmux(AUTOSMUX_18_CHANNELS)) Serial.println("Fallo AutoSmux");
  if (!mySensor.enableSpectralMeasurement()) Serial.println("Fallo activar medicion");

  Serial.println(">>> SISTEMA LISTO: INICIO CICLO <<<");
  
  // Iniciar primer movimiento
  stepper.moveTo(POSICION_MEDIA);
}

// ================================================================
// >>> LOOP PRINCIPAL <<<
// ================================================================
void loop() {
  
  // 1. SIEMPRE ejecutar el motor (es vital para AccelStepper)
  //    Excepto si ya completamos todo.
  if (estadoActual != COMPLETADO) {
    stepper.run();
  }

  // 2. Máquina de Estados (Lógica)
  
  // Si el motor llegó a su destino (distanceToGo == 0) verificamos qué hacer
  if (stepper.distanceToGo() == 0) {
    
    unsigned long currentMillis = millis();

    switch (estadoActual) {
      
      case INICIO_AVANCE:
        // Acaba de llegar a la MITAD
        tiempoParadaInicio = currentMillis;
        medicionRealizada = false; // Preparamos para medir
        estadoActual = PARADA_MEDIA;
        Serial.println("-> Llegada a la MITAD. Esperando y midiendo...");
        break;

      case PARADA_MEDIA:
        // Aquí hacemos dos cosas: Medir y Esperar
        
        // A) Intentar medir si no lo hemos hecho aún
        if (!medicionRealizada) {
           tomarLecturaSensor("POSICION MEDIA");
           medicionRealizada = true; // Marcar como hecho para no repetir
        }

        // B) Verificar si ya pasó el tiempo de espera
        if (currentMillis - tiempoParadaInicio >= TIEMPO_PARADA_MS) {
          Serial.println("-> Moviendo al FINAL...");
          stepper.moveTo(POSICION_FINAL); 
          estadoActual = AVANCE_FINAL;
        }
        break;

      case AVANCE_FINAL:
        // Acaba de llegar al FINAL
        tiempoParadaInicio = currentMillis;
        medicionRealizada = false; // Preparamos para medir
        estadoActual = PARADA_FINAL;
        Serial.println("-> Llegada al FINAL. Esperando y midiendo...");
        break;

      case PARADA_FINAL:
        // A) Intentar medir
        if (!medicionRealizada) {
           tomarLecturaSensor("POSICION FINAL");
           medicionRealizada = true;
        }

        // B) Verificar tiempo
        if (currentMillis - tiempoParadaInicio >= TIEMPO_PARADA_MS) {
          Serial.println("-> Retornando a INICIO...");
          stepper.moveTo(POSICION_INICIO); 
          estadoActual = RETORNO;
        }
        break;

      case RETORNO:
        // Llegó al inicio (0)
        Serial.println(">>> CICLO COMPLETADO <<<");
        estadoActual = COMPLETADO;
        
        // Apagar motores para ahorrar energía/calor
        digitalWrite(ENA, LOW);
        digitalWrite(ENB, LOW);
        stepper.disableOutputs(); 
        break;
        
      case COMPLETADO:
        // Nada que hacer
        break;
    }
  }
}

// ================================================================
// >>> FUNCIÓN AUXILIAR DE LECTURA <<<
// ================================================================
void tomarLecturaSensor(String etiqueta) {
  Serial.print("LEYENDO DATOS EN: ");
  Serial.println(etiqueta);

  mySensor.ledOn();
  // Pequeña espera para estabilizar luz (delay bloqueante pequeño es aceptable aquí
  // porque el motor está detenido en este estado)
  delay(50); 

  if (mySensor.readSpectraDataFromSensor()) {
     int channelsRead = mySensor.getData(myData);
     
     Serial.print("DATA: ");
     for (int i = 0; i < channelsRead; i++) {
        Serial.print(myData[i]);
        Serial.print(",");
     }
     Serial.println();
  } else {
    Serial.println("Error leyendo sensor.");
  }
