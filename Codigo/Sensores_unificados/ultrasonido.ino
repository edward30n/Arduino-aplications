int ultrasonidoled()              //Función para retornar la distancia de senso con el sensor ultrasonico
{
  long t;                         // Timepo que demora en llegar el eco
  long d;                         // Distancia en centimetros
  digitalWrite(trigger, HIGH);    //
  delayMicroseconds(10);          // Se envia un pulso de 10us
  digitalWrite(trigger, LOW);
  t = pulseIn(echo, HIGH);        // Se obtiene el ancho del pulso
  d = t/59;                       // Se divide en la velocidad del sonido de la zona donde se realizan las pruebas
  return(d);                      // Se retorna el resultado de los centimetros 
}
