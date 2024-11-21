/* Este es responsable de inicializar una conexión WebSocket con el servidor,
solicita al servidor el valor actual del control deslizante tan pronto como
la interfaz web está completamente cargada en el navegador y maneja el intercambio
de datos a través del protocolo WebSocket. */
// La compuerta es el punto de entrada a la interfaz de WebSocket.
var compuerta = `ws://${window.location.hostname}/ws`;
/* window.location.hostname obtiene la dirección de la página actual
(la IP del servidor web ) */
var miVariableWebsocket;
/* Se agrega un detector de eventos que llamará a la función cuandocarga
cuando se cargue la página web. */
window.addEventListener('load', cuandocarga);
function cuandocarga(event) {
inicializaWebSocket();
inicializaBoton() ;/* llama a la función incializaBoton () */
}
function inicializaWebSocket() {
console.log('Intentando abrir una conexión de tipo WebSocket...');
miVariableWebsocket = new WebSocket(compuerta);
miVariableWebsocket.onopen = onOpen;
miVariableWebsocket.onclose = onClose;
miVariableWebsocket.onmessage = onMessage;
}
function onOpen(event) {
    console.log('Conexión abierta');
}
function onClose(event) {
console.log('Conexión cerrada');
setTimeout(initWebSocket, 2000);
}
function onMessage(event) {
document.getElementById('estado').innerHTML = event.data;
console.log(event.data);
}
function inicializaBoton() {/* La función inicializaBoton () obtiene los botones por sus
identificadores ('bON y 'bOFF') y agrega un detector de eventos
de tipo 'clic'. */
document.getElementById('bON').addEventListener('click', toggleON);
document.getElementById('bOFF').addEventListener('click', toggleOFF);
}
/* La función toggleON () envía un mensaje usando la conexión WebSocket
con el texto 'bON' */
function toggleON(event) {
miVariableWebsocket.send('bON');
}
/* La función toggleOFF () envía un mensaje usando la conexión WebSocket
con el texto 'bOFF' */
function toggleOFF(event) {
miVariableWebsocket.send('bOFF');
}
/* Entonces, el ESP32 (microcontrolador de la Wemos D1 R32, debería manejar
lo que sucede cuando recibe estos Mensajes: enciende o apaga el LED y
notifica a todos los clientes. */