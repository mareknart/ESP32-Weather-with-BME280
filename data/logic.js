readingPressure();
translation();

Highcharts.setOptions({
    time: {
        timezoneOffset: -120 //Add your time zone offset here in minutes
    }
});
var chartT = new Highcharts.Chart({
    chart: {
        renderTo: 'pressure_chart'
    },
    series: [{
        name: chartTranslate("chartLegend"),
        type: 'line',
        color: '#101D42',
        marker: {
            symbol: 'circle',
            radius: 3,
            fillColor: '#101D42'
        }
    }],
    title: {
        text: chartTranslate("chartName")
    },
    xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: {
            day: '%d.%m.%y %H:%M:%S'
        }
    },
    yAxis: {
        title: {
            text: chartTranslate("yAxis")
        }
    },
    credits: {
        enabled: false
    }
})

setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("temperature").innerHTML = String(parseFloat(this.responseText).toFixed(1));
        }
    };
    xhttp.open("GET", "/temperature", true);
    xhttp.send();
}, 10000);
setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("humidity").innerHTML = String(parseFloat(this.responseText).toFixed(1));
        }
    };
    xhttp.open("GET", "/humidity", true);
    xhttp.send();
}, 10000);
setInterval(function () {
    readingPressure();
}, 1800000);

function readingPressure() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var response = this.responseText;

            //getting data from json file
            readJsonFile("data.json", function (text) {
                var data = JSON.parse(text);
                data.forEach(item => {
                    var tempDate = item.time;
                    var x = tempDate * 1000,
                        y = item.pressure;
                    if (chartT.series[0].data.length > 4320) {
                        chartT.series[0].addPoint([x, y], true, true, true);
                    } else {
                        chartT.series[0].addPoint([x, y], true, false, true);
                    }
                    
                });
                document.getElementById("pressure").innerHTML = String((data[data.length - 1].pressure));
                const today = new Date((data[data.length - 1].time)*1000);
                document.getElementById("lastUpdate").innerHTML = today.toLocaleString();
            })
        }
    }
    xhttp.open("GET", "/pressure", true);
    xhttp.send();
}

//json file

function readJsonFile(file, callbac) {
    var rawFile = new XMLHttpRequest();
    rawFile.overrideMimeType("application/json");
    rawFile.open("GET", file, true);
    rawFile.onreadystatechange = function () {
        if (rawFile.readyState === 4 && rawFile.status == "200") {
            callbac(rawFile.responseText);
        }
    }
    rawFile.send(null);
}

function chartTranslate(name) {
    var plLanguage = {
        chartName: "Poprzednie wskazania",
        yAxis: "Ciśnienie hPa",
        chartLegend: "Ciśnienie"
    };
    var enLanguage = {
        chartName: "Previous readings",
        yAxis: "Pressure hPa",
        chartLegend: "Pressure"
    };
    var language = navigator.language;
    if (language == "pl" || language == "pl-PL") {
        return plLanguage[name];
    }
    return enLanguage[name];
}

function translation() {
    var plLanguage = ["Temperatura", "Wilotność", "Ciśnienie", "Ostatnia aktualizacja: ", "Stacja pogodowa"];
    var enLanguage = ["Temperature", "Humidity", "Pressure", "Last update: ", "Weather station"];
    var language = navigator.language;
    if (language == "pl" || language == "pl-PL") {
        document.getElementById("temperatureLabel").innerHTML = plLanguage[0];
        document.getElementById("humidityLabel").innerHTML = plLanguage[1];
        document.getElementById("pressureLabel").innerHTML = plLanguage[2];
        document.getElementById("lastUpdateLabel").innerHTML = plLanguage[3];
        document.getElementById("mainName").innerHTML = plLanguage[4];
    } else {
        document.getElementById("temperatureLabel").innerHTML = enLanguage[0];
        document.getElementById("humidityLabel").innerHTML = enLanguage[1];
        document.getElementById("pressureLabel").innerHTML = enLanguage[2];
        document.getElementById("lastUpdateLabel").innerHTML = enLanguage[3];
        document.getElementById("mainName").innerHTML = enLanguage[4];
    }
}