#include <Arduino.h>
// Una vez que se grabe en Wemos es posible que sea necesario pulsar botón RESET
#include <WiFi.h> // Para manejar placa ESP32
const char* ssid = "Potros_Wifi";
const char* password = "";
WiFiServer server(80);
#define LED2 2 // LED en terminal 2
String estado = "";
void setup() {
Serial.begin(115200);
pinMode(LED2, OUTPUT);
// Conecta a la red wifi.
Serial.println();
Serial.print("Conectando con ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("Conectado con WiFi.");
// Inicio del Servidor web.
server.begin();
Serial.println("Servidor web iniciado.");
// Esta es la IP
Serial.print("Esta es la IP para conectar: ");
Serial.print("http://");
Serial.println(WiFi.localIP());
}
void loop() {
// Consulta si se ha conectado algún cliente.
WiFiClient cliente = server.available();
if (!cliente) {
return;
}
Serial.print("Nuevo cliente: ");
Serial.println(cliente.remoteIP());
/////////////////////////////////////////////////////
// Lee la información enviada por el cliente.
String peticion = cliente.readStringUntil('\r');
Serial.println(peticion);
// Realiza la petición del cliente.
if (peticion.indexOf("on2") != -1) {digitalWrite(LED2, HIGH); estado = "Encendido";}
if (peticion.indexOf("off2") != -1){digitalWrite(LED2, LOW); estado = "Apagado";}
cliente.println("HTTP/1.1 200 OK");
cliente.println("Content-Type: text/html");
cliente.println(""); // Importante.
cliente.println("<!DOCTYPE HTML>");
cliente.println("<html>");
cliente.println("<head><meta charset=utf-8></head>");
cliente.println("<body><center><font face='Arial'>");
cliente.println("<h1>Servidor web con ESP32.</h1>");
cliente.println("<h2><font color='#009900'>No responsivo</font></h2>");
cliente.println("<br><br>");
cliente.println("<a href='on2'><button>Enciende LED2</button></a>");
cliente.println("<br><br>");
cliente.println("<a href='off2'><button>Apaga LED2</button></a>");
cliente.println("<br><br>");
cliente.println(estado);
cliente.println("</font></center></body></html>");
Serial.print("Cliente desconectado: ");
Serial.println(cliente.remoteIP());
cliente.flush();
cliente.stop();
}