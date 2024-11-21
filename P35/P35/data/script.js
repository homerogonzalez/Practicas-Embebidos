// Se obtienen los estados actuales de los GPIOs cuando la pagina carga
window.addEventListener('load', obtieneEstados);
// Función para obtener y actualizar los estados de los GPIOs
// en la página web cuando carga por primera ocasión.
function obtieneEstados(){
var xhr_intercambiadatossinrecargartodalapagina = new XMLHttpRequest();
// se crea el objeto para intercambiar datos parciales de la página web
xhr_intercambiadatossinrecargartodalapagina.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200)
{
var miObjetoProvenienteDeTextoJSON = JSON.parse(this.responseText);
/* JSON.parse() es una función interna de JavaScript para convertir una
cadena escrita en formato JSON a objetos nativos de JavaScript */
console.log(miObjetoProvenienteDeTextoJSON);
for (i in miObjetoProvenienteDeTextoJSON.gpios) {
var salida = miObjetoProvenienteDeTextoJSON.gpios[i].salida;
var estado = miObjetoProvenienteDeTextoJSON.gpios[i].estado;
console.log(salida);
console.log(estado);
// podemos observar salida y estado de cada GPIO en consola
if (estado == "1") {
document.getElementById(salida).checked = true;
document.getElementById(salida+"s").innerHTML = "ENCENDIDO";
/* con la "s" concatenada a salida,
necesitamos actualizar el texto de estado a "ENCENDIDO" */
}
else {
document.getElementById(salida).checked = false;
document.getElementById(salida+"s").innerHTML = "APAGADO";
/* con la "s" concatenada a salida,
necesitamos actualizar el texto de estado a "APAGADO" */
}
}
}
};
xhr_intercambiadatossinrecargartodalapagina.open("GET", "/quierosaberestados", true);
xhr_intercambiadatossinrecargartodalapagina.send();
// Envía las salidas y los estados actuales del los GPIOs
}
// Envía peticiones para controlar GPIOs
function toggleCheckbox (element) {
    /* Función de JavaScript que envía peticiones al ESP32 de la Wemos D1 R32
cuando hacemos clic en los switches deslizadores, esta función
acepta como argumento el id de le checkboox */
var xhr_intercambiadatossinrecargartodalapagina = new XMLHttpRequest();
if (element.checked) {
xhr_intercambiadatossinrecargartodalapagina.open("GET",
"/actualizar?salida="+element.id+"&estado=1", true);
/* se recibe una petición al mover cada switch. 'actualizar' está
también en los archivos de main.cpp y script.json */
document.getElementById(element.id+"s").innerHTML = "ENCENDIDO";
/* con la "s" concatenada a salida,
necesitamos actualizar el texto de estado a "ENCENDIDO" */
}
else {
xhr_intercambiadatossinrecargartodalapagina.open("GET",
"/actualizar?salida="+element.id+"&estado=0", true);
document.getElementById(element.id+"s").innerHTML = "APAGADO";
/* con la "s" concatenada a salida,
necesitamos actualizar el texto de estado a "APAGADO" */
}
xhr_intercambiadatossinrecargartodalapagina.send();
// Envía las salidas y los estados actuales del los GPIOs
}