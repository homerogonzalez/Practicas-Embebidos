#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
const char* ssid = "Potros_Wifi";
const char* password = "";
// Crear objeto AsyncWebServer en el puerto 80
AsyncWebServer server(80);
void initSPIFFS() {
if (!SPIFFS.begin(true)) {
Serial.println("Un error ha ocurrido mientras era montado SPIFFS");
}
Serial.println("SPIFFS montado exitosamente");
}
void initWiFi()
{
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
Serial.print("Conectando a la red WiFi ..");
while (WiFi.status() != WL_CONNECTED)
{ Serial.print('.');
delay(1000);
}
Serial.println(WiFi.localIP());
}
void setup() {
Serial.begin(115200);
initWiFi();
initSPIFFS();
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
{
request->send(SPIFFS, "/index.html", "text/html");
} );
/* Cuando recibe esa solicitud, envía el texto HTML guardado en el archivo index.html para construir
la página web. El primer argumento de la función send () es el sistema de archivos donde se guardan
los archivos, en este caso SPIFFS para el ESP32). El segundo argumento es la ruta donde se
encuentra el archivo. El tercer argumento se refiere al tipo de contenido (texto HTML).
Cuando el archivo HTML se carga en su navegador, solicitará el archivo CSS y el favicon. Estos son
archivos estáticos guardados en el mismo directorio (de SPIFFS o LittleFS). */
server.serveStatic("/", SPIFFS, "/");
server.begin();
}
void loop() {
}