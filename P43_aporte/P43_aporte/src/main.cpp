#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
const char* ssid = "Note20";
const char* password = "homero123";
// Crea un objeto AsyncWebServer en el puerto 80
AsyncWebServer server(80);
// Crea una fuente de Event en /eventos
AsyncEventSource events("/eventos");
// Variable JSON para captar las lecturas del sensor
JSONVar lecturas;
// variables de temporización
unsigned long UltimaOcasion = 0;
unsigned long RetardoDeTiempo = 5000; // cada 5 segundos se leerá el sensor
#define presion_nivel_mar (1013.25)
// Crea un objeto sensor
Adafruit_BME280 bme; // Wemos y BME280 (SCL = SCL, SDA = SDA)
// Inicializa BME280
void inicializarSensorBME(){
if (!bme.begin(0x76))
{ /* La dirección I2C predeterminada del módulo BME280 es 0x76HEX y
se puede cambiar a 0x77HEX fácilmente con el puente de soldadura
del chip.
*/
Serial.println("Podría no encontrarse un sensor válido BME280, verifique el alambrado!");
while (1);
}
}
// Obtener las lecturas del sensor y retornar un objeto JSON
String obtieneLecturasDelSensor(){
lecturas["temperatura"] = String(bme.readTemperature());
lecturas["humedad"] = String(bme.readHumidity());
lecturas["presion"] = String(bme.readPressure()/100.0F);
lecturas["altura"] = String(bme.readAltitude(presion_nivel_mar));
String cadenaJSON = JSON.stringify(lecturas);
return cadenaJSON;
}
// Initicializa SPIFFS
void inicializarSPIFFS() {
if (!SPIFFS.begin()) {
Serial.println("Un error ha ocurrido mientras se estaba montando SPIFFS");
}
Serial.println("SPIFFS montado exitosamente");
}
// Inicializa WiFi
void inicializarWiFi() {
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
Serial.print("Conectando a WiFi ..");
while (WiFi.status() != WL_CONNECTED) {
Serial.print('.');
delay(1000);
}
Serial.println(WiFi.localIP());
}
void setup() {
Serial.begin(115200);
inicializarSensorBME();
inicializarWiFi();
inicializarSPIFFS();
// Raíz URL del Servidor Web
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
request->send(SPIFFS, "/index.html", "text/html");
});
server.serveStatic("/", SPIFFS, "/");
// Solicitud de las últimas lecturas del sensor
server.on("/lecturas", HTTP_GET, [](AsyncWebServerRequest *request)
{
String VariableJSON = obtieneLecturasDelSensor(); /* La variable variableJSON contiene
el retorno de la función obtieneLecturasDelSensor (). */
request->send(200, "application/json", VariableJSON);
/* al enviar una cadena JSON como respuesta, el método send () acepta como
primer argumento la respuesta código (200),
el segundo es el tipo de contenido ("application/json")
y finalmente el contenido de la variable (variableJSON).*/
VariableJSON = String();
});
events.onConnect([](AsyncEventSourceClient *client)
{
if(client->lastId())
{
Serial.printf("Cliente reconectado! El último mensaje ID que se obtuvo es: %u\n", client->lastId());
}
client->send("hola!", NULL, millis(), 10000); /* envía el mensaje "hola!",
identificador NULL, en milisegundos y establece la reconexión con un
retardo de 10 segundos */
});
server.addHandler(&events);
// Inicia el servidor
server.begin();
}
void loop() {
if ((millis() - UltimaOcasion) > RetardoDeTiempo)
{
// Envía eventos al cliente con lecturas del sensor cada 5 segundos
events.send("ping",NULL,millis());
events.send(obtieneLecturasDelSensor().c_str(),"nuevas_lecturas" ,millis());
UltimaOcasion = millis();

// Serial.print("Altitud aproximada = ");
// Serial.print(bme.readAltitude(presion_nivel_mar));
// Serial.println(" metros ");

/* en https://es.wikipedia.org/wiki/Ciudad_Obregón se indica
una altitud de Altitud Media 40 y 38 m s. n. m.
El sensor BME280 indica 42 m */
}
}