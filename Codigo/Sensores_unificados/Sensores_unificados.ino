#include <LiquidCrystal_I2C.h>                      // Librería del controlador de pantalla.
#include <SPI.h>                                    // Librería para el intercambio de datos hacia la SD
#include <SD.h>                                     // Librería de la memoria SD
#define ed 4                                        // Constante del puerto cs (Selector de esclavo)
File archivo;                                       // Se crea variable de tipo File en donde se guardarán los valores del 
LiquidCrystal_I2C lcd(0x27,16,2);                   // inicializa la interfaz I2C del LCD 16x2
int pulsePin              = 3;                      // Sensor de Pulso conectado al puerto A0
byte corazon []           =                         // Caracter en forma de corazon
  { 
    B00000,B00000,B01010,B11111,B11111,B11111,B01110,B00100
  };
// Estas variables son volatiles porque son usadas durante la rutina de interrupcion en la función interrupt para el sensor de pulso
volatile int BPM;                                   // Pulsaciones por minuto
volatile int Signal;                                // Entrada de datos del sensor de pulsos
volatile int IBI           = 600;                   // tiempo entre pulsaciones
volatile boolean Pulse     = false;                 // Verdadero cuando la onda de pulsos es alta, falso cuando es Baja
volatile boolean QS        = false;                 // Verdadero cuando el Arduino Busca un pulso del Corazon
// Variables del sensor de ultrasonido
const int trigger          = A1;                    // Pin digital parala salida de la señal
const int echo             = A2;                    // Pin digital para la entrada de la señal
const int frenoSilla       = 12;                    // Pin para encer el led que simula el frendo de la silla de ruedas
// Variables del sensor de pulso
const int ledPulso         = 40;                    // Led que simula las pulsaciones del corazón
// Variables del sensor de infrarrojo       
const int sensorInfrarrojo = 2;                     // Entrada del sensor infrarrojo
const int speakerPin       = 9;                     // Entrada del buzzer
// Variables del sensor de temperatura
const int ledVentilador2   = 8;                     // Led que simula los ventiladores
int contador               = 0;                     // Contador para el número de muestras de BPM
const int pulsadorTer      = 10;
int val;

void setup()
{ 
  Serial.begin(9600);                               // Puerto serial configurado a 9600 Baudios
  Serial.println("Iniciando memoria");              // Mensaje de inicio de memoria
  if(!SD.begin(ed))                                 // Se asegura que la memoria tengo tipo fat, fat34, que exista una memoria conectada y que no se encuentre dañada
  {
    Serial.println("conenxión fallida");
    return;                                         // En caso de tener algun problema, se sale del programa
  }
  Serial.print("Memoria inicializada con éxito");   // Mensaje de confirmación de conexión con una memoria sd
  archivo =SD.open("datos.txt", FILE_WRITE);        // Variable de tipo file se iguala con un archivo de tipo txt que se crea en la memoria SD y se define de tipo escritura
  lcd.init();                                       // Se inicializa el LCD 16x2
  lcd.backlight();                                  // Se enciende la luz que contrasta la pantalla       
  lcd.clear();                                      // Se deja limpia la pantalla        
  //Codigo para el sensor ultrasonido
  pinMode(trigger,          OUTPUT);                // pin como salida
  pinMode(echo,             INPUT);                 // pin como entrada
  digitalWrite(trigger,     LOW);                   // Inicializamos el pin con 0
  //Codigo para el sensor infrarrojo
  pinMode(sensorInfrarrojo, INPUT);                 // pin como entrada
  pinMode(speakerPin,       OUTPUT);                // pin como salida
  //Codigo para el sensor de temperatura
  pinMode(ledVentilador2,   OUTPUT);                // pin como salida
  //Codigo para medidor caridaco
  pinMode(ledPulso,         OUTPUT);                // pin como salida
  pinMode(pulsadorTer,      INPUT);                 // pin como entrada
  lcd.createChar(0, corazon);                       // Crea el caracter en forma de corazón para la pantalla
  interruptSetup();                                 // Configura la interrucion para leer el sensor de pulsos cada 2mS  
}
void loop()
{
  //Codigo para el sensor de infrarrojo
  int valorInfrarrojo;                              // Variable local que almacena el valor del sensor infrarrojo
  valorInfrarrojo = digitalRead(sensorInfrarrojo);  // Se iguala la variable local la resultado de 1-0 que arroja el sensor infrarrojo
  if(valorInfrarrojo == 0)                          // Pregunta si hay algun objeto en el rango del sensor infrarrojo
  {
    digitalWrite(speakerPin, HIGH);                 // Enciende el buzzer
  }     
  else
  {
   digitalWrite(speakerPin, LOW);                   // Apaga el buzzer cuando ya no hay un obstaculo
  }
  //Codigo para medidor caridaco
  int pulso = analogRead(A3);                       // Lee el valor del pulsometro conectado al puerto Analogo A0
  if (pulso >= 515)                                 // Enciende led 13 cuando el pulso pasa de un valor 
  {                                                 
    digitalWrite(ledPulso, HIGH);
  }  
  else
  {
    digitalWrite(ledPulso, LOW);
  }  
  lcd.setCursor(0,0);                               // Muestra en el LCD el valor de BPM en la primera posición
  lcd.print("BPM=");
  lcd.print(BPM);
  lcd.print("   ");   
  lcd.setCursor(7,0);                              
  lcd.write(0);                                     // Muestra en el LCD el valor del caracter creado en otra posición 
  Serial.println(pulso);                            // Muestra en el monitor el valor de BPM para poder construir la grafica
  Serial.println("BPM: "+ BPM);                     // Imprime el valor de los BPM en la pantalla Serial
  //Codigo para sensor ultrasonico
  Serial.print("Distancia (cm): ");
  Serial.println(ultrasonidoled());                 // Imprime el valor de la distancia del sensor ultrasonico
  if (QS == true)
  {                                                 // Bandera del Quantified Self es verdadera cuando el Arduino busca un pulso del corazon
    QS = false;                                     // Reset a la bandera del Quantified Self 
  }
  if(ultrasonidoled() <= 10)                        // Enciende un Led que representa la activación del freno de alguno de los lados de la silla de ruedas
  {
    digitalWrite(frenoSilla, HIGH);                 // Si hay una distancia menor a 10 cm. se enciende el led frenosSilla
  }
  else
  {
     digitalWrite(frenoSilla, 0);                   // En caso que la distancia sea mayor, el Led que representa los frenos se mantiene apagado
  }
  //Codigo del sensor infrarrojo
  infrarrojoSerial();
  // Codigo sensor de temperatura
  lcd.setCursor(0,1);                               // Ajusta la posición en la que se imprimen los datos de temperatura
  float imprimirT =temperaturalcd();                // Crea una variable local para guardar el valor de temperatura
  lcd.print(imprimirT,1);                           // Se ajustan los decimales de la variable a imprimir iguales a uno y se imprime
  lcd.setCursor(6,1);                               // Se imprime informaición adicional de temperatura
  lcd.print("Grados");
  delay(500);
  if(temperaturalcd() > 26)                         // Condicional para encender el led que representa los ventiladores en caso que la temperatura supere los 30 grados
  {
    digitalWrite(ledVentilador2,5);    
  }
  else                                              // Si la temperatura no llega a los 30 grados, el led que representa los ventiladores se mantiene apagado
  {
     digitalWrite(ledVentilador2,0);
  }
 
  if(true)
  {
      contador+=1;
  }
  if(archivo)                                       // Condicional que determina si efectivamente se creo el archivo para almacenar los valores
 {                                                  // Sumador para llevar el conteo de la muestra
    archivo.print(contador);
    archivo.print(",");
    archivo.print(BPM);                             // Impresión del valor cardiaco en el archivo de tipo txt separado por comas
    archivo.print(",");
    archivo.println(pulso); 
 }
 val=digitalRead(pulsadorTer);
if( val == HIGH )                                   // Condicional que determina si el boton que finaliza la toma de datos a la sd
    {
    Serial.println("Fin de toma de datos");
    delay(1000);
    archivo.close();                                // Comando para finalización de la toma de datos
    }
 }
