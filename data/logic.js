//chart code
//chart config
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
            day: '%e.%b %H:%M'
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
            var response = this.responseText;
            var x = (new Date()).getTime(),
                y = parseFloat(response);
            if (y > 900) {
                if (chartT.series[0].data.length > 4320) {
                    chartT.series[0].addPoint([x, y], true, true, true);
                } else {
                    chartT.series[0].addPoint([x, y], true, false, true);
                }
                document.getElementById("pressure").innerHTML = String(parseFloat(response).toFixed(1));
            }
        }
    };
    xhttp.open("GET", "/pressure", true);
    xhttp.send();
}, 10000);

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