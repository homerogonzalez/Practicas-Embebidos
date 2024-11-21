#include <Arduino.h>

String encabezado = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
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

<title>Lectura de push button HTML-Javascript</title>
<script>
function refresca(PeriodoDeRefrescamiento)
{
setTimeout("location.reload(true);", PeriodoDeRefrescamiento);
}
window.onload = refresca(100);
</script>
</head>
<body>
<div id='main'>
<h2>Servidor Web autoactualizable con recarga rápida leyendo push button en HTML</h2>
<div id='cont'>
<p>Contador de actualizaciones = %cont%</p>
</div>
</div>
</body>
</html>
)=====";
#include <WiFi.h>
const char* ssid = "Potros_Wifi";
const char* password = "";
int pinEntrada5 = 5; // Se utilizará la entrada D5 de la Wemos D1
int valorEntrada5 = 0;
WiFiServer server(80);
String temporalString = "";
unsigned int cont = 0;
void setup()
{
pinMode(pinEntrada5, INPUT); // se prepara como entrada al pin 4
Serial.begin(115200);
Serial.println();
Serial.println("El puerto serie iniciado a 115200");
Serial.println();
// Conectarse a la red Wi Fi
Serial.print(F("Conectando a ")); Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(500);
}
Serial.println("");
Serial.println("CONECTADO");
Serial.print(" IP ");
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
cliente.print( encabezado );
cliente.print( temporalString );
valorEntrada5 = digitalRead(pinEntrada5); //lectura digital de pin
cliente.print(" ");
cliente.print("El valor digital de entrada es: ");
cliente.print(valorEntrada5);
Serial.print("contador = "); Serial.println(cont);
delay(5);
cliente.flush();
cliente.stop();
// El cliente será desconectado cuando la función retorna y el objeto 'cliente' es destruido
}