/* Este es código en Javascript es responsable de:
• Inicializar el protocolo de origen de eventos.
• Agregar un detector de eventos para el evento nuevas_lecturas.
• Creación de gráficos.
• Obtener las lecturas de sensor más recientes del evento y dibujarlas
en los gráficos;
• Hacer una solicitud HTTP GET para las lecturas actuales del sensor
cuando accede la página web por primera vez;
*/
// Obtiene las lecturas del sensor cuando carga la página
window.addEventListener('load', obtenerLecturas);
Highcharts.setOptions({
    time: {
        timezoneOffset: 7 * 60 //Se multiplica por el meridiano de Greenwich
    }
});
// Crea gráfica de Temperatura
/*Para crear un nuevo gráfico, se crea Highcharts.Chart () y se pasa como
argumento las propiedades del gráfico.
*/
var chartT = new Highcharts.Chart({
    chart:
    { /*En la siguiente línea, se define dónde desea colocar el gráfico.
En nuestro ejemplo, queremos colocarlo en la etiqueta <div>
con el ID grafica-temperatura (se vinclula en index.html) */
        renderTo: 'grafica-temperatura' /* La gráfica es colocada en el elemento HTML
con identificador 'grafica-temperatura' */
    },
    // se define el nombre de las series
    series: [
        {
            name: 'sensor BME280' //aparece abajo de la gráfica indicando la serie
        }
    ],
    title: {
        text: undefined
    },
    plotOptions:
    { //opciones generales que se aplican a las series

        line: {
            animation: false, /* animación: permite deshabilitar o alterar
las características de la inicial animación de una serie.
La animación está habilitada de forma predeterminada, por lo que
la configuramos en falso (false).
Puedes establecerlo en verdadero (true) para ver cómo funciona.
*/
            dataLabels: { /* • dataLabels: permite mostrar etiquetas de datos
para cada punto de datos en una serie en el gráfico. */
                enabled: true
                /* Hay muchas otras propiedades que puedes establecer en plotOption
                para cambiar la forma en que la serie de presenta. Se recomienda realizar una
                revisión a la documentación y probar las propiedades para ver cómo funciona:
                • https://www.highcharts.com/docs/chart-concepts/series
                (con este link pueden hacerse aportes a esta práctica)
                */
            }
        }
    },
    //Define las propiedades del eje X y el eje Y
    xAxis: {
        type: 'datetime', // etiqueta horizontal de la gráfica (el tiempo transcurriendo)
        dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
        title: {
            text: 'Temperatura en grados centígrados' // etiqueta vertical de la gráfica
        }
    },
    credits: { //Establece si se visualizan o no los creditos de Highcharts
        enabled: true
    }
});
// Crea gráfica de Humedad
var chartH = new Highcharts.Chart({ /*En la siguiente línea, se define dónde
se desea colocar el gráfico. En nuestro ejemplo, queremos colocarlo en
la etiqueta <div> con el ID grafica-humedad (se vinclula en index.html) */
    chart: {
        renderTo: 'grafica-humedad' /* La gráfica es colocada en el elemento HTML
con identificador 'grafica-humedad' */
    },
    series: [{ // se define el nombre de las series
        name: 'sensor BME280' //aparece abajo de la gráfica indicando la serie
    }],
    title: { //es para añadir un título a la gráfica
        text: 'Evitar humedecer el sensor'
    },
    subtitle: { //es para añadir un subtítulo a la gráfica
        text: 'Módulo de 4 terminales comunicación I2C'
    },
    plotOptions: { //opciones generales que se aplican a las series
        line: {
            animation: false, /* animación: permite deshabilitar o alterar
las características de la inicial animación de una serie.
La animación está habilitada de forma predeterminada, por lo que
la configuramos en falso (false).
Puede establecerlo en verdadero (true) para ver cómo funciona.
*/
            dataLabels: {
                enabled: true /* dataLabels: permite mostrar etiquetas de datos
para cada punto de datos en una serie en el gráfico. */
            }
        },
        series: {
            color: '#50b8b4' // color de la serie de puntos para humedad
        }
    },
    //Define las propiedades del eje X y el eje Y
    xAxis: {
        type: 'datetime', // etiqueta horizontal de la gráfica (el tiempo transcurriendo)
        dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
        title: {
            text: 'Humedad (%)' // etiqueta vertical de la gráfica
        }
    },
    credits: { //Establece si se visualizan o no los creditos de Highcharts
        enabled: false //para la gráfica de humedad no se visualizan
    }
});
/* Para trazar la temperatura y la humedad en los gráficos, creamos dos
funciones:
graficarTemperatura () y graficarHumedad ()
que aceptan como argumento el valor que queremos trazar.
*/
//Dibuja el valor de la temperatura en el gráfica de Temperatura
function graficarTemperatura(value) { /* para agregar un punto al gráfico,
usamos las siguientes líneas: */
    var x = (new Date()).getTime() // El valor 'x' del gráfico es la marca de tiempo.
    var y = Number(value); /*El valor de 'y' se pasa como argumento. Necesitamos
convertirlo en un número. */
    if (chartT.series[0].data.length > 40) { /* Nuestros gráficos solo tienen una serie (el índice es 0). Entonces,
podemos acceder a la serie en el gráfico de temperatura utilizando:
chartT.series [0].
Primero, verificamos la longitud de los datos de la serie:
• Si la serie tiene más de 40 puntos: agregar y cambiar un nuevo punto;
• O si la serie tiene menos de 40 puntos: agregar un nuevo punto.
*/
        chartT.series[0].addPoint([x, y], true, true, true); // últimos argumentos: Redraw,shift,
        // withEvent
        /* Para agregar un nuevo punto, se usa el método addPoint ()
        que acepta lo siguientes argumentos:
        El valor que se trazará. Si es un solo número, un punto con ese valor y es
        adjunto a la serie. Si es una matriz, se interpretará como valores x e y.
        En nuestro caso, pasamos una matriz con los valores x e y;
        • Opción Redraw -Redibujar- (booleano): establezca en true para volver a dibujar
        el gráfico después de que el punto sea adicional.
        • Opción de corrimiento -shift- (booleano): si es verdadero, un punto se desplaza del
        inicio de la serie como uno se adjunta al final. Cuando la longitud del
        gráfico es superior a 40, establecemos la opción de cambio a verdadero.
        • Opción con evento -withEvent- (booleana): se utiliza internamente para activar
        el evento addPoint de la serie.
        */
    }
    else {
        chartT.series[0].addPoint([x, y], true, false, true); // últimos argumentos: Redraw,shift,
        //withEvent
        /* • Opción Redraw -Redibujar- (booleano): establezca en true para volver a dibujar
        el gráfico después de que el punto sea adicional.
        • Opción de corrimiento -shift- (booleano): si es falso, un punto NO se desplaza del
        inicio de la serie como uno se adjunta al final. Cuando la longitud del
        gráfico NO es superior a 40, establecemos la opción de cambio a falso.
        • Opción con evento -withEvent- (booleana): se utiliza internamente para activar
        el evento addPoint de la serie.
        */
    }
}
//Dibuja el valor de humedad en la gráfica de humedad
function graficarHumedad(value) { /* para agregar un punto al gráfico,
usamos las siguientes líneas: */
    var x = (new Date()).getTime() // El valor 'x' del gráfico es la marca de tiempo.
    var y = Number(value); /*El valor de 'y' se pasa como argumento. Necesitamos
convertirlo en un número. */
    if (chartH.series[0].data.length > 40) { /* Nuestros gráficos solo tienen una serie (el índice es 0). Entonces,
podemos acceder a la serie en el gráfico de temperatura utilizando:
chartT.series [0].
Primero, verificamos la longitud de los datos de la serie:
• Si la serie tiene más de 40 puntos: agregar y cambiar un nuevo punto;
• O si la serie tiene menos de 40 puntos: agregar un nuevo punto.
*/
        chartH.series[0].addPoint([x, y], true, true, true); // últimos argumentos: Redraw,shift,
        //withEvent
        /* Para agregar un nuevo punto, se usa el método addPoint ()
        que acepta lo siguientes argumentos:
        El valor que se trazará. Si es un solo número, un punto con ese valor y es
        adjunto a la serie. Si es una matriz, se interpretará como valores x e y.
        En nuestro caso, pasamos una matriz con los valores x e y;
        • Opción Redraw -Redibujar- (booleano): establezca en true para volver a dibujar
        el gráfico después de que el punto sea adicional.
        • Opción de corrimiento -shift- (booleano): si es verdadero, un punto se desplaza del
        inicio de la serie como uno se adjunta al final. Cuando la longitud del
        gráfico es superior a 40, establecemos la opción de cambio a verdadero.
        • Opción con evento -withEvent- (booleana): se utiliza internamente para activar
        el evento addPoint de la serie.
        */
    }
    else {
        chartH.series[0].addPoint([x, y], true, false, true); // últimos argumentos: Redraw,shift,
        //withEvent
        /* • Opción Redraw -Redibujar- (booleano): establezca en true para volver a dibujar
        el gráfico después de que el punto sea adicional.
        • Opción de corrimiento -shift- (booleano): si es falso, un punto NO se desplaza del
        inicio de la serie como uno se adjunta al final. Cuando la longitud del
        gráfico NO es superior a 40, establecemos la opción de cambio a falso.
        • Opción con evento -withEvent- (booleana): se utiliza internamente para activar
        el evento addPoint de la serie.
        */
    }
}
// Función para obtener las lecturas actuales en la página web cuando se carga
function obtenerLecturas() {
    /* Cuando la página se carga en su navegador, llama a la función obtenerLecturas ()
    para obtener las lecturas actuales del sensor. Esa función realiza una solicitud
    HTTP URL en /lecturas.*/
    var variableAJAX = new XMLHttpRequest();
    variableAJAX.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            /* El servidor responde con una cadena JSON que contiene
            las últimas lecturas del sensor. */
            var miObjetoJSON = JSON.parse(this.responseText);
            console.log(miObjetoJSON);
            var temp = miObjetoJSON.temperatura;
            var hum = miObjetoJSON.humedad;
            graficarTemperatura(temp);
            /* Traza las lecturas del sensor llamando a las funcionesgraficarTemperatura ()
            y graficarHumedad () y les pasa la temperatura (miObjetoJSON.temperatura) y
            la humedad (miObjetoJSON.humedad) como argumentos. */
            graficarHumedad(hum);
        }
    };
    variableAJAX.open("GET", "/lecturas", true);
    variableAJAX.send();
}
if (!!window.EventSource) /* Para suscribirse a una transmisión de eventos, crear un objeto
EventSource y pasarle la URL de su transmisión:*/ { /* Grafica las lecturas en las gráficas cuando
el cliente reciba las lecturas en el evento nuevas_lecturas. */
    var fuenteDeEventos = new EventSource('/eventos'); /* Creamos un nuevo objeto
fuenteDeEventos y especificamos la URL de la página que envían las
actualizaciones. En nuestro caso, es /eventos.
*/
    /* Una vez que haya creado una instancia de una fuente
    de evento, se puede comenzar a "escuchar" los mensajes
    de el servidor con addEventListener ().
    */
    /* En el siguiente link se muestran los detectores de
    eventos predeterminados, de acuerdo a la
    documentación de AsyncWebServer.
    https://github.com/me-no-dev/ESPAsyncWebServer#setup-event-source-in-the-browser
    */
    fuenteDeEventos.addEventListener('open', function (e) { /* abierto para "escuchar"
la ocurrencia de eventos */
        console.log("evento conectado"); //envia a consola
    }, false);
    fuenteDeEventos.addEventListener('error', function (e) { // detecta si hay error
        if (e.target.readyState != EventSource.OPEN) {
            console.log("evento desconectado"); //envia a consola
        }
    }, false);
    fuenteDeEventos.addEventListener('message', function (e) { //detecta mensaje
        console.log("mensaje", e.data); // envía a consola
    }, false);
    /* Luego, agregamos un detector de eventos ("escuchador")
    para el evento 'nuevas_lecturas' */
    fuenteDeEventos.addEventListener
        ('nuevas_lecturas', function (e) {
            console.log("nuevas_lecturas", e.data);
            /* Necesitamos convertir la cadena JSON en una VariableObjeto
            JSON usando el método parse ().El resultado se guarda en la
            variable miObjetoJSON. */
            var miObjetoJSON = JSON.parse(e.data);
            console.log(miObjetoJSON); // se envía a consola
            /* ya que se convierten los datos con el método parse ()
            podemos hacer uso de la variable miObjetoJSON. */
            /* Ahora se puede graficar:
            la temperatura con miObjetoJSON.temperatura,
            la humedad con miObjetoJSON.humedad y
            la presión con VariableObjeto.presion.
            */
            graficarTemperatura(miObjetoJSON.temperatura);
            graficarHumedad(miObjetoJSON.humedad);
        }, false); // el valor predeterminado es falso para Chrome.
    /* de esta manera, cuando hay nuevas lecturas disponibles,
    el ESP32 de la Wemos D1 envía un evento ('nuevas_lecturas')
    al cliente con una cadena JSON que contiene las lecturas
    del sensor para graficar temperatura y humedad */
}