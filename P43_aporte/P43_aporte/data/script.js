// Cuando la página se carga, se obtienen lecturas del sensor
window.addEventListener('load', obtenerLecturas);
/*Cuando se acceda a la página web por primera vez, haremos una
solicitud al servidor para obtener las lecturas actuales del
sensor. De lo contrario, tendríamos que esperar a una nueva
lectura del sensor para desplegar (esto debido a eventos enviados
por el servidor), lo que puede llevar algún tiempo dependiendo de
el intervalo que se estableció en el servidor. Es necesario agregar
un detector de eventos que llame a la función obtenerLecturas
cuando se cargue la página web */
/* El objeto window representa una ventana abierta en un navegador.
El método addEventListener () configura una función que se llamará
cuando un determinado evento sucede. En este caso, llamaremos a la
función obtenerLecturas cuando se cargue la página ('load') para
obtener las lecturas actuales del sensor.
*/
/* Función para obtener las lecturas del sensor
cuando la página se carga por primera vez */
function obtenerLecturas()
{ /* Ya se sabe como hacer solicitudes que utilizan JavaScript.
Creando un nuevo objeto XMLHttpRequest. Luego, envíar una
solicitud GET al servidor en la URL /lecturas usando los
métodos open () y send (). */
var variableAJAX = new XMLHttpRequest();
/* Cuando enviemos esa solicitud, el ESP32 enviará una
respuesta con la información de la solicitud. Por lo tanto,
debemos ser capaces de manejar lo que sucede cuando recibimos
la respuesta.
Usaremos la propiedad onreadstatechange que define una función
que se ejecutará cuando cambia la propiedad readyState.
La propiedad readyState mantiene el estado de XMLHttpRequest.
*/
variableAJAX.onreadystatechange = function()
{
if (this.readyState == 4 && this.status == 200)
{ /* La respuesta de la solicitud está lista cuando el
readyState es 4 y el status es 200.
• readyState = 4 significa que la solicitud finalizó y
la respuesta está lista;
• estado = 200 significa "OK"
*/
/*La respuesta enviada por el ESP8266 de la Wemos D1 es
el siguiente texto en formato JSON:
{"temperatura":"25.66",
"humedad":"30.26",
"presion":"1004.84"}
*/
/* Necesitamos convertir la cadena JSON en un objeto JSON
usando el método parse (). El resultado se guarda en la
variable miObjetoJSON.
*/
var miObjetoJSON = JSON.parse(this.responseText);
/* Entonces, podemos obtener la temperatura con
miObjetoJSON.temperatura, la humedad con miObjetoJSON.humedad
y la presión con miObjetoJSON.presion. */
console.log(miObjetoJSON); //se envían a consola
/*Las siguientes líneas colocan los datos recibidos en los
elementos con los correspondientes ids ("temp", "hum" y "pres")
en la página web. */
document.getElementById("temp").innerHTML = miObjetoJSON.temperatura;
document.getElementById("hum").innerHTML = miObjetoJSON.humedad;
document.getElementById("pres").innerHTML = miObjetoJSON.presion;
document.getElementById("altu").innerHTML = miObjetoJSON.altura;
// Hasta aquí está completa la función de obtenerLecturas()
}
};
variableAJAX.open("GET", "/lecturas", true);
variableAJAX.send(); // se envían
}
/* Ahora, necesitamos manejar los eventos enviados por
el servidor (Server Sent Events SSE).
*/
// Creamos una fuente de eventos para "escuchar" los eventos
if (!!window.EventSource) /* El objeto window representa la ventana que contiene un documento
DOM (Document Object Model)
El DOM es un modelo de documento que se carga en el navegador web y que representa el
documento como un árbol de nodos, en donde cada nodo representa una parte del documento
(puede tratarse de un elemento, una cadena de texto o un comentario).
Permite ejecutar código en el navegador para acceder e interactuar con cualquier nodo del
documento.
Estos nodos pueden crearse, moverse o modificarse.
Pueden añadirse a estos nodos manejadores de eventos (event listeners en inglés) que se
ejecutarán/activarán cuando ocurra el evento indicado en este manejador.
*/
{ /* Creamos un nuevo objeto fuenteDeEventos y especificamos
la URL de la página que envían las actualizaciones.
En nuestro caso, es /eventos.
*/
var fuenteDeEventos = new EventSource('/eventos');
/* Una vez que haya creado una instancia de una fuente
de evento, se puede comenzar a "escuchar" los mensajes
de el servidor con addEventListener ().
*/
/* En el siguiente link se muestran los detectores de
eventos predeterminados, de acuerdo a la
documentación de AsyncWebServer.
https://github.com/me-no-dev/ESPAsyncWebServer#setup-event-source-in-the-browser
*/
fuenteDeEventos.addEventListener('open', function(e)
{
/*El parámetro (e) se pasa automáticamente de javascript a su función cuando agrega un detector de
eventos. Representa el elemento que se vio afectado, un ejemplo sería el elemento de botón en el
que se hizo clic.
*/
console.log("Eventos Conectados"); //envia a consola
}, false);
fuenteDeEventos.addEventListener('error', function(e) {
if (e.target.readyState != EventSource.OPEN) { // Si e.tarjet.readyState NO ES abierto
console.log("Eventos Desconectados"); //envia a consola
}
}, false);
/* Luego, agregamos un detector de eventos ("escuchador")
para el evento 'nuevas_lecturas' */
fuenteDeEventos.addEventListener('nuevas_lecturas', function(e) {
console.log("nuevas_lecturas", e.data);
/* Necesitamos convertir la cadena JSON en una VariableObjeto
JSON usando el método parse ().El resultado se guarda en la
variable VariableObjeto. */
var VariableObjeto = JSON.parse(e.data); /* ya que se convierten
los datos con el método parse () podemos hacer uso de la
variable VariableObjeto. */
/* Ahora se puede obtener:
la temperatura con VariableObjeto.temperatura,
la humedad con VariableObjeto.humedad y
la presión con VariableObjeto.presion.
Las siguientes líneas colocan los datos recibidos en los
elementos con los correspondientes ids ("temp", "hum" y "pres")
en la página web.
*/
document.getElementById("temp").innerHTML = VariableObjeto.temperatura;
document.getElementById("hum").innerHTML = VariableObjeto.humedad;
document.getElementById("pres").innerHTML = VariableObjeto.presion;
document.getElementById("altu").innerHTML = VariableObjeto.altura;
}, false);
/* de esta manera, cuando hay nuevas lecturas disponibles,
el ESP8266 de la Wemos D1 envía un evento ('nuevas_lecturas')
al cliente con una cadena JSON que contiene las lecturas
del sensor.
*/
}