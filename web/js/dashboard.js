window.addEventListener('DOMContentLoaded', event => {
    const sidebarToggle = document.body.querySelector('#sidebarToggle');
    if (sidebarToggle) {
        sidebarToggle.addEventListener('click', event => {
            event.preventDefault();
            document.body.classList.toggle('sb-sidenav-toggled');
            localStorage.setItem('sb|sidebar-toggle', document.body.classList.contains('sb-sidenav-toggled'));
        });
    }
});


$(document).ready ( function(){
    var sdErrorParent = document.getElementById("sderror");
    sdError = document.createElement("div");
    sdError.classList.add("col-mb-4");
    sdError.classList.add("card");
    sdError.classList.add("bg-warning");      //red - danger, yellow - warning           
    sdError.classList.add("text-white");                
    sdError.classList.add("mb-4");                
    sdErrorBody = document.createElement("div");
    sdErrorBody.classList.add("card-body");
    sdErrorText = document.createTextNode("Placeholder for SD Card info");
    sdErrorBody.appendChild(sdErrorText);
    sdError.appendChild(sdErrorBody);
    sdErrorParent.appendChild(sdError);
    initWebSocket();
    getYear();
}); 

setInterval(function() {
    getYear();
}, 1000);

function getYear()
{
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
    if(this.readyState===4 && this.status === 200)
        $("#datetime").text("Current date/time: " + this.responseText);
    };
    xhttp.open("GET", "/timestamp", true);
    xhttp.send();
}
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage;
}
function onOpen(event) {
    console.log('Connection opened');
    websocket.send('lastSample');
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

function onMessage(event) {
    console.log(event.data);
    var lastSample = JSON.parse(event.data);
    $("#lastTimestamp").text(lastSample.timestamp);
    $("#temp").text(lastSample.Temperature);
    $("#hum").text(lastSample.Humidity);
    $("#pm25").text(lastSample.Pm25);
    $("#pm10").text(lastSample.Pm10);
    $("#pm100").text(lastSample.Pm100);
    $("#part03").text(lastSample.Particles03);
    $("#part05").text(lastSample.Particles05);
    $("#part10").text(lastSample.Particles10);
    $("#part25").text(lastSample.Particles25);
    $("#part50").text(lastSample.Particles50);
    $("#part100").text(lastSample.Particles100);
}