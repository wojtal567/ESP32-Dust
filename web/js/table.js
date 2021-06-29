window.addEventListener('DOMContentLoaded', event => {
    const sidebarToggle = document.body.querySelector('#sidebarToggle');
    if (sidebarToggle) {
        sidebarToggle.addEventListener('click', event => {
            event.preventDefault();
            document.body.classList.toggle('sb-sidenav-toggled');
            localStorage.setItem('sb|sidebar-toggle', document.body.classList.contains('sb-sidenav-toggled'));
        });
    }
    const datatablesSimple = document.getElementById('datatablesSimple');
    if (datatablesSimple) {
        new simpleDatatables.DataTable(datatablesSimple);
    }
});

$(document).ready ( function(){
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
