#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
const char* ssid = "Potros_Wifi";
const char* password = "";
// Crea el objeto AsyncWebServer en el puerto 80
AsyncWebServer server(80);
/* La biblioteca ESPAsyncWebServer incluye un complemento WebSocket que facilita el
manejar las conexiones WebSocket. Crear un objeto AsyncWebSocket llamado ws para
manejar las conexiones en la ruta / ws. */
AsyncWebSocket ws("/ws");
const int ledPin = 4; // Conexión para el LED a controlar PWM
const int frec = 5000;
const int ledCanal = 0;
const int resolucion = 12;
String ValorDeslizador = "0";
int cicloUtil; // variable para contolar la brillantez del LED
// Inicializa SPIFFS
void inicializaSPIFFS() {
if (!SPIFFS.begin()) {
Serial.println("Un error ha ocurrido mientras se montaba SPIFFS");
}
Serial.println("SPIFFS montado exitosamente");
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
//***********************************************************************************
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
/* La función handleWebSocketMessage () es una función de devolución de llamada que
se ejecutará siempre que recibamos nuevos datos de los clientes a través del
protocolo WebSocket. */
{
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
data[len] = 0;
ValorDeslizador = (char*)data; // El cliente envía el valor actual del control
// deslizante al servidor. Entonces, guardamos ese valor en la variable ValorDeslizador.
cicloUtil = map(ValorDeslizador.toInt(), 0, 100, 0, 1023);
/* ValorDeslizador puede ser un valor entre 0 y 100, pero el ciclo de trabajo es un valor
entre 0 y 1023. Entonces, usamos la función map () para convertir desde el rango 0-100
al rango de 0 a 1023.
La función map() de Arduino permite transformar un valor entero de un rango de entrada al valor
correspondiente a otro rango de salida.
Los 5 parámetros de entrada son valores enteros:
 valor de entrada
 inicio rango de entrada
 final rango de entrada
 inicio rango de salida
 final rango de salida
Y la función devuelve el valor entero de salida una vez realizado el “mapeo”.
*/
Serial.println(cicloUtil); // Envía el valor del ciclo útil al monitor
/* NOTA:En el monitor se actualiza al valor deslizado por el último cliente
porque está programado en modo Web socket. Para actualizar en la gráfica
se debe actualizar la página Web */
}
}
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void
*arg, uint8_t *data, size_t len) {
/*Ahora necesitamos configurar un detector de eventos para manejar los diferentes pasos
asincrónicos del protocolo WebSocket. Este controlador de eventos se puede implementar
definiendo onEvent () de esta manera: */
// El argumento de tipo representa el evento que ocurre. Puede tomar los siguientes valores:
switch (type) {
case WS_EVT_CONNECT: // cuando un cliente ha iniciado sesión
Serial.printf("WebSocket cliente #%u conectado de %s\n", client->id(), client->remoteIP().toString().c_str());
break;
case WS_EVT_DISCONNECT: // cuando un cliente se desconecta
Serial.printf("WebSocket cliente #%u desconectado\n", client->id());
break;
case WS_EVT_DATA: // cuando se recibe un paquete de datos del cliente
handleWebSocketMessage(arg, data, len);
break;
case WS_EVT_PONG: // En respuesta a una solicitud de ping
/* Ping es un comando o una herramienta de diagnóstico que permite hacer una verificación
del estado de una determinada conexión o host local.
Es un acrónimo para Packet Internet Groper, lo que literalmente significa
“buscador de paquetes en redes”. Se trata de un comando que permite verificar el estado de
una conexión para determinar si una dirección IP específica o host es accesible desde la red o no. */
case WS_EVT_ERROR: // cuando se recibe un error del cliente.
break;
}
}
void inicializaWebSocket() { // Inicializa el protocolo WebSocket
ws.onEvent(onEvent);
server.addHandler(&ws);
}
void setup() {
Serial.begin(115200);
pinMode(ledPin, OUTPUT);
inicializaSPIFFS(); // se inicializa SPIFFS
inicializaWiFi(); // inicializa Wi Fi
// configura LED funcionalidades PWM
ledcSetup(ledCanal, frec, resolucion);
/* El equivalente al analogWrite que se emplea en Arduino para generar un PWM,
en el caso del ESP32 sería la modulación que llaman ledc. */
// Selecciona el canal para el GPIO que será controlado
ledcAttachPin(ledPin, ledCanal);
/* El equivalente al analogWrite que se emplea en Arduino para generar un PWM,
en el caso del ESP32 sería la modulación que llaman ledc. */
inicializaWebSocket(); // inicializa el protocolo Websocket
/* Las siguientes líneas manejan lo que sucede cuando recibe una
solicitud en la raíz (/) URL (dirección IP de ESP32).
Cuando recibe esa solicitud, envía el texto HTML guardado en el archivo
index.html a construir la página web. */
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
request->send(SPIFFS, "/index.html", "text/html");
});
/* Cuando el archivo HTML se cargue en su navegador, solicitará el CSS,
Archivos JavaScript y favicon. Estos son archivos estáticos guardados en
el mismo directorio (SPIFFS o LittleFS). Entonces, podemos simplemente
agregar la siguiente línea para servir archivos en un directorio cuando
sea solicitado por la URL raíz. Servirá los archivos CSS y favicon
automáticamente (también se agrega una imagen en este ejemplo) */
server.serveStatic("/", SPIFFS, "/");
server.on("/ValorActual", HTTP_GET, [](AsyncWebServerRequest *request){
/* Cuando se abre la página web por primera vez, realiza una solicitud en
la ruta /ValorActual para solicitar el ValorDeslizador actual.
ValorActual vincula aquí con script.js */
request->send(200, "/text/plain", String(ValorDeslizador).c_str());
/* Simplemente puede enviar una cadena con esa información
(tercer argumento de la función send ()). */
});
// Inicia el servidor
server.begin();
}
void loop() {
ledcWrite(ledCanal, cicloUtil);
/* El equivalente al analogWrite que se emplea en Arduino para generar un PWM,
en el caso del ESP32 sería la modulación que llaman ledc.
Aunque la funcióm se llama ledc, no significa que únicamente sirva para LEDs.
Puede usarse para cualquier dispositivo, incluidos motores, transitores, etc. */
//El LED se controlará físicamente en el loop()
ws.cleanupClients();
/* Los navegadores a veces no cierran correctamente
la conexión WebSocket, incluso cuando se llama a la función close () en
JavaScript. Esto eventualmente agotará los recursos del servidor web y hará
que el servidor se bloquee. Llamar periódicamente a la función
cleanupClients () desde el bucle principal () limita el número de clientes
cerrando el cliente más antiguo cuando se ha superado el número máximo
de clientes. Esto se puede llamar en cada ciclo, sin embargo, si desea
usar menos energía, entonces es suficiente llamar con tan poca frecuencia
como una vez por segundo. */
}