// Conexion Sensor Pulsos -> Arduino UNO:   S -> A0 , Vcc -> 5V , (-) -> GND
#include <LiquidCrystal_I2C.h> //Librería del controlador de pantalla.
#include <SPI.h>
#include <SD.h>
#define ed 4
File archivo;
LiquidCrystal_I2C lcd(0x27,16,2);   // inicializa la interfaz I2C del LCD 16x2
int pulsePin =3 ;                   // Sensor de Pulso conectado al puerto A0
// Estas variables son volatiles porque son usadas durante la rutina de interrupcion en la segunda Pestaña
volatile int BPM;                   // Pulsaciones por minuto
volatile int Signal;                // Entrada de datos del sensor de pulsos
volatile int IBI = 600;             // tiempo entre pulsaciones
volatile boolean Pulse = false;     // Verdadero cuando la onda de pulsos es alta, falso cuando es Baja
volatile boolean QS = false;        // Verdadero cuando el Arduino Busca un pulso del Corazon
const int ledPulso = 40;
const int boton=10;
int boton1;
void setup(){ 
   Serial.begin(9600);                // Puerto serial configurado a 9600 Baudios
   Serial.println("Iniciando memoria");
  if (!SD.begin(ed))
  {
    Serial.println("conenxión fallida");
    return;
  }
  Serial.print("Memoria inicializada con éxito");
  archivo =SD.open("datoss.txt", FILE_WRITE);
   //Codigo para el sensor ultrasonido
 
  //Codigo para medidor caridaco
 pinMode(ledPulso, OUTPUT); 
 pinMode(boton,INPUT);
 lcd.init();                         // Se inicializa el LCD 16x2
 lcd.backlight();                           
 lcd.clear();                                
 interruptSetup();                  // Configura la interrucion para leer el sensor de pulsos cada 2mS  
}
void loop(){
 //Codigo para medidor caridaco
 int pulso = analogRead(A3);           //Lee el valor del pulsometro conectado al puerto Analogo A0
 if (pulso >= 515) {                   // Enciende led 13 cuando el pulso pasa de un valor (debe ajustarse)
    digitalWrite(ledPulso, HIGH);
 }  
 else{
    digitalWrite(ledPulso, LOW);
 }  
   lcd.setCursor(0,0);                   //Muestra en el LCD el valor de BPM 
   lcd.print("BPM=");
   lcd.print(BPM);
   lcd.print("   ");   

   Serial.println(pulso);  //Muestra en el monitor el valor de BPM 
  // Serial.println(BPM);
                           // envia el valor del pulso por el puerto serie  (desabilitarla si habilita la anterior linea)
  if (QS == true){                       // Bandera del Quantified Self es verdadera cuando el Arduino busca un pulso del corazon
    QS = false;                          // Reset a la bandera del Quantified Self 
  }
  if(archivo)                                       // Condicional que determina si efectivamente se creo el archivo para almacenar los valores
 {                                // Sumador para llevar el conteo de la muestra
  int contador =1;
    archivo.print(contador);
    archivo.print(",");
    archivo.print(BPM);                             // Impresión del valor cardiaco en el archivo de tipo txt separado por comas
    archivo.print(",");
    archivo.println(pulso);
    
 }
 boton1=digitalRead(boton);
 
if( boton1 == HIGH )
    {
    Serial.println("Fin de toma de datos");
    delay(1000);
    archivo.close();
    }
}
