#include <Arduino.h>
#include <WiFi.h>
const char* ssid = "Potros_Wifi";
const char* password = "";
// bool horario = false;
// bool antihor = false;
int grado = 90;
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
String solicitud = cliente.readStringUntil('\r');
Serial.println(solicitud);
// Página WEB
cliente.println("HTTP/1.1 200 OK");
cliente.println("Content-Type: text/html");
cliente.println("");
cliente.println("<!DOCTYPE HTML>");
cliente.println("<html>");
cliente.println("<head><meta name='viewport' content='width=device-width, initial-scale=1.0'/><meta charset=utf-8></head>");
cliente.println("<body style='font-size:100%; font-style: italic;background-color: black;color: #42C920;'><center><font face='Arial'>");
// inicia JavaScript
cliente.println("<script>");
cliente.println("function ActualizaRango(val) {");
cliente.println("window.location.href = '/deslizar/'+val;");
cliente.println("}");
cliente.println("</script>");
// Termina JavaScript
cliente.print("<div style='text-align:center;'><p>Sistemas Embebidos P25</p><p>Fernando Homero Gonzalez Anguiano 204489</p></div>");
cliente.println("<h1>Control de motor de CD con scrollbar</h1>");
cliente.println("<h2><font color='#009900'>Desliza la barra</font></h2>");
cliente.println("<h3>Sistemas embebidos</h3>");
cliente.println("<div style='text-align:center;'>");
if(solicitud.indexOf("/deslizar/")!=-1)
{
String valor=solicitud.substring(14,17);
grado=valor.toInt();
String x;
x="<input style='width:50%;' type='range' min='0' max='180' step='90' value=%val% onchange='ActualizaRango(this.value);'/>";
x.replace("%val%", String(grado) );
cliente.println(x);
}
else
{
cliente.println("<input style='width:50%;' type='range' min='0' max='180' step='10' onchange='ActualizaRango(this.value);'/>");
}
cliente.println("</br>");
cliente.println("</div>");
cliente.println("<br><br>");
cliente.println("<br>");
if (grado == 0){
  tipo="antihorario"; 
  digitalWrite(hora,LOW);
  digitalWrite(anti,HIGH);
}
else if (grado == 180){
  tipo="horario"; 
  digitalWrite(hora,HIGH);
  digitalWrite(anti,LOW);
}
else if (grado == 90){
  tipo="stop"; 
  digitalWrite(hora,LOW);
  digitalWrite(anti,LOW);
}
// servo1.write(grado);
cliente.println(" <h2>Sentido del giro</h2>");
cliente.println(String(tipo));
cliente.println("<h2>");
cliente.println("</h2>");
cliente.println("</font></center></body></html>");
Serial.print("Cliente desconectado: ");
cliente.flush();
cliente.stop();
}