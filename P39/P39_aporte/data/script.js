// La puerta de enlace es el punto de entrada a la interfaz de WebSocket.
var PuertaDeEnlace = `ws://${window.location.hostname}/ws`;
/*
window.location.hostname obtiene la dirección de la página actual (la
dirección IP del servidor web). */
var miWebSocket; // Crea una nueva variable global llamada miWebSsocket
function onLoad(event)
{
/* Agrega un detector de eventos que llamará la
función onload cuando se cargue la página web. */
initWebSocket(); // inicializa WebSocket
}
function initWebSocket()
{ /* La función initWebSocket () inicializa una conexión WebSocket
en la puerta de enlace definido anteriormente PuertaDeEnlace.
También asignamos varias funciones de devolución de llamada
que se activarán cuando la conexión WebSocket se abra, se cierre
o cuando se recibe un mensaje.
*/
console.log('Intentando abrir una conexión WebSocket...');
miWebSocket = new WebSocket(PuertaDeEnlace);
miWebSocket.onopen = onOpen;
miWebSocket.onclose = onClose;
miWebSocket.onmessage = onMessage;
}
function onOpen(event)
{
console.log('Conexión abierta'); /* Cuando se abre la conexión, presenta
un mensaje en la consola para propósitos de depuración y envía un mensaje
que diga "estados", para que el servidor sepa que necesita enviar los
estados GPIO actuales. */
miWebSocket.send("estados");
}
function onClose(event) { /* Si por alguna razón la conexión del
WebSocket está cerrada, llama a initWebSocket () volverá a funcionar
después de 2000 milisegundos (2 segundos). */
console.log('Conexión cerrada');
setTimeout(initWebSocket, 2000);
}
function onMessage(event)
{ /* Finalmente, necesitamos manejar lo que sucede cuando el cliente
recibe un nuevo mensaje (evento onMessage). El servidor (Wemos D1 R1)
enviará una variable JSON con los estados GPIO actuales en el siguiente
formato:
{
"gpios":[
{
"salida":"3",
"estado":"0"
},
{
"salida":"4",
"estado":"0"
},
{
"salida":"12",
"estado":"0"
},
{
"salida":"13",
"estado":"0"
}
]
}
Esto puede observarse en la consola si al momento de estar funcionando el servidor
tecleamos Ctrol-Shift-J
*/
var miObjetoJSON = JSON.parse(event.data);
/* Puede obtener la respuesta del servidor como una cadena de JavaScript
utilizando la propiedad event.data (porque recibe los datos de un evento).
La respuesta viene en formato JSON, entonces,podemos guardar la respuesta
como un objeto JSON usando el método JSON.parse () como este:
*/
console.log(miObjetoJSON); /* Para fines de depuración, puede mostrar
el valor de la variable JSON miObjetoJSON en la consola usando
console.log ().
*/
for (i in miObjetoJSON.gpios){ /*
Ahora, necesitamos un bucle for para pasar por todas las salidas
y los estados correspondientes.
*/
var salida = miObjetoJSON.gpios[i].salida;
var estado = miObjetoJSON.gpios[i].estado;
console.log(salida); // se presenta salida en consola
console.log(estado); // se presenta estado en consola
/* analizando las vueltas del ciclo for:
En vuelta 0:
• miObjetoJSON.gpios [0].output devuelve 4
• miObjetoJSON.gpios [0].estado devuelve 0 (al inicio)
El bucle for pasa por todos los objetos dentro de la matriz gpios,
obtiene el GPIO y los estados correspondientes y los guarda en la
salida y el estado de variables JavaScript
En vuelta 1:
• miObjetoJSON.gpios [1].output devuelve 5
• miObjetoJSON.gpios [1].estado devuelve 0 (al inicio)
En vuelta 2:
• miObjetoJSON.gpios [2].output devuelve 12
• miObjetoJSON.gpios [2].estado devuelve 0 (al inicio)
En vuelta 3:
• miObjetoJSON.gpios [3].output devuelve 13
• miObjetoJSON.gpios [3].estado devuelve 0 (al inicio)
Los estados van cambiando de valor a 1 o regresan a 0
a medida que se mueven los deslizadores en la interface
gráfica de control
*/
if (estado == "1"){ /* Se evalúa para id = "3", "4", "12", "13"
y actualiza el estado correspondiente a ENCENDIDO o APAGADO y se
establezca si el control deslizante queda en una posición
activada o no */
document.getElementById(salida).checked = true;
// Al "checkar" resultó 1 ?
document.getElementById(salida+"s").innerHTML = "ENCENDIDO";
/* También necesitamos actualizar el texto del estado a ENCENDIDO.
Obtener el elemento con id = "4s", "5s", "12s", "13s" (cada salida + "s")
y actualizar el texto a ENCENDIDO. */
}
else{
document.getElementById(salida).checked = false;
// Al "checkar" resultó 0
document.getElementById(salida+"s").innerHTML = "APAGADO";
/* También necesitamos actualizar el texto del estado a APAGADO.
Obtener el elemento con id = "4s", "5s", "12s", "13s" (cada salida + "s")
y actualizar el texto a APAGADO. */
}
}
console.log(event.data); //visualizar con la consola
}
// Envía peticiones para controlar GPIOs
function toggleCheckbox (element)
{ /* La función toggleCheckBox () envía un mensaje usando la conexión
WebSocket cada vez que se activa un interruptor en la página web.
El mensaje contiene el número GPIO que queremos controlar */
console.log(element.id); /* (element.id corresponde a la identificación
del interruptor deslizante que corresponde al número GPIO) */
miWebSocket.send(element.id); // envía el elemento.id
/*Además, también actualizamos el estado actual de GPIO en la página web: */
if (element.checked){
document.getElementById(element.id+"s").innerHTML = "ENCENDIDO";
}
else {
document.getElementById(element.id+"s").innerHTML = "APAGADO";
}
}
/* Entonces, la Wemos D1 debería manejar lo que sucede cuando recibe estos
mensajes: active o desactive los GPIO correspondientes y notifique a todos
los clientes. */
window.addEventListener('load', onLoad);