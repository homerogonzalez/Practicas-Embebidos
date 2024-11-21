#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
const char* ssid = "Potros_Wifi";
const char* password = "";
// Crea un objeto server en el puerto 80
AsyncWebServer server(80);
/*La library AsyncWebSocket provee el método .textAll (se observa más adelante en este código) para
estar enviando el mismo mensaje a todos los clientes al mismo tiempo */
// Crea un objeto ws de tipo WebSocket
AsyncWebSocket ws("/ws");
// Establece el pin que se va a usar
const int ledPin = 2;
// Almacena el estado original del LED
bool estadodeLED = 0;
// Inicializa SPIFFS
void inicializaSPIFF() {
if (!SPIFFS.begin(true)) {
Serial.println("Un error ha ocurrido mientras se montaba SPIFFS");
}
else {
Serial.println("SPIFFS montado exitosamente");
}
}
// Inicializa WiFi
void inicializaWiFi() {
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
Serial.print("Connecting to WiFi ..");
while (WiFi.status() != WL_CONNECTED) {
Serial.print('.');
delay(1000);
}
Serial.println(WiFi.localIP());
}
// Reemplaza marcador de posición (placeholder) con el valor del estado del LED
String processor(const String& var) {
/* cuando la página web es solicitada, el ESP8266 verifica si el texto HTML
tiene marcadores de posición, se lee el estado actual del GPIO con
digitalRead(ledPin) se establece el valor de la variable de acuerdo al
estado en que se encentre el LED) */
if(var == "ESTADO") {
if(digitalRead(ledPin)) {
  estadodeLED = 1;
return "ON";
}
else {
estadodeLED = 0;
return "OFF";
}
}
return String();
}
void notifyClients(String state) {
// notifica el estado a todos los clientes
/* les envía el mismo mensaje a todos los clientes que estén
conectados, les hace el envío al mismo tiempo */
/* La library AsyncWebSocket provee el método .textAll para estar
enviando el mismo mensaje a todos los clientes al mismo tiempo */
ws.textAll(state);
}
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
/* La función handleWebSocketMessage () es una función de devolución
de llamada que se ejecutará siempre que recibamos nuevos datos de los
clientes a través del protocolo WebSocket.*/
AwsFrameInfo *info = (AwsFrameInfo*)arg;
/*Si recibimos el mensaje "bON", se establece el valor de la variable
EstadoDeLED en 0 y se notifica a todos los clientes llamando a la función
notifyClients ().*/
if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
data[len] = 0;
if (strcmp((char*)data, "bON") == 0) {
estadodeLED = 1;
notifyClients("ON"); //Notifica a todos los clientes
}
/*Si recibimos el mensaje "bAPAGADO", se establece el valor de la variable
EstadoDeLED en 1 y se notifica a todos los clientes llamando a la función
notifyClients ().*/
if (strcmp((char*)data, "bOFF") == 0) {
estadodeLED = 0;
notifyClients("OFF"); //Notifica a todos los clientes
}
}
}
/* Ahora necesitamos configurar un detector de eventos para manejar los
diferentes pasos asincrónicos pasos del protocolo WebSocket.
Este controlador de eventos se puede implementar definiendo
onEvent () de la siguiente manera: */
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
void *arg, uint8_t *data, size_t len) {
// El argumento de tipo representa el evento que ocurre. Puede tomar los siguientes valores:
switch (type) {
case WS_EVT_CONNECT: // cuando un cliente ha iniciado sesión
Serial.printf("WebSocket cliente #%u conectado de %s\n", client->id(), client->remoteIP().toString().c_str());
break;
case WS_EVT_DISCONNECT: // cuando un cliente se desconecta
Serial.printf("WebSocket cliente #%u desconectado \n", client->id());
break;
case WS_EVT_DATA: // cuando se recibe un paquete de datos del cliente
handleWebSocketMessage(arg, data, len);
break;
case WS_EVT_PONG:
// en respuesta a una solicitud de ping
/* Ping es un comando o una herramienta de diagnóstico que permite hacer una verificación
del estado de una determinada conexión o host local.
Es un acrónimo para Packet Internet Groper, lo que literalmente significa
“buscador de paquetes en redes”. Se trata de un comando que permite verificar el estado de
una conexión para determinar si una dirección IP específica o host es accesible desde la red o no. */
case WS_EVT_ERROR: // cuando se recibe un error del cliente.
break;
}
}
void inicializaWebSocket() { // inicializa el protocolo WebSocket
ws.onEvent(onEvent);
server.addHandler(&ws);
}
void setup() {
pinMode(ledPin, OUTPUT);
Serial.begin(115200);
inicializaSPIFF();
inicializaWiFi();
inicializaWebSocket();// inicializa el protocolo Websocket
/* Las siguientes líneas manejan lo que sucede cuando recibe una
solicitud en la raíz (/) URL (dirección IP de ESP32).
Cuando recibe esa solicitud, envía el texto HTML guardado en el archivo
index.html a construir la página web. También necesita pasar la función
del procesador como argumento para que reemplace todos los marcadores
de posición con los valores correctos. */
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
request->send(SPIFFS, "/index.html", "text/html",false, processor);
});
/* El primer argumento de la función send () es el sistema de archivos donde
se guardan los archivos, en este caso se guarda en SPIFFS (o LittleFS en
el caso del ESP8266).
El segundo El argumento es la ruta donde se encuentra el archivo.
El tercer argumento se refiere a la tipo de contenido (texto HTML).
El tercer argumento significa descarga = falso.
Finalmente, el último argumento es la función del procesador. */
/* Cuando el archivo HTML se cargue en su navegador, solicitará el CSS,
Archivos JavaScript y favicon. Estos son archivos estáticos guardados en
el mismo directorio (SPIFFS o LittleFS). Entonces, podemos simplemente
agregar la siguiente línea para servir archivos en un directorio cuando
sea solicitado por la URL raíz. Servirá los archivos CSS y favicon
automáticamente (también se agrega una imagen en este ejemplo) */
server.serveStatic("/", SPIFFS, "/");
// Inicia el servidor
server.begin();
}
void loop() {
/* Los navegadores a veces no cierran correctamente
la conexión WebSocket, incluso cuando se llama a la función close () en
JavaScript. Esto eventualmente agotará los recursos del servidor web y hará
que el servidor se bloquee. Llamar periódicamente a la función
cleanupClients () desde el bucle principal () limita el número de clientes
cerrando el cliente más antiguo cuando se ha superado el número máximo
de clientes. Esto se puede llamar en cada ciclo, sin embargo, si desea
usar menos energía, entonces es suficiente llamar con tan poca frecuencia
como una vez por segundo. Si se incluye el valor 50 entre paréntesis podemos
ampliar la cantidad de clientes que perciben los cambios en la página web
del servidor embebido*/
ws.cleanupClients();
digitalWrite(ledPin, estadodeLED);
}