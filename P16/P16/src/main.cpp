#include <Arduino.h>
#include <WiFi.h>
const char* ssid = "Potros_Wifi";
const char* password = "";
WiFiServer server(80);
String CadenaSolicitud = "";
unsigned int cont = 0;
int LDR = 34; //Se usará esta entrada ADC6 de Wemos ESP 32
String encabezado = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
// Inicia la cadena cruda
String CadenaCruda = R"=====(
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
</style>
<script>
var SolicitudAjax = null;
if (window.XMLHttpRequest) { SolicitudAjax =new XMLHttpRequest(); }
// El objeto XMLHttpRequestEs el que permite la comunicación asíncrona (en segundo plano)
// con el servidor.
else { SolicitudAjax =new ActiveXObject("Microsoft.XMLHTTP"); }
setInterval(actualizaContador, 200);
// Cuando el timer inicia, es llamado actualizaContador
// después de 200 ms (porque el timer tiene un contador descendente)
// actualizaContador llama ajaxLoad()y es cuando se realiza el
// trabajo de desplegar el valor actualizado del contador
function actualizaContador()
// al terminar su conteo descendente
// el timer ejecuta obtenerCuenta
{
ajaxLoad('obtenerCuenta');
}
function ajaxLoad(ajaxURL)
// Aquí está la parte de Ajax:
{
if(!SolicitudAjax){ alert('AJAX is not supported.'); return; }
SolicitudAjax.open('GET',ajaxURL,true);
SolicitudAjax.onreadystatechange = function()
{
if(SolicitudAjax.readyState == 4 && SolicitudAjax.status==200)
{
var ResultadoDeAjax = SolicitudAjax.responseText;
document.getElementById('cont_ParteDePagina').innerHTML = ResultadoDeAjax;
//hace referencia a la parte que se va a actualizar
}
}
SolicitudAjax.send();
}

</script>
<title>Intensidad</title>
</head>
<body>
<div id='main'>
<h2>Servidor Web que detecta el nivel de intensidad luminosa</h2>
<p id='cont_ParteDePagina'>cont = 0</p>
<!-- cont inicia en cero -->
</div>
</div>
</body>
<div id='nombre'>
<p>Sistemas Embebidos P16</p>
<p>Fernando Homero Gonzalez Anguiano 204489</p>
</div>
</html>
)====="; // Se cierra cadena cruda
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void setup()
{
Serial.begin(115200);
Serial.println();
Serial.println("Puerto serie a velocidad 115200");
Serial.println();
// Conectar a la red WiFi
Serial.print("Conectando a "); Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(500);
}
Serial.println("");
Serial.println("CONECTADO A ");
Serial.print("IP ");
Serial.print(WiFi.localIP());

// start a server
server.begin();
Serial.println(" Servidor iniciado");
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void loop()
{
// Verifica si un cliente se ha conectado
WiFiClient cliente = server.available();
if (!cliente) { return; }
// Lee la primera linea de la petición
CadenaSolicitud = cliente.readStringUntil('\r');
// El servidor está constantemente verificando por si hay una
// solicitud, y cuando obtiene una, verifica si la petición
// incluye la cadena "obtenerCuenta"
if ( CadenaSolicitud.indexOf("obtenerCuenta") > 0 ) // si la petición
// contiene la cadena "obtenerCuenta":
{
cont ++; // se incrementa contador
cliente.print( encabezado ); // se despliega en página Web
cliente.print("<html>");
cliente.print( "<h2>Contador de actualizaciones = </h2>" );
cliente.print( "<h2>" );
cliente.print( cont );
cliente.print( "</h2>" );
int lecturaADC = analogRead(LDR);
cliente.print( "<h2> El nivel de intensidad luminosa captado por LDR es:</h2>" );
cliente.print( "<h2>" );
cliente.print( lecturaADC );
cliente.print( "</h2>" );
cliente.print("</html>");
Serial.print("Lectura LDR="); //Se despliega en monitor
Serial.println(lecturaADC);
}
else
// de lo contrario que la petición NO CONTIENE obtenerCuenta
//enviamos la página Web y reseteamos cont a cero
{
cliente.flush();
cliente.print( encabezado );
cliente.print( CadenaCruda );
cont = 0;
}
delay(5);
}