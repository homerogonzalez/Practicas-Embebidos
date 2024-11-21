#include <Arduino.h>
#include <WiFi.h>
const char* ssid = "Megacable_4caZKTR";
const char* password = "Obregon3109.";
int pinEntrada34 = 34; // Se utilizará la entrada IO34 de la Wemos D1 R32
WiFiServer server(80);
String temporalString = "";
unsigned int cont = 0;
String protocolo = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
// String html_Cadena2=R"=====(
// <style>
// .contenedor { /* vincula con “contenedor” en html */
// background-color: rgb(192, 192, 192);
// width: 80%;
// border-radius: 15px;
// }
// .propiedades { /* vincula con “propiedades” en html */
// background-color: rgb(116, 194, 92);
// color: white;
// padding: 1%;
// text-align: right;
// font-size: 20px;
// border-radius: 15px;
// }
// .porcentaje { /* vincula con “porcentaje” en html */
// width:%x%%;
// }
// </style>)=====";
// String html_Cadena3 = R"=====(
// <div class="contenedor">
// <div class="propiedades porcentaje">%x%%</div> <!– porcentaje remplazado ––>
// </div>)=====";
String html_CadenaCruda = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name='viewport' content='width=device-width, initial-scale=1.0'/>
<meta charset='utf-8'>
<style>
body {font-size:100%; font-style: italic;background-color: black;color: #42C920;}
#main {display: table; margin: auto; padding: 0 10px 0 10px; }
h2 {text-align:center; }
#nombre {text-align:right; }
.contenedor { /* vincula con “contenedor” en html */
background-color: rgb(192, 192, 192);
width: 80%;
border-radius: 15px;
}
.propiedades { /* vincula con “propiedades” en html */
background-color: rgb(116, 194, 92);
color: white;
padding: 1%;
text-align: right;
font-size: 20px;
border-radius: 15px;
}
.porcentaje { /* vincula con “porcentaje” en html */
width:%x%%;
}
</style>
<script>
function refresca(PeriodoDeRefrescamiento)
{
setTimeout("location.reload(true);", PeriodoDeRefrescamiento); //se ejecuta location.reload después
// de PeriodoDeRefrescamiento
}
window.onload = refresca(100); // el argument de refresca se guardará como
// PeriodoDeRefrescamiento
</script>
<title>ADC con barra</title>
</head>
<body>
<div id='main'>
<h2>Servidor Web de conversión analógica a digital en HTML</h2>
<div id='cont'>
<p id='cont_ParteDePagina'>Contador de actualizaciones = 0 </p>
</div>
</div>
<div class="contenedor">
<div class="propiedades porcentaje">%x%%</div> <!– porcentaje remplazado ––>
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
// Verifica si un cliente se ha conectado
WiFiClient cliente = server.available();
if (!cliente) { return; }
// Lee la primera linea de la petición
temporalString = cliente.readStringUntil('\r');
// El servidor está constantemente verificando por si hay una
// solicitud, y cuando obtiene una, verifica si la petición
// incluye la cadena "obtenerCuenta"
if ( temporalString.indexOf("obtenerCuenta") > 0 ) // si la petición
// contiene la cadena "obtenerCuenta":
{
cont ++;
temporalString = html_CadenaCruda;
temporalString.replace("%cont%", String(cont) );
cliente.print( protocolo );
cliente.print( temporalString );
cliente.print(" ");
int c = analogRead(pinEntrada34); // valor análogo
int porc=c*100/4095; // porcentaje del valor análogo, ADC de 12 bits 0 a 4095
// cliente.print("<html>");
// temporalString=html_Cadena2;
temporalString.replace("%x%", String(porc) ); // reemplaza porcentaje en html_Cadena2
// cliente.print(temporalString);
cliente.print("<h2> Valor de la señal análoga </h2>");
cliente.print("<h2>");
cliente.print( c );
cliente.print("</h2>");
// temporalString=html_Cadena3;
temporalString.replace("%x%", String(porc) ); // reemplaza porcentaje en html_Cadena3
// cliente.print(temporalString);
// cliente.print("</html>");
cliente.print(" ");
Serial.print("contador = "); Serial.println(cont);
}
else
// de lo contrario que la petición NO CONTIENE obtenerCuenta
//enviamos la página Web y reseteamos cont a cero
{
cliente.flush();
cliente.print( protocolo );
cliente.print( temporalString );
cont = 0;
}
delay(5);
}