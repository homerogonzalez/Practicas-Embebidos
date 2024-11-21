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
// Crea una fuente de eventos en /eventos
AsyncEventSource events("/eventos");
// Variable JSON para guardar las lecturas del sensor
JSONVar lecturas;
// Variables de tiempo:
unsigned long UltimoTiempo = 0;
unsigned long TiempoDeRetardo = 10000; // lecturas cada 30 s.
#define presion_nivel_mar (1013.25)
// Crea un objeto sensor
Adafruit_BME280 bme; // BME280 se conecta a la Wemos D1, crea objeto bme
// por I2C (GPIO SDA = SDA, GPIO SCL = SCL)
// Inicializa BME280
void inicializaSensorBME()
{
if (!bme.begin(0x76)) {
Serial.println("Podría no estar conectado el sensor BME280, verificar las conexiones");
while (1);
}
}
// Obtener las lecturas y devolver un objeto JSON
String obtenerLecturasDelSensor()
{
lecturas["temperatura"] = String(bme.readTemperature());
lecturas["humedad"] = String(bme.readHumidity());
lecturas["presion"] = String(bme.readPressure()/100.0F);
lecturas["altura"] = String(bme.readAltitude(presion_nivel_mar));
String CadenaJSON = JSON.stringify(lecturas);
return CadenaJSON;
}
// Inicializa SPIFFS
void inicializaSPIFFS() {
if (!SPIFFS.begin()) {
Serial.println("Un error ha ocurrido mientras se montaba SPIFFS");
}
Serial.println("SPIFFS montado exitosamente");
}
// Inicializa WiFi
void iinicializaWiFi() {
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
Serial.print("Conectando a red WiFi ..");
while (WiFi.status() != WL_CONNECTED) {
Serial.print('.');
delay(1000);
}
Serial.println(WiFi.localIP());
}
void setup() {
// Puerto serie para propósitos de depuración
Serial.begin(115200);
inicializaSensorBME(); // inicializa el sensor BME280
iinicializaWiFi(); // inicializa Wi Fi
inicializaSPIFFS(); // inicializa SPIFFS
// raíz URL del Servidor Web:
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
request->send(SPIFFS, "/index.html", "text/html");
});
server.serveStatic("/", SPIFFS, "/");
// Solicita las últimas lecturas del sensor BME280
server.on("/lecturas", HTTP_GET, [](AsyncWebServerRequest *request){
String VariableJSON = obtenerLecturasDelSensor();
request->send(200, "application/json", VariableJSON);
VariableJSON = String();
});
events.onConnect([](AsyncEventSourceClient *client){
if(client->lastId()){
Serial.printf("Cliente reconectado! ID de último mensaje ID que se obtuvo: %u\n", client->lastId());
}
// envía evento con el mensaje "hola!", ID, milésimas actuales
// y establece la reconexión en 10 segundos
client->send("hola!", NULL, millis(), 10000);
});
server.addHandler(&events);
// inicia el servidor
server.begin();
}
void loop() {
if ((millis() - UltimoTiempo) > TiempoDeRetardo) {
// Envía eventos al cliente con las lecturas del sensor cada 30 segundos
events.send("ping",NULL,millis());
events.send(obtenerLecturasDelSensor().c_str(),"nuevas_lecturas" ,millis());
UltimoTiempo = millis();
}
}