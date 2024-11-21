#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Arduino_JSON.h>
const char* ssid = "Potros_Wifi";
const char* password = "";
// Crea el objeto AsyncWebServer en el puerto 80
AsyncWebServer server(80);
/* La biblioteca ESPAsyncWebServer incluye un complemento WebSocket que facilita el
manejar las conexiones WebSocket. Crear un objeto AsyncWebSocket llamado ws para
manejar las conexiones en la ruta / ws. */
AsyncWebSocket ws("/ws");
// Establece el número de salidas
#define NUM_SALIDAS 4
// Asigna los números de salidas
// En Wemos D1: 5 está en /D3 y 13 está en /D7
int salidaGPIOs[NUM_SALIDAS] = {4, 5, 12, 13};
/* El código está preparado para controlar los GPIO 4, 5, 12 y 13. Puede modificarse
la cantidad de NUM_SALIDAS y variables salidaGPIOs para cambiar el número de GPIO y
cuáles los que se requiere controlar.
La variable NUM_SALIDAS define la cantidad de GPIOs. El salidaGPIOs es una matriz
con los números GPIO que desea controlar.
NOTA: Si cambia la cantidad de GPIOs y los GPIO que desea controlar, también debe
cambiarse los identificadores de los elementos HTML en el documento index.html. */
// Inicializa SPIFFS
void initFS() {
if (!SPIFFS.begin()) {
Serial.println("Un error ha ocurrido mientras se montaba SPIFFS");
}
Serial.println("SPIFFS montado exitosamente");
}
// Inicializa WiFi
void initWiFi() {
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
Serial.print("Conectando a la red WiFi ..");
while (WiFi.status() != WL_CONNECTED) {
Serial.print('.');
delay(1000);
}
Serial.println(WiFi.localIP());
}
/* La función ObtenerEstadosDeSalida () comprueba el estado de todos sus GPIO
y devuelve una variable de cadena JSON con esa información. */
String ObtenerEstadosDeSalida(){
JSONVar miObjetoVectorDePuertosJSON;
for (int i =0; i<NUM_SALIDAS; i++){
miObjetoVectorDePuertosJSON["gpios"][i]["salida"] = String(salidaGPIOs[i]);
miObjetoVectorDePuertosJSON["gpios"][i]["estado"] = String(digitalRead(salidaGPIOs[i]));
}
/* La variable json VectorDePuertos contiene un arreglo de GPIOs
con la siguiente estructura:
gpios[0] = salida = estado = 0
gpios[1] = salida = 5 estado = 0
gpios[2] = salida = 12 estado = 0
gpios[3] = salida = 13 estado = 0
*/
String CadenaJSON = JSON.stringify(miObjetoVectorDePuertosJSON);
/* la función JSON.stringfy convierte el vector en formato
texto JSON */
Serial.print(CadenaJSON); // se envía la cadenaJSON al monitor de arduino
return CadenaJSON;
}
void notificaClientes(String estado) {
/* La función notificaClientes () notifica a todos los clientes con un mensaje
que contiene lo que sea que se pase como argumento. En este caso, queremos
notificar a todos los clientes de el estado actual de todos los GPIO
siempre que haya un cambio. */
ws.textAll(estado);
}
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
AwsFrameInfo *info = (AwsFrameInfo*)arg; //La totalidad del mensaje está en un frame
/*La función handleWebSocketMessage () es una función de devolución
de llamada que se ejecutará siempre que recibamos nuevos datos de los
clientes a través del protocolo WebSocket. Como se explica previamente,
el cliente enviará el mensaje "estados" para solicitar el GPIO actual de
los estados o un mensaje que contiene el número GPIO para cambiar el estado. */
if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
data[len] = 0;
if (strcmp((char*)data, "estados") == 0) {
/* Si no (0) se han recibido los estados, obtenerlos */
notificaClientes(ObtenerEstadosDeSalida());
}
else{
  int gpio = atoi((char*)data); // atoi convierte a entero el texto
digitalWrite(gpio, !digitalRead(gpio));
/* grabamos el número de gpio en la variable gpio
observa que se graba el valor negado ! de la lectura digital
porque es un interruptor deslizable de dos estados */
notificaClientes(ObtenerEstadosDeSalida());
/* Si recibimos el mensaje "estados", enviamos un mensaje a todos los
clientes con el estado de todos los GPIO que utilizan la función notificaClientes.
Llamar a la función ObtenerEstadosDeSalida() devuelve una cadena JSON con
los estados GPIO.
*/
}
}
}
/*
Ahora necesitamos configurar un detector de eventos para manejar los diferentes
pasos asincrónicos del protocolo WebSocket. Este controlador de eventos
se puede implementar definiendo onEvent () de la siguiente manera:
*/
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,AwsEventType TipoDeEvento, void *arg, uint8_t *data, size_t len) {
switch (TipoDeEvento)
{ /* El argumento TipoDeEvento representa el evento que ocurre.
Puede tomar los siguientes valores: */
case WS_EVT_CONNECT: // cuando un cliente ha iniciado sesión
Serial.printf("Cliente WebSocket #%u conectado de %s\n", client->id(), client->remoteIP().toString().c_str());
break;
case WS_EVT_DISCONNECT: // cuando un cliente se desconecta
Serial.printf("Cliente WebSocket #%u desconectado\n", client->id());
break;
case WS_EVT_DATA: // cuando se recibe un paquete de datos del cliente
handleWebSocketMessage(arg, data, len);
break;
case WS_EVT_PONG: // en respuesta a una solicitud de ping
/* Ping es un comando o una herramienta de diagnóstico que permite hacer una
verificación del estado de una determinada conexión o host local.
Es un acrónimo para Packet Internet Groper, lo que literalmente significa
“buscador de paquetes en redes”. Se trata de un comando que permite verificar
el estado de una conexión para determinar si una dirección IP específica
o host es accesible desde la red o no. */
case WS_EVT_ERROR: // cuando se recibe un error del cliente.
break;
}
}
void initWebSocket() {
  /*
Finalmente, la función initWebSocket () inicializa el protocolo WebSocket.
*/
ws.onEvent(onEvent);
server.addHandler(&ws);
}
void setup(){
// Puerto serie para propósitos de depuración
Serial.begin(115200);
// Prepara GPIOs como salidas
for (int i =0; i<NUM_SALIDAS; i++){
pinMode(salidaGPIOs[i], OUTPUT);
}
initFS(); // iniciliza File System
initWiFi(); // inicializa Wi Fi
initWebSocket(); // Inicializa WebSocket
/* Las siguientes líneas manejan lo que sucede cuando recibe una
solicitud en la raíz (/) URL (dirección IP de ESP8266). */
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
request->send(SPIFFS, "/index.html", "text/html",false);
/* Se usa SPIFFS porque es ESP32
El archivo principal en la raíz de SPIFFS es /index.html
Es texto/html
false indica que NO es descarga */
});
server.serveStatic("/", SPIFFS, "/"); /* Cuando el archivo HTML
se cargue en el navegador, solicitará el CSS, archivo JavaScript y favicon.
Estos son archivos estáticos guardados en el mismo directorio
(SPIFFS o SPIFFS). Entonces, podemos simplemente agregar la siguiente
línea para servir archivos en un directorio cuando sea solicitado por
la URL raíz. Servirá los archivos CSS y favicon automáticamente
(también en ocasiones agregamos una imagen) */
server.begin(); // inicia el servidor
}
void loop() {
ws.cleanupClients(); /* Los navegadores a veces no cierran correctamente
la conexión WebSocket, incluso cuando se llama a la función close () en
JavaScript. Esto eventualmente agotará los recursos del servidor web y hará
que el servidor se bloquee. Llamar periódicamente a la función
cleanupClients () desde el bucle principal () limita el número de clientes
cerrando el cliente más antiguo cuando se ha superado el número máximo
de clientes. Esto se puede llamar en cada ciclo, sin embargo, si desea
usar menos energía, entonces es suficiente llamar con tan poca frecuencia
como una vez por segundo. */
}