#include <Arduino.h>

//----------------------------------------------------------------------------------------------------------------------------------
// Una vez que se grabe en Wemos D1 ESP32 es posible que sea necesario pulsar botón RESET
void setup(){
pinMode(LED_BUILTIN, OUTPUT); // Prepara el pin como digital el pin LED_BUILTIN como salida.
}
// LED_BUILTIN se refiere al led construido en la placa
void loop()// Este bucle se ejecutará repetitivamente:
{
digitalWrite(LED_BUILTIN, HIGH);
// apaga el LED (HIGH es el nivel de voltaje) WEMOS D1 ESP32 trabaja con LÓGICA NEGATIVA
delay(1000); // Retardo de 1 segundo
digitalWrite(LED_BUILTIN, LOW); // Enciende el LED (el nivel de voltaje es LOW)
delay(1000); // Retardo de 1 segundo
}
// Nota en diversas placas el LED construido en la tarjeta opera con lógica negativa
// -------------------------------------------------------------------------------------------------------------------------