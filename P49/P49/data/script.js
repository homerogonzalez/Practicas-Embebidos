// Obtiene las lecturas del sensor cuando carga la página
window.addEventListener('load', obtieneLecturas);
// Crea gráfica de temperatura
var chartT = new Highcharts.Chart({
chart:{
renderTo:'chart-temperature'
},
series: [
{
name: 'Sensor BME280'
}
],
title: {
text: undefined
},
plotOptions: {
line: {
animation: false,
dataLabels: {
enabled: true
}
}
},
xAxis: {
type: 'datetime',
dateTimeLabelFormats: { second: '%H:%M:%S' }
},
yAxis: {
title: {
text: 'Temperatura grados Centigrados'
}
},
credits: {
enabled: false
}
});
// Crea gráfica de humedad
var chartH = new Highcharts.Chart({
chart:{
renderTo:'chart-humidity'
},
series: [{
name: 'Sensor BME280'
}],
title: {
text: undefined
},
plotOptions: {
    line: {
    animation: false,
    dataLabels: {
    enabled: true
    }
    },
    series: {
    color: '#50b8b4'
    }
    },
    xAxis: {
    type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
    },
    yAxis: {
    title: {
    text: 'Humedad (%)'
    }
    },
    credits: {
    enabled: false
    }
    });
    // Coloca los puntos de temperatura y humedad en las gráficas
    function plotTemperature(timeValue, value){
    console.log(timeValue);
    var x = new Date(timeValue*1000).getTime();
    console.log(x);
    var y = Number(value);
    if(chartT.series[0].data.length > 40) {
    chartT.series[0].addPoint([x, y], true, true, true);
    } else {
    chartT.series[0].addPoint([x, y], true, false, true);
    }
    }
    // coloca los puntos de humedad en la gráfica correspondiente
    function plotHumidity(timeValue, value){
    console.log(timeValue);
    var x = new Date(timeValue*1000).getTime();
    console.log(x);
    var y = Number(value);
    if(chartH.series[0].data.length > 40) {
    chartH.series[0].addPoint([x, y], true, true, true);
    } else {
    chartH.series[0].addPoint([x, y], true, false, true);
    }
    }
    // Función para obtener las lecturas en la página web cuando
    // carga por primera ocasión
    function obtieneLecturas(){
        var xhr = new XMLHttpRequest();
        xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
        var miObj = JSON.parse("["+this.responseText.slice(0, -1)+"]");
        /* La propiedad de solo lectura XMLHttpRequest responseText devuelve el texto recibido de
        un servidor después de que se envía una solicitud. Con la propiedad slice es para leer la
        primera línea de un archivo de texto en javascript
        */
        var len = miObj.length;
        if(len > 40) {
        for(var i = len-40; i<len; i++){
        plotTemperature(miObj[i].tiempo, miObj[i].temperatura);
        plotHumidity(miObj[i].tiempo, miObj[i].humedad);
        }
        }
        else {
        for(var i = 0; i<len; i++){
        plotTemperature(miObj[i].tiempo, miObj[i].temperatura);
        plotHumidity(miObj[i].tiempo, miObj[i].humedad);
        }
        }
        }
        };
        xhr.open("GET", "/lecturas", true);
        xhr.send();
        }
        if (!!window.EventSource) {
        var source = new EventSource('/events');
        source.addEventListener('open', function(e) {
        console.log("Eventos Conectados");
        }, false);
        source.addEventListener('error', function(e) {
        if (e.target.readyState != EventSource.OPEN) {
        console.log("Eventos Desconectados");
        }
        }, false);
        source.addEventListener('message', function(e) {
        console.log("message", e.data);
        }, false);
        source.addEventListener('nuevas_lecturas', function(e) {
        console.log("nuevas_lecturas", e.data);
        var miObj = JSON.parse(e.data);
        console.log(miObj);
        plotTemperature(miObj.tiempo, miObj.temperatura);
        plotHumidity(miObj.tiempo, miObj.humedad);
        }, false);
        }