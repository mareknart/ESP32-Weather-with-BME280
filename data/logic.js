window.addEventListener('load', getReadings);

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
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("pressure").innerHTML = String(parseFloat(this.responseText).toFixed(1));
        }
    };
    xhttp.open("GET", "/pressure", true);
    xhttp.send();
}, 10000);


//chart code
//chart config
var chartT = new Highcharts.Chart({
    chart: {
        renderTo: 'pressure_chart'
    },
    series: [{
        name: 'Ciśnienie',
        type: 'line',
        color: '#101D42',
        marker: {
            symbol: 'circle',
            radius: 3,
            fillColor: '#101D42'
        }
    }],
    title: {
        text: "Poprzednie wskazania"
    },
    xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: {
            second: '%H:%M:%S'
        }
    },
    yAxis: {
        title: {
            text: 'Ciśnienie hPa'
        }
    },
    credits: {
        enabled: false
    }
})

function plotPressure(jsonValue) {

    var keys = Object.keys(jsonValue);
    console.log("klucz " + keys);
    console.log(keys.length);

    //for (var i = 0; i < keys.length; i++) {
        var x = (new Date()).getTime();
        console.log(x);
        const key = keys[0];
        var y = Number(jsonValue[key]);
        console.log(y);

        if (chartT.series[0].data.length > 4320) {
            chartT.series[0].addPoint([x, y], true, true, true);
        } else {
            chartT.series[0].addPoint([x, y], true, false, true);
        }

    //}
}

function getReadings() {
    var xhr = new XMLHttpRequest();
    xhr.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var myObj = JSON.parse(this.responseText);
            console.log(myObj);
            plotPressure(myObj);
        }
    };
    xhr.open("GET", "/readings", true);
    xhr.send();
}

if (!!window.EventSource) {
    var source = new EventSource('/events');

    source.addEventListener('open', function (e) {
        console.log("Events Connected");
    }, false);

    source.addEventListener('error', function (e) {
        if (e.target.readyState != EventSource.OPEN) {
            console.log("Events Disconnected");
        }
    }, false);

    source.addEventListener('message', function (e) {
        console.log("message", e.data);
    }, false);

    source.addEventListener('new_readings', function (e) {
        console.log("new_readings", e.data);
        var myObj = JSON.parse(e.data);
        console.log(myObj);
        plotPressure(myObj);
    }, false);
}