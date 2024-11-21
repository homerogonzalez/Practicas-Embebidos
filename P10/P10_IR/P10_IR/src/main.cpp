#include <Arduino.h>
#include <WiFi.h>
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
// Aquí está la parte de Ajax:
var SolicitudAjax = null;
if (window.XMLHttpRequest) { SolicitudAjax =new XMLHttpRequest(); } /* PASO 1: Obtener la
instancia del objeto XMLHttpRequest */
/* El objeto XMLHttpRequest Es el que permite la comunicación asíncrona (en segundo plano)
con el servidor.
Asíncrona significa que pueden ocurrir unas cosas independientemente
del flujo del programa principal The NodeJS handbook */
else { SolicitudAjax =new ActiveXObject("Microsoft.XMLHTTP"); }
setInterval(actualizaContador, 200); /* La función setInterval y el método setInterval() Para utilizar la
función setInterval, se invoca el método setInterval() y se le pasa la función de callback y el retardo
como parámetros. Aquí está un ejemplo: setInterval(funcionCallback, retardo);
En este ejemplo, funcionCallback es la función que se ejecutará repetidamente después del retardo
especificado. Retardo es el tiempo en milisegundos que debe transcurrir antes de ejecutar la función
de callback. Por ejemplo, si deseas que una función llamada actualizarDatos() se ejecute cada 5
segundos, se puede usar el siguiente código:
setInterval(actualizarDatos, 5000); */
/* * * * * * * * * * * * * * * * ** * * * * * * * * * * * * * * * * * * * * * * * * * * */
function actualizaContador()
// al terminar su conteo descendente
// el timer ejecuta obtenerCuenta
{
ajaxLoad('obtenerCuenta');
}
/* * * * * * * * * * * * * * * * ** * * * * * * * * * * * * * * * * * * * * * * * * * * */
function ajaxLoad(ajaxURL)
{
if(!SolicitudAjax) { alert('AJAX is not supported.'); return; }
SolicitudAjax.open('GET',ajaxURL,true); // PASO 2: Realiza la petición HTTP
SolicitudAjax.onreadystatechange = function() // PASO 3: Prepara la función de respuesta
{
if(SolicitudAjax.readyState == 4 && SolicitudAjax.status==200) //PASO 4: Ejecuta la respuesta
{
var ResultadoDeAjax = SolicitudAjax.responseText;
document.getElementById('cont_ParteDePagina').innerHTML = ResultadoDeAjax;
//hace referencia a la parte que se va a actualizar
}
}
SolicitudAjax.send(); // PASO 5: Se envía la respuesta a la solicitud
}
</script>
<title>Auto refresh parcial con AJAX</title>
</head>
<body>
<div id='main'>
<h2>Ejemplo autoactualizable usando AJAX (Asynchronous JavaScript And XML)</h2>
<!--establece un elemento id indicando que es el contador
Como parte de la página Web a autoactualizar: -->
<p id='cont_ParteDePagina'>cont = 0</p>
<!-- cont inicia en cero -->
</div>
</div>
</body>
</html>
)====="; // Se cierra cadena cruda
const char* ssid = "Potros_Wifi";
const char* password = "";
WiFiServer server(80);
String CadenaSolicitud = "";
unsigned int cont = 0;
/* variables de este ejemplo*/
int pinEntradaD5 = 5; // Se utilizará la entrada D5 de la Wemos D1 R32
int valorEntradaD5 = 0;
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void setup()
{
pinMode(valorEntradaD5, INPUT);
Serial.begin(115200);
Serial.println();
Serial.println("Puerto serie a velocidad 115200");
Serial.println();
// Conectar a la red WiFi
Serial.print("Conectando a la red Wi Fi "); Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(500);
}
Serial.println("");
Serial.print("Conectado a la IP: ") ;
Serial.print(WiFi.localIP());
Serial.print(" ");
// iniciar el servidor
server.begin();
Serial.println("Servidor iniciado");
} // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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
cliente.print( header ); // se despliega en página Web
cliente.print("<html>");
cliente.print( "<h2> contador </h2>" );
cliente.print( "<h2>" );
cliente.print( cont );
cliente.print( "</h2>" );
valorEntradaD5 = digitalRead(pinEntradaD5);
cliente.print ("<h1> Servidor Web autoactualizable detector de objetos opacos </h2>");
cliente.print("</h1>");
cliente.print("<html>");
if (valorEntradaD5 == 1)
cliente.print("<h1> No hay objeto opaco </h1>");
else
cliente.print("<h1> Se ha detectado un objeto opaco </h1>");
cliente.print("</html>");
Serial.print("cont="); //Se despliega en monitor
Serial.println(cont);
}
else
// de lo contrario que la petición NO CONTIENE obtenerCuenta enviamos la página Web y
//reseteamos cont a cero
{
cliente.flush();
cliente.print( header );
cliente.print( CadenaCruda );
cont = 0;
}
delay(5);
}