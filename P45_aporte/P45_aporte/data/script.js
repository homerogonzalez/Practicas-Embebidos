// Obtiene las lecturas del sensor cuando carga la página
window.addEventListener('load', obtenerLecturas);
/*Cuando se acceda a la página web por primera vez, haremos una
solicitud al servidor para obtener las lecturas actuales del
sensor. De lo contrario, tendríamos que esperar a una nueva
lectura del sensor para desplegar (esto debido a eventos enviados
por el servidor), lo que puede llevar algún tiempo dependiendo de
el intervalo que se estableció en el servidor. Es necesario agregar
un detector de eventos que llame a la función getReadings
cuando se cargue la página web */
/* El objeto window representa una ventana abierta en un navegador.
El método addEventListener () configura una función que se llamará
cuando un determinado evento sucede. En este caso, llamaremos a la
función obtenerLecturas cuando se cargue la página ('load') para
obtener las lecturas actuales del sensor.
*/
//Función para agregar fecha y hora de la última actualización
function actualizarFechaYHora() {
var FechaActual = new Date();
var FechaYHora = FechaActual.getDate() + "/"
+ (FechaActual.getMonth()+1) + "/"
+ FechaActual.getFullYear() + " hora: "
+ FechaActual.getHours() + ":"
+ FechaActual.getMinutes() + ":"
+ FechaActual.getSeconds();
document.getElementById("tiempo-actualizado").innerHTML = FechaYHora;
console.log(FechaYHora); // envía a consola
}
/* Función para obtener las lecturas del sensor
cuando la página se carga por primeza vez */
function obtenerLecturas()
{ /* Ya hemos visto en prácticas anteriorescomo hacer solicitudes
que utilizan JavaScrip, creando un nuevo objeto XMLHttpRequest.
Luego, enviar una solicitud GET al servidor en la URL /lecturas
usando los métodos open () y send (). */
var variableAJAX = new XMLHttpRequest();
variableAJAX.onreadystatechange = function()
{ /* Cuando enviemos esa solicitud, el ESP32 enviará una
respuesta con la información de la solicitud. Por lo tanto,
debemos ser capaces de manejar lo que sucede cuando recibimos
la respuesta.
Usaremos la propiedad onreadystatechange que define una función
que se ejecutará cuando cambia la propiedad readyState.
La propiedad readyState mantiene el estado de XMLHttpRequest.
*/
if (this.readyState == 4 && this.status == 200)
{ /* La respuesta de la solicitud está lista cuando el
readyState es 4 y el status es 200.
• readyState = 4 significa que la solicitud finalizó y
la respuesta está lista;
• estado = 200 significa "OK"
*/
/*La respuesta enviada por el ESP32 de la Wemos D1 R32 es
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
console.log(miObjetoJSON); // envía a consola
/*Las siguientes líneas colocan los datos recibidos en los
elementos con los correspondientes ids ("temp", "hum" y "pres")
en la página web. */
document.getElementById("temp").innerHTML = miObjetoJSON.temperatura;
document.getElementById("hum").innerHTML = miObjetoJSON.humedad;
document.getElementById("pres").innerHTML = miObjetoJSON.presion;
document.getElementById("alt").innerHTML = miObjetoJSON.altura;
actualizarFechaYHora();
}
};
variableAJAX.open("GET", "/lecturas", true);
variableAJAX.send(); // se envían
}
/* Ahora, necesitamos manejar los eventos enviados por
el servidor (Server Sent Events SSE).
*/
// Creamos una fuente de eventos para "escuchar" los eventos
if (!!window.EventSource)
{ /* Creamos un nuevo objeto EventSource y especificamos
la URL de la página que envían las actualizaciones.
En nuestro caso, es /eventos.
*/
var FuenteDeEventos = new EventSource('/eventos');
/* Una vez que haya creado una instancia de una fuente
de evento, se puede comenzar a "escuchar" los mensajes
de el servidor con addEventListener ().
*/
/* En el siguiente link se muestran los detectores de
eventos predeterminados, de acuerdo a la
documentación de AsyncWebServer.
https://github.com/me-no-dev/ESPAsyncWebServer#setup-event-source-in-the-browser
*/
FuenteDeEventos.addEventListener('open', function(e)
{ // abierto para "escuchar" la ocurrencia de eventos
/* Objetos Event:
Cada función que responde a un evento recibe un objeto que contiene información acerca del
evento. Aunque algunos eventos tiene sus propios objetos, existe un objeto llamado Event que es
común a cada evento. Las siguientes son algunas de sus propiedades y métodos
El objeto Event es enviado a la función como un argumento, y por lo tanto tenemos que declarar un
parámetro que recibirá este valor. El nombre del parámetro es irrelevante, pero es usualmente
definido como e o evento. */
console.log("Eventos Conectados"); //envia a consola
}, false);
FuenteDeEventos.addEventListener('error', function(e)
{ // detecta si hay error
// abierto para "escuchar" la ocurrencia de eventos
if (e.target.readyState != EventSource.OPEN)
{
console.log("Eventos Desconectados"); //envia a consola
}
}, false); // false por default en Chrome
/* Luego, agregamos un detector de eventos ("escuchador")
para el evento 'nuevas_lecturas' */
FuenteDeEventos.addEventListener('nuevas_lecturas', function(e) {
console.log("nuevas_lecturas", e.data); // envía a consola
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
document.getElementById("alt").innerHTML = VariableObjeto.altura;
actualizarFechaYHora();
}, false); // el valor predeterminado es falso para Chrome.
/* de esta manera, cuando hay nuevas lecturas disponibles,
el ESP32 de la Wemos D1 envía un evento ('nuevas_lecturas')
al cliente con una cadena JSON que contiene las lecturas
del sensor.
*/
}
    s