/* Este es responsable de inicializar una conexión WebSocket con el servidor,
solicita al servidor el valor actual del control deslizante tan pronto como
la interfaz web está completamente cargada en el navegador y maneja el intercambio
de datos a través del protocolo WebSocket. */
/*La puerta de enlace es el punto de entrada a la interfaz de WebSocket. */
var PuertaDeEnlaceAlSocket = `ws://${window.location.hostname}/ws`;
/* window.location.hostname obtiene la dirección de la página actual
(la IP del servidor web)
Crear una nueva variable global llamada miWebSocket. */
var miWebSocket;
/* Se agrega un detector de eventos que llamará a la función onload
cuando se cargue la página web. */
window.addEventListener('load', onload);
function onload(event) {
/* La función onload () llama a la función initWebSocket () para
inicializar una conexión de WebSocket con el servidor y la función
initButton () para agregar detectores de eventos a los botones. */
initWebSocket();
obtenerValorActual();
}
function initWebSocket() {
/* La función initWebSocket () inicializa una conexión WebSocket en
PuertaDeEnlaceAlSocket definida anteriormente. También asignamos varias funciones
de devolución de llamada para cuando la conexión del WebSocket se abre,
se cierra o cuando se recibe un mensaje. */
console.log('Intentando abrir una conexión WebSocket...');
miWebSocket = new WebSocket(PuertaDeEnlaceAlSocket);
miWebSocket.onopen = onOpen;
miWebSocket.onclose = onClose;
miWebSocket.onmessage = onMessage;
}
function onOpen(event) { // función de devolución de llamada para
//cuando la conexión del WebSocket se abre
console.log('Conexión abierta');
}
function onClose(event) { // función de devolución de llamada para
//cuando la conexión del WebSocket se cierra
console.log('Conexión cerrada');
setTimeout(initWebSocket, 2000);
}
onMessage = (event) => { // función de devolución de llamada para
//cuando se recibe un nmensaje
console.log(event.data);
document.getElementById("DeslizadorPWM").value = event.data;
document.getElementById("textoValorDeslizador").innerHTML = event.data;
}
function obtenerValorActual() {
/* La función obtenerValorActual () realiza una solicitud en la URL /ValorActual y
maneja la respuesta. Se vincula con main.cpp*/
var variableAJAX = new XMLHttpRequest();
variableAJAX.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {
document.getElementById("DeslizadorPWM").value = this.responseText;
document.getElementById("textoValorDeslizador").innerHTML = this.responseText;
}
};
variableAJAX.open("GET", "/ValorActual", true);
variableAJAX.send();
/* Cuando el servidor recibe esa solicitud, responde con el valor del control deslizante.
Establecemos la posición actual del control deslizante en ese valor. El ID del control
deslizante es DeslizadorPWM y es posible establecer su valor usando .value como
se realizó previamente con:
document.getElementById("pwmSlider").value = this.responseText;
Finalmente, se agrega el valor al párrafo con el id textoValorDeslizador
(vinculado en index.html)
document.getElementById("textoValorDeslizador").innerHTML = this.responseText;
*/
}
function updateSliderPWM(element) {
//function actualizaDeslizadorPWM(element) {
// esta función es llamada cuando se arrastra el deslizador
var ValorDeslizador = document.getElementById("DeslizadorPWM").value;
/*La función obtiene el valor actual del control deslizante y lo guarda en
la variable ValorDeslizador. */
document.getElementById("textoValorDeslizador").innerHTML = ValorDeslizador;
/* Luego, actualiza el valor del control deslizante ValorDeslizador en el párrafo con
el id de textoValorDeslizador */
console.log(ValorDeslizador);
/* presenta el valor del control deslizante en la consola para fines de depuración */
miWebSocket.send(ValorDeslizador);
/* Finalmente, envía el valor del control deslizante ValorDeslizador al cliente mediante
el protocolo WebSocket */
}