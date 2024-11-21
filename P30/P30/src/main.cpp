#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
const char* ssid = "Potros_Wifi";
const char* password = "";
// Crea el objeto servidor mediante AsyncWebServer en el puerto 80
AsyncWebServer servidor(80);
// Define el pin del LED
const int ledPin = 2;
// Variable cadena para almacenar el estado del LED
String EstadoDeLED;
// Inicializa SPIFFS
void inicializaSPIFFS() {
if (!SPIFFS.begin(true)) {
Serial.println("Un error ha ocurrido mientras era montado SPIFFS");
}
Serial.println("SPIFFS montado exitosamente");
}
void inicializaWiFi()
{
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
Serial.print("Conectando a la red WiFi ..");
while (WiFi.status() != WL_CONNECTED)
{
Serial.print('.');
delay(1000);
}
Serial.println(WiFi.localIP());
}
String processor(const String& var)
{
/*La función processor () reemplaza los marcadores de posición
en el texto HTML con los valores actuales.
Cuando se solicita la página web, el ESP32 comprueba si el texto HTML
tiene marcadores de posición. Si encuentra el marcador de posición %ESTADO%,
lea el estado actual de GPIO con digitalRead (ledPin) y configure la variable
de valor EstadoDeLED en consecuencia. La función devuelve el estado actual de GPIO
como una variable de cadena. */
if(var == "ESTADO") {
if(digitalRead(ledPin)) {
EstadoDeLED = "ENCENDIDO";
}
else {
EstadoDeLED = "APAGADO";
}
return EstadoDeLED;
}
return String();
}
void setup() {
Serial.begin(115200);
inicializaWiFi();
inicializaSPIFFS();
// Establece GPIO2 como una SALIDA
pinMode(ledPin, OUTPUT);
servidor.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
{
request->send(SPIFFS, "/index.html", "text/html");
} );
/*
Cuando recibe esa solicitud, envía el texto HTML guardado en el archivo index.html
para construir la página web. También necesita pasar la función de procesador,
que reemplaza a todos los marcadores de posición en el texto HTML con los valores
correctos.
El primer argumento de la función send () es el sistema de archivos donde se guardan
los archivos, en este caso se guarda en SPIFFS (o LittleFS para el ESP8266).
El segundo argumento es la ruta donde se encuentra el archivo.
El tercer argumento se refiere al tipo de contenido (texto HTML).
*/
servidor.serveStatic("/", SPIFFS, "/");
// Procedimiento para establecer el GPIO 2 en estado ALTO
servidor.on("/encendido", HTTP_GET, [](AsyncWebServerRequest *request)
{
digitalWrite(ledPin, HIGH); // enciende
request->send(SPIFFS, "/index.html", "text/html", false, processor);
}
);
// Procedimiento para establecer el GPIO 2 en estado BAJO
servidor.on("/apagado", HTTP_GET, [](AsyncWebServerRequest *request)
{
digitalWrite(ledPin, LOW); // apaga
request->send(SPIFFS, "/index.html", "text/html", false, processor); /* El cuarto argumento significa
descarga = falso.
El primer argumento de la función send () es el sistema de archivos donde se guardan
los archivos, en este caso se guarda en SPIFFS (o LittleFS para el ESP8266).
El segundo argumento es la ruta donde se encuentra el archivo.
El tercer argumento se refiere al tipo de contenido (texto HTML).
El cuarto argumento significa descarga = falso.
Finalmente, el último argumento es la función del procesador.
Cuando el archivo HTML se carga en su navegador, solicitará el archivo CSS y el favicon.
Estos son archivos estáticos guardados en el mismo directorio (de SPIFFS o LittleFS). */
}
);
servidor.begin();
}
void loop() { }