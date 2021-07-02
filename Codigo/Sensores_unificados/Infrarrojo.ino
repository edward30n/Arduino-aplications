int valorInfrarrojo;                                  // Variable que almacena los valores del sensor Infrarrojo
int infrarrojo()                                      // Función que se llama en la pestaña principal para poder imprimir en el monitor seríal la información de 
{ 
  valorInfrarrojo = digitalRead(sensorInfrarrojo);    // Asignación del sensor a la constante
  if(valorInfrarrojo == 0)                            // Condicional para encender la alarma en caso de detectar un objeto no identificado
  {
    digitalWrite(speakerPin, HIGH);
  }     
  else                                                // En caso de que no se detecte nada, el buzzer se mantiene apagado 
  {
    digitalWrite(speakerPin, LOW); 
  }
}
int infrarrojoSerial()                                // Función que se llama en la pestaña principal para poder imprimir en el monitor seríal la información de 
{ 
  valorInfrarrojo = digitalRead(sensorInfrarrojo);      
  Serial.print("Valor de obstaculo: ");               // Impresión de los valores en el monitor serial
  Serial.println(valorInfrarrojo);
}
