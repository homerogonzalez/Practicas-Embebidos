#include <Arduino.h>

#include <WiFi.h>
const char* ssid = "Potros_Wifi";
const char* password = "";
WiFiServer server(80);
String temporalString = "";
unsigned int cont = 0;
String protocolo = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
String html_CadenaCruda = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name='viewport' content='width=device-width, initial-scale=1.0'/>
<meta charset='utf-8'>
<style>
body {font-size:100%;}
#main {display: table; margin: auto; padding: 0 10px 0 10px; }
h2 {text-align:center; }
p { text-align:center; }
</style>
<script>
function refresca(PeriodoDeRefrescamiento)
{
setTimeout("location.reload(true);", PeriodoDeRefrescamiento);
}
window.onload = refresca(100);
</script>
<title>Intensidad</title>
</head>
<body>
<div id='main'>
<h2>Servidor Web que detecta el nivel de intensidad luminosa</h2>
<div id='cont'>
<p>Contador de actualizaciones = %cont%</p>
</div>
</div>
</body>
</html>
)=====";
void setup()
{
Serial.begin(115200);
Serial.println();
Serial.println("El puerto serie iniciado a 115200");
Serial.println();
// Conectarse a la red Wi Fi
Serial.print("Conectando a "); Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(500);
}
Serial.println("");
Serial.println("CONECTADO ");
Serial.print("IP ");
Serial.print(WiFi.localIP());
// iniciar un servidor
server.begin();
Serial.println(" Servidor iniciado");
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void loop()
{
// Verificar si un cliente se ha conectado
WiFiClient cliente = server.available();
if (!cliente) { return; }
cont ++;
temporalString = html_CadenaCruda;
temporalString.replace("%cont%", String(cont) );
cliente.flush();
cliente.print( protocolo );
cliente.print( temporalString );
cliente.print(" ");
int intensidadLuminosa = analogRead(34); // en C es posible declarar variable aquí
cliente.print("<html>");
cliente.print("<h2> El nivel de intensidad luminosa captado por LDR es: </h2>");
cliente.print("<h2>");
cliente.print( intensidadLuminosa );
cliente.print("</h2>");
cliente.print("</html>");
cliente.print(" ");
Serial.print("contador = "); Serial.println(cont);
delay(5);
// pequeño retardo
}
