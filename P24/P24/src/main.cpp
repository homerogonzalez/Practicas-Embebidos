#include <Arduino.h>
#include <WiFi.h>
const char* ssid = "Note20";
const char* password = "homero123";
// bool horario = false;
// bool antihor = false;
int hora = 5;
int anti = 13; 
String tipo = ""; 
WiFiServer server(80);
void setup() {
Serial.begin(115200);   
delay(10);
pinMode(anti, OUTPUT);
pinMode(hora, OUTPUT);
// Conectar línea de señal servo a /D5 de Wemos D1
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
if (solicitudCliente.indexOf("horario") != -1) {
  tipo="horario"; 
  digitalWrite(hora,HIGH);
  digitalWrite(anti,LOW);
  }

if (solicitudCliente.indexOf("stop") != -1) {
  tipo="stop"; 
  digitalWrite(hora,LOW);
  digitalWrite(anti,LOW);
  }

if (solicitudCliente.indexOf("antihor") != -1){
  tipo="antihorario"; 
  digitalWrite(hora,LOW);
  digitalWrite(anti,HIGH);
}
// Página WEB
cliente.println("HTTP/1.1 200 OK");
cliente.println("Content-Type: text/html");
cliente.println("");
cliente.println("<!DOCTYPE HTML>");
cliente.println("<html>");
cliente.println("<head><meta name='viewport' content='width=device-width, initial-scale=1.0'/><meta charset=utf-8></head>");
cliente.println("<body style='font-size:100%; font-style: italic;background-color: black;color: #42C920;'><center><font face='Arial'>");
cliente.println("<h1>Control de moto CD</h1>");
cliente.println("<h2><font color='#009900'>Manejo de servomotores</font></h2>");
cliente.println("<h3>Servidor web</h3>");
cliente.println("<br><br>");
cliente.println("<a href='antihor'><button style = 'font-style: italic;background-color: black;color: #42C920;border-style: solid;border-color:#42C920;'>Giro en sentido antihorario</button></a>");
cliente.println("<a href='stop'><button style = 'font-style: italic;background-color: black;color: #42C920;border-style: solid;border-color:#42C920;'>Stop</button></a>");
cliente.println("<a href='horario'><button style = 'font-style: italic;background-color: black;color: #42C920;border-style: solid;border-color:#42C920;'>Giro en sentido horario</button></a>");
cliente.println("<br>");
cliente.println("<h2>Sentido de giro</h2>");
cliente.println("<h2>");
cliente.println(String(tipo));
cliente.println("</h2>");
cliente.print("<div style='text-align:right;'><p>Sistemas Embebidos P24</p><p>Fernando Homero Gonzalez Anguiano 204489</p></div>");
cliente.println("</font></center></body></html>");
Serial.print("Cliente desconectado: ");
cliente.flush();
cliente.stop();
}