#include <Arduino.h>

String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
// Inicia la cadena cruda
String CadenaCruda = R"=====(
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
function actualizaContador()
// al terminar su conteo descendente
// el timer ejecuta obtenerCuenta
{
ajaxLoad('obtenerCuenta');
}
var SolicitudAjax = null;
if (window.XMLHttpRequest) { SolicitudAjax =new XMLHttpRequest(); }
// El objeto XMLHttpRequestEs el que permite la comunicación asíncrona (en segundo plano)
// con el servidor.
else { SolicitudAjax =new ActiveXObject("Microsoft.XMLHTTP"); }
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
setInterval(actualizaContador, 200);
// Cuando el timer inicia, es llamado actualizaContador
// después de 200 ms (porque el timer tiene un contador descendente)
// actualizaContador llama ajaxLoad()y es cuando se realiza el
// trabajo de desplegar el valor actualizado del contador
</script>
<title>ADC Auto refresh parcial AJAX</title>
</head>
<body>
<div id='main'>
<h2>Lectura analógica autoactualizable usando AJAX (Asynchronous JavaScript And XML)</h2>
<!--establece un elemento id indicando que es el contador
dividido de la página -->
<p id='cont_ParteDePagina'>cont = 0</p>
<!-- cont inicia en cero -->
</div>
</div>
</body>
</html>
)====="; // Se cierra cadena cruda
#include <WiFi.h>

const char* ssid = "Potros_Wifi";
const char* password = "";

WiFiServer server(80);
String request = "";
unsigned int cont = 0;
int pinEntradaD5 = 5; // Entrada /D5 de la Wemos D1 R32
int valorEntradaD5 = 0;
void setup()
{
Serial.begin(115200);
Serial.println();
Serial.println("Puerto serie a velocidad 115200");
Serial.println();
// Conectar a la red WiFi
Serial.print(F("Conectando a ")); Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(500);
}
Serial.println("");
Serial.println("CONECTADO ");
Serial.print("A IP: ");
Serial.print(WiFi.localIP());
Serial.println(" ");
// iniciar el servidor
server.begin();
Serial.println("Servidor iniciado");
}
// - - - - - - - - - - - - - - - - - - - - - - - -
void loop()
{
// Verifica si un cliente se ha conectado
WiFiClient cliente = server.available();
if (!cliente) { return; }
// Lee la primera linea de la petición
request = cliente.readStringUntil('\r');
// El servidor está constantemente verificando por si hay una
// solicitud, y cuando obtiene una, verifica si la petición
// incluye la cadena "obtenerCuenta"
if ( request.indexOf("obtenerCuenta") > 0 ) // si la petición
// contiene la cadena "obtenerCuenta":
{
  cont ++; // se incrementa contador
cliente.print( header ); // se despliega en página Web
cliente.print("<html>");
cliente.print( "<h2> contador </h2>" );
cliente.print( "<h2>" );
cliente.print( cont );
cliente.print( "</h2>" );
int lecturaADC = analogRead(34); //Se usa IO34 es entrada ADC6 de 12 bits
cliente.print( "<h2> Lectura analógica a digital </h2>" );
cliente.print( "<h2>" );
cliente.print( lecturaADC );
cliente.print( "</h2>" );
valorEntradaD5 = digitalRead(pinEntradaD5); // lectura digital en pin D5
cliente.print( "<h2> El valor digital de entrada es: </h2>" );
cliente.print( "<h2>" );
cliente.print( valorEntradaD5 );
cliente.print( "</h2>" );
cliente.print("</html>");
Serial.print("cont="); //Se despliega en monitor
Serial.println(cont);
Serial.print("Lectura ADC="); //Se despliega en monitor
Serial.println(lecturaADC);
Serial.print("Lectura digital en D5= "); //Se despliega en monitor
Serial.println(valorEntradaD5);
}
else
// de lo contrario que la petición NO CONTIENE obtenerCuenta
//enviamos la página Web y reseteamos cont a cero
{
cliente.flush();
cliente.print( header );
cliente.print( CadenaCruda );
cont = 0;
}
delay(5);
}