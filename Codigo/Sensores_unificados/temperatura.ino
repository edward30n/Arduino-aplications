float temperaturalcd()                                   // Función para retornar unicamente el promedio de temperaturas y reportarlo en la pantalla lcd
{
 int reading = analogRead(A0);                         // Se toma el volataje leido del sensor de temperatura
 float sumaTemp = 0;                                   // Variable que almacena la suma de los valores alacenados en el vector
 float promedioTemp=0;
 float tempVector[4];                                 // Vector que almacena 20 valores de temperatura
 float voltage = reading * 5.0;                        // converting that reading to voltage, for 3.3v arduino use 3.3
 voltage /= 1024.0;                                    // Valor maximo del sensor (1024) bytes
 float temperatureC = (voltage - 0.5) * 100 ;          // Se convierte cada 10 milivoltios por grado con un valor de "offset" de 500 mV, a grados: ((voltage - 500mV) times 100)
 for(int i = 0; i <= 4; i ++)                         // for para recorrer la totalidad de la matriz y almacenar valores sensados de temperatura
 {
    tempVector[i]=temperatureC;             
 }
 for(int i=0; i<=4; i++)                              // Sumatoria de los valores almacenados en la matriz
 {
    sumaTemp+=tempVector[i];
 }
 
 promedioTemp =sumaTemp/4;                            // Promedio de la sumatoria de valores
 return(temperatureC-6);
}
