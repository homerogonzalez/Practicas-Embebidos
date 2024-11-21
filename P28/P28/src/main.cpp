#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
const char* ssid = "Potros_Wifi";
const char* password = "";
#define DHTPIN 5 // pin digital 5 conectado al pin de DATA del sensor DTH 11
// Marcar con comentarios el sensor que no se esté utilizando:
#define TIPO_DHT DHT11 // DHT 11
//#define TIPO_DHT DHT22 // DHT 22 (AM2302)
//#define TIPO_DHT DHT21 // DHT 21 (AM2301)
DHT dht(DHTPIN, TIPO_DHT);
// temperatura y humedad, actualizados en el lazo
float t = 0.0;
float h = 0.0;
// Crear el objeto AsyncWebServer en el puerto 80
AsyncWebServer server(80);
// El tiempo por lo general se debería declarar "unsigned long" para variables que implican tiempo
// El valor llegará a ser muy grande al transcurrir el tiempo si declaramos int puede no alcanzar
unsigned long previosMilisegundos = 0; // almacenará el último tiempo en que DHT fue actualizado
// Actualiza DTH cada 1 segundo
const long intervalo = 1000; // intervalo para el monitor de arduino
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet"
href="https://use.fontawesome.com/releases/v5.7.2/css/all.css"
integrity="sha384-
fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr"
crossorigin="anonymous">
<style>
html {
font-family: Arial;
display: inline-block; /*muestra los elementos de caja en la misma línea y con tamaño

personalizado */

margin: 0px auto; /* centra */
text-align: center;
}
h2 { font-size: 3.0rem; }
p { font-size: 3.0rem; }
.unidades { font-size: 1.2rem; }
.etiquetas-DTH{
font-size: 1.5rem;
vertical-align: middle; /* alinea elementos verticalmente */
padding-bottom: 15px;
}
</style>
</head>
<body>
<h2>Servidor web con sensor DHT </h2>
<p>
<!-- para escoger el color del ícono de temperatura-->
<i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
<!-- span aplica estilo al texto-->
<span class="etiquetas-DTH">Temperatura</span>
<span id=temperatura>%TEMPERATURA%</span>
<!-- unidad de grados como superíndice-->
<sup class="unidades">&deg;C</sup>
</p>
<p>
<!-- para escoger el color del ícono de humedad -->
<i class="fas fa-tint" style="color:#00add6;"></i>
<span class="etiquetas-DTH">Humedad</span>
<span id="humedad">%HUMEDAD%</span>
<sup class="unidades">%</sup>
</p>
</body>
<script>
setInterval(function ( ) {
var solicitudAjax = new XMLHttpRequest();
solicitudAjax.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("temperatura").innerHTML = this.responseText;
}
};
solicitudAjax.open("GET", "/temperatura", true);
solicitudAjax.send();
}, 100 ) ; /* se efectúa la lectura de temp cada 100 ms */
setInterval(function ( ) {
var solicitudAjax = new XMLHttpRequest();
solicitudAjax.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("humedad").innerHTML = this.responseText;
}
};
solicitudAjax.open("GET", "/humedad", true);
solicitudAjax.send();
}, 100 ) ; /* se efectúa la lectura de humedad cada 100 ms */
</script>
</html>)rawliteral";
// La función propia processor reemplaza marcadores de posición
// TEMPERATURA y HUMEDAD con los valores de DTH
String processor(const String& var)
{
if(var == "TEMPERATURA"){
return String(t);
}
else if(var == "HUMEDAD"){
return String(h);
}
return String();
}
// - - - - - - - - - - - - - - - - - - - - - - - -
void setup(){
// Puerto serie para propósitos de grabación depuración
// y comprobación
Serial.begin(115200);
dht.begin();
// Conectar a Wi-Fi
WiFi.begin(ssid, password);
Serial.println("Conectando a la red WiFi");
while (WiFi.status() != WL_CONNECTED) {
delay(1000);
Serial.print(".");
}
// Presenta la IP local de ESP32
Serial.println(WiFi.localIP());
// Ruta para la raíz / de la página web
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
request->send_P(200, "text/html", index_html, processor);
}); // este 200 es de OK
server.on("/temperatura", HTTP_GET, [](AsyncWebServerRequest *request){
request->send_P(200, "text/plain", String(t).c_str());
}); // este 200 es de OK
server.on("/humedad", HTTP_GET, [](AsyncWebServerRequest *request){
request->send_P(200, "text/plain", String(h).c_str());
}); // este 200 es de OK
// Inicia el servidor
server.begin();
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - */
void loop()
{
unsigned long actualesMilisegundos = millis();
if (actualesMilisegundos - previosMilisegundos >= intervalo) {
// Graba los últimos valores detectados por DTH
previosMilisegundos = actualesMilisegundos;
// Lee temperatura en Centígrados (la default)
float nuevaT = dht.readTemperature();
if (isnan(nuevaT)) {
// Comprobamos si las lecturas de temperatura pueden dar algún fallo
// mediante la función isnan()
// Esta función devuelve un 1 en caso de que el valor no sea numérico
Serial.println("Falla al leer el sensor DHT!");
}
else {
t = nuevaT;
Serial.println(t); // Envía valor de temperatura
}
// lee humedad
float nuevaH = dht.readHumidity();
//si la lectura de humedad read falló, , no cambies el valor de h
if (isnan(nuevaH)) {
// Comprobamos si las lecturas de humedad pueden dar algún fallo
// mediante la función isnan()
// Esta función devuelve un 1 en caso de que el valor no sea numérico
Serial.println("Falla al leer el sensor DHT!");
}
else {
h = nuevaH;
Serial.println(h); // Envía valor de humedad
}
}
}