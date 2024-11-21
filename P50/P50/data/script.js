// Obtiene las lecturas del sensor cuando carga la página
window.addEventListener('load', obtieneLecturas);
// Crea gráfica de temperatura
var chartT = new Highcharts.Chart({
    chart: {
        renderTo: 'chart-temperature',
        backgroundColor: '#18181b'
    },
    series: [
        {
            name: 'Sensor BME280',
            color: '#9147ff'
        }
    ],
    title: {
        text: undefined,
        style: {
            color: '#FFFFFF',
            fontWeight: 'bold'
        }
    },
    plotOptions: {
        line: {
            animation: false,
            dataLabels: {
                enabled: true
            }
        },
        series: {
            color: '#9147ff'
        }
    },
    xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: { second: '%H:%M:%S' },
        labels: {
            style: {
                color: '#FFFFFF'
            }
        }
    },
    yAxis: {
        title: {
            text: 'Temperatura (°C))',
            style: {
                color: '#FFFFFF'
            }
        },
        labels: {
            style: {
                color: '#FFFFFF'
            }
        }
    },
    credits: {
        enabled: false
    },
    legend: {
        itemStyle: {
            color: '#FFFFFF',
            fontWeight: 'bold'
        }
    }

});
// Crea gráfica de humedad
var chartH = new Highcharts.Chart({
    chart: {
        renderTo: 'chart-humidity',
        backgroundColor: '#18181b'
    },
    series: [{
        name: 'Sensor BME280',
        color: '#9147ff'
    }],
    title: {
        text: undefined,
        style: {
            color: '#FFFFFF',
            fontWeight: 'bold'
        }
    },
    plotOptions: {
        line: {
            animation: false,
            dataLabels: {
                enabled: true
            }
        },
        series: {
            color: '#9147ff'
        }
    },
    xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: { second: '%H:%M:%S' },
        labels: {
            style: {
                color: '#FFFFFF'
            }
        }
    },
    yAxis: {
        title: {
            text: 'Humedad (%)',
            style: {
                color: '#FFFFFF'
            }
        },
        labels: {
            style: {
                color: '#FFFFFF'
            }
        }
    },
    credits: {
        enabled: false
    },
    legend: {
        itemStyle: {
            color: '#FFFFFF',
            fontWeight: 'bold'
        }
    }
});
// Crea gráfica de altitud
var chartP = new Highcharts.Chart({
    chart: {
        renderTo: 'chart-presion',
        backgroundColor: '#18181b'
    },
    series: [
        {
            name: 'Sensor BME280',
            color: '#9147ff'
        }
    ],
    title: {
        text: undefined,
        style: {
            color: '#FFFFFF',
            fontWeight: 'bold'
        }
    },
    plotOptions: {
        line: {
            animation: false,
            dataLabels: {
                enabled: true
            }
        },
        series: {
            color: '#9147ff'
        }
    },
    xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: { second: '%H:%M:%S' },
        labels: {
            style: {
                color: '#FFFFFF'
            }
        }
    },
    yAxis: {
        title: {
            text: 'Presion (hPa)',
            style: {
                color: '#FFFFFF'
            }
        },
        labels: {
            style: {
                color: '#FFFFFF'
            }
        }
    }
    ,
    credits: {
        enabled: false
    },
    legend: {
        itemStyle: {
            color: '#FFFFFF',
            fontWeight: 'bold'
        }
    }

});
// Crea gráfica de presion
var chartA = new Highcharts.Chart({
    chart: {
        renderTo: 'chart-altitud',
        backgroundColor: '#18181b'
    },
    series: [{
        name: 'Sensor BME280',
        color: '#9147ff'
    }],
    title: {
        text: undefined,
        style: {
            color: '#FFFFFF',
            fontWeight: 'bold'
        }
    },
    plotOptions: {
        line: {
            animation: false,
            dataLabels: {
                enabled: true
            }
        },
        series: {
            color: '#9147ff'
        }
    },
    xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: { second: '%H:%M:%S' },
        labels: {
            style: {
                color: '#FFFFFF'
            }
        }
    },
    yAxis: {
        title: {
            text: 'Altitud sobre nivel de mar (m)',
            style: {
                color: '#FFFFFF'
            }
        },
        labels: {
            style: {
                color: '#FFFFFF'
            }
        }
    },
    credits: {
        enabled: false
    },
    legend: {
        itemStyle: {
            color: '#FFFFFF',
            fontWeight: 'bold'
        }
    }
});
// Coloca los puntos de temperatura y humedad en las gráficas
function plotTemperature(timeValue, value) {
    console.log(timeValue);
    var x = new Date(timeValue * 1000).getTime();
    console.log(x);
    var y = Number(value);
    if (chartT.series[0].data.length > 40) {
        chartT.series[0].addPoint([x, y], true, true, true);
    } else {
        chartT.series[0].addPoint([x, y], true, false, true);
    }
}
// coloca los puntos de humedad en la gráfica correspondiente
function plotHumidity(timeValue, value) {
    console.log(timeValue);
    var x = new Date(timeValue * 1000).getTime();
    console.log(x);
    var y = Number(value);
    if (chartH.series[0].data.length > 40) {
        chartH.series[0].addPoint([x, y], true, true, true);
    } else {
        chartH.series[0].addPoint([x, y], true, false, true);
    }
}
// coloca los puntos de humedad en la gráfica correspondiente
function plotPresion(timeValue, value) {
    console.log(timeValue);
    var x = new Date(timeValue * 1000).getTime();
    console.log(x);
    var y = Number(value);
    if (chartP.series[0].data.length > 40) {
        chartP.series[0].addPoint([x, y], true, true, true);
    } else {
        chartP.series[0].addPoint([x, y], true, false, true);
    }
}
// coloca los puntos de humedad en la gráfica correspondiente
function plotAltitud(timeValue, value) {
    console.log(timeValue);
    var x = new Date(timeValue * 1000).getTime();
    console.log(x);
    var y = Number(value);
    if (chartA.series[0].data.length > 40) {
        chartA.series[0].addPoint([x, y], true, true, true);
    } else {
        chartA.series[0].addPoint([x, y], true, false, true);
    }
}
// Función para obtener las lecturas en la página web cuando
// carga por primera ocasión
function obtieneLecturas() {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var miObj = JSON.parse("[" + this.responseText.slice(0, -1) + "]");
            /* La propiedad de solo lectura XMLHttpRequest responseText devuelve el texto recibido de
            un servidor después de que se envía una solicitud. Con la propiedad slice es para leer la
            primera línea de un archivo de texto en javascript
            */
            var len = miObj.length;
            if (len > 40) {
                for (var i = len - 40; i < len; i++) {
                    plotTemperature(miObj[i].tiempo, miObj[i].temperatura);
                    plotHumidity(miObj[i].tiempo, miObj[i].humedad);
                    plotPresion(miObj[i].tiempo, miObj[i].presion);
                    plotAltitud(miObj[i].tiempo, miObj[i].altitud);
                }
            }
            else {
                for (var i = 0; i < len; i++) {
                    plotTemperature(miObj[i].tiempo, miObj[i].temperatura);
                    plotHumidity(miObj[i].tiempo, miObj[i].humedad);
                    plotPresion(miObj[i].tiempo, miObj[i].presion);
                    plotAltitud(miObj[i].tiempo, miObj[i].altitud);
                }
            }
        }
    };
    xhr.open("GET", "/lecturas", true);
    xhr.send();
}
if (!!window.EventSource) {
    var source = new EventSource('/events');
    source.addEventListener('open', function (e) {
        console.log("Eventos Conectados");
    }, false);
    source.addEventListener('error', function (e) {
        if (e.target.readyState != EventSource.OPEN) {
            console.log("Eventos Desconectados");
        }
    }, false);
    source.addEventListener('message', function (e) {
        console.log("message", e.data);
    }, false);
    source.addEventListener('nuevas_lecturas', function (e) {
        console.log("nuevas_lecturas", e.data);
        var miObj = JSON.parse(e.data);
        console.log(miObj);
        plotTemperature(miObj.tiempo, miObj.temperatura);
        plotHumidity(miObj.tiempo, miObj.humedad);
        plotPresion(miObj.tiempo, miObj.presion);
        plotAltitud(miObj.tiempo, miObj.altitud);
    }, false);
}