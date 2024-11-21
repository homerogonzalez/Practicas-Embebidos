#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Servo.h>
Servo servo1; // se crea un objeto servo1
int grado = 90; // se establece un grado inicial
const char* ssid = "Potros_Wifi";
const char* password = "";
WiFiServer server(80);
void setup() {
Serial.begin(115200);   
delay(10);
servo1.attach(5); // Conectar línea de señal servo a /D5 de Wemos D1
// Conectar a la red Wi fi
Serial.println();
Serial.println();
Serial.print("Conectando a ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi conectada");
// Iniciar el servidor:
server.begin();
Serial.println("Servidor iniciado ");
// Presenta la dirección IP
Serial.print("Use esta URL : ");
Serial.print("http:// ");
Serial.print(WiFi.localIP());
Serial.println("/");
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void loop() {
// Consulta si se ha conectado algún cliente.
WiFiClient cliente = server.available();
if (!cliente) { // al NO haber cliente, espera
return;
}
Serial.print("Nuevo cliente: ");
// Espera hasta que el cliente envíe datos.
while(!cliente.available()){ delay(1); }
// Lee la información enviada por el cliente.
String solicitudCliente = cliente.readStringUntil('\r');
Serial.println(solicitudCliente);
// Realiza la petición del cliente.
if (solicitudCliente.indexOf("aumenta") != -1) {grado = grado + 5; servo1.write(grado);}
if (solicitudCliente.indexOf("disminuye") != -1){grado = grado - 5; servo1.write(grado);}
// Página WEB
cliente.println("HTTP/1.1 200 OK");
cliente.println("Content-Type: text/html");
cliente.println("");
cliente.println("<!DOCTYPE HTML>");
cliente.println("<html>");
cliente.println("<head><meta charset=utf-8></head>");
cliente.println("<body><center><font face='Arial'>");
cliente.println("<h1>Control de servomotor 0 a 180°</h1>");
cliente.println("<h2><font color='#009900'>Manejo de servomotores</font></h2>");
cliente.println("<h3>Servidor web</h3>");
cliente.println("<br><br>");
cliente.println("<a href='aumenta'><button>Pulsa para aumentar 5 grados.</button></a>");
cliente.println("<a href='disminuye'><button>Pulsa para disminuir 5 grados.</button></a>");
cliente.println("<br>");
cliente.println(" <h2>Grado de giro</h2>");
cliente.println("<h2>");
cliente.println(grado);
cliente.println("</h2>");
cliente.println("</font></center></body></html>");
Serial.print("Cliente desconectado: ");
cliente.flush();
cliente.stop();
}