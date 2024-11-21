#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
const char* ssid = "Note20";
const char* password = "homero123";
// Crea un objeto AsyncWebServer en el puerto 80
AsyncWebServer server(80);
// Crea una fuente de eventos en /events
AsyncEventSource events("/events");
// Define cliente NTP Client para obtener fecha y hora
WiFiUDP ntpUDP;
NTPClient tiempoCliente(ntpUDP, "pool.ntp.org");
// variable Json para captar las lecturas del sensor
JSONVar lecturas;
// Nombre del archivo en el que serán guardadas las lecturas
const char* rutaDeDatos = "/data.txt";
// Variables de temporización
unsigned long UltimoTiempo = 0;
// unsigned long RetardoDeTiempo = 60000; // 1 minuto = 1*60*1000 ms
unsigned long RetardoDeTiempo = 10000;
//unsigned long RetardoDeTiempo = 1800000;
//unsigned long RetardoDeTiempo = 1800000; //media hora 30*60*1000 ms
// Crea un objeto del sensor bme
Adafruit_BME280 bme; /* BME280 se conecta a la Wemos D1
por I2C (SDA = SDA, SCL = SCL */
// Inicializa BME280
void inicializaBME(){
if (!bme.begin(0x76)) {
Serial.println("Podría no ser un sensor válido BME280, verificar el alambrado!");
while (1);
}
}
// Función que obtiene el tiempo actual
unsigned long obtieneTiempo() {
tiempoCliente.update();
unsigned long ahora = tiempoCliente.getEpochTime();
return ahora;
}
// Obtiene las lecturas del sensor y devuelve objeto JSON
String obtieneLecturasDelSensor(){
lecturas["tiempo"] = String(obtieneTiempo());
lecturas["temperatura"] = String(bme.readTemperature());
lecturas["humedad"] = String(bme.readHumidity());
lecturas["presion"] = String(bme.readPressure()/100.0F);
String jsonString = JSON.stringify(lecturas);
return jsonString;
}
// Inicializa SPIFFS
void inicializaSPIFFS() {
if (!SPIFFS.begin()) {
Serial.println("Un error ha ocurrido mientras se montaba SPIFFS");
}
Serial.println("SPIFFS montado exitosamente");
}
// Lee archivo desde SPIFFS
String readFile(fs::FS &fs, const char * path){
Serial.printf("Leyendo archivo: %s\r\n", path);
File archivo = fs.open(path, "r");
if(!archivo || archivo.isDirectory()){
Serial.println("- falló al abrir el archivo o al leerlo");
return String();
}
String contenidoDeArchivo;
while(archivo.available()){
contenidoDeArchivo += archivo.readStringUntil('\n');
break;
}
archivo.close();
return contenidoDeArchivo;
}
// Agregar dato al archivo en SPIFFS
void anadirDatoaArchivo(fs::FS &fs, const char * path, const char * mensaje){
Serial.printf("Añadiendo dato al archivo: %s\r\n", path);
File archivo = fs.open(path, "a");
if(!archivo){
Serial.println("- falló al abrir el archivo o al agregar dato");
return;
}
if(archivo.print(mensaje)){
Serial.println("- mensaje agregado");
} else {
Serial.println("- falló al agregar");
}
archivo.close();
}
// Borrando archivo
void borraArchivo(fs::FS &fs, const char * path){
Serial.printf("Borrando archivo: %s\r\n", path);
if(fs.remove(path)){
Serial.println("- Archivo borrado");
} else {
Serial.println("- falló el borrado de archivo");
}
}
// Obtener el tamaño de archivo
int obtieneTamanoDeArchivo(fs::FS &fs, const char * path){
File archivo = fs.open(path, "r");
if(!archivo){
Serial.println("Falló al abrir el archivo o al verificar su tamaño");
return 0;
}
Serial.print("Tamaño de archivo: ");
Serial.println(archivo.size());
return archivo.size();
}
// Inicializa WiFi
void inicializaWiFi() {
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
Serial.print("Conectando a la red WiFi ..");
while (WiFi.status() != WL_CONNECTED) {
Serial.print('.');
delay(1000);
}
Serial.println(WiFi.localIP());
}
void setup() {
Serial.begin(115200);
Serial.print("ok");
inicializaBME();
inicializaWiFi();
inicializaSPIFFS();
// Inicializa NTPClient para obtener el tiempo
tiempoCliente.begin();
/* Es esta parte debe ajustarse el tiempo offset en segundos de acuerdo a la
zona horaria, por ejemplo:
GMT +1 = 3600
GMT +8 = 28800
GMT -1 = -3600
GMT 0 = 0
GMT -7 = -25200 7*60*60 */
tiempoCliente.setTimeOffset(-25200);
// Crea archivo datos.txt
File archivo = SPIFFS.open(rutaDeDatos, "r");
if(!archivo) {
Serial.println("El archivo no existe");
Serial.println("Creando archivo...");
// Prepara lecturas para añadir al archivo
String mensaje = obtieneLecturasDelSensor() + ",";
// Añade datos al archivo para crearlo
anadirDatoaArchivo(SPIFFS, rutaDeDatos, mensaje.c_str());
}
else {
Serial.println("El archivo ya existe");
}
archivo.close();
// Raíz URL del servidor web
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
request->send(SPIFFS, "/index.html", "text/html");
});
server.serveStatic("/", SPIFFS, "/");
// Petición de las últimas lecturas del sensor
server.on("/lecturas", HTTP_GET, [](AsyncWebServerRequest *request){
request->send(SPIFFS, "/data.txt", "text/txt");
});
// Petición de datos crudos
server.on("/visualizar-datos", HTTP_GET, [](AsyncWebServerRequest *request){
request->send(SPIFFS, "/data.txt", "text/txt");
});
// Petición para borrar datos
server.on("/borrar-datos", HTTP_GET, [](AsyncWebServerRequest *request){
borraArchivo(SPIFFS, rutaDeDatos);
request->send(200, "text/plain", "data.txt ha sido borrado.");
});
events.onConnect([](AsyncEventSourceClient *client){
if(client->lastId()){
Serial.printf("Cliente reconectado! El Ultimo mensaje de ID que se obtuvo es: %u\n", client->lastId());
}
// envía evento con mensaje "hola!", id millis actuales
// y establece la reconexión después de 10 s
client->send("hola!", NULL, millis(), 10000);
});
server.addHandler(&events);
// inicia el servidor
server.begin();
events.send(obtieneLecturasDelSensor().c_str(),"nuevas_lecturas" ,millis());
}
void loop() {
if ((millis() - UltimoTiempo) > RetardoDeTiempo) {
// Envía eventos al cliente con las lecturas del sensor cada media 30 minutos
events.send("ping",NULL,millis());
events.send(obtieneLecturasDelSensor().c_str(),"nuevas_lecturas" ,millis());
String mensaje = obtieneLecturasDelSensor() + ",";

if ((obtieneTamanoDeArchivo(SPIFFS, rutaDeDatos))>= 3400){
Serial.print("Demasiados puntos de datos, borrando archivo...");
/* Hacer comentario a las siguientes dos líneas de código si no se desea que
el archivo de datos se borre automáticamente
en esa situación no almacenará más datos */
borraArchivo(SPIFFS, rutaDeDatos);
anadirDatoaArchivo(SPIFFS, "/data.txt", mensaje.c_str());
}
else{
// Agregar nuevas lecturas al archivo
anadirDatoaArchivo(SPIFFS, "/data.txt", mensaje.c_str());
}
UltimoTiempo = millis();
Serial.print(readFile(SPIFFS, rutaDeDatos));
}
}