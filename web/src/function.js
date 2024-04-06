function toggleMenu() {
    var menu = document.getElementById("menu");
    menu.classList.toggle("open");
}

function toggleTick(item) {
    item.classList.toggle("ticked");
}

// Function to update the clock
function updateClock() {
    var now = new Date();
    var hours = now.getHours();
    var minutes = now.getMinutes();
    var seconds = now.getSeconds();

    // Pad single digit minutes and seconds with leading zeros
    minutes = minutes < 10 ? '0' + minutes : minutes;
    seconds = seconds < 10 ? '0' + seconds : seconds;

    // Update the clock element with the current time
    var clockElement = document.getElementById('clock');
    clockElement.textContent = hours + ':' + minutes + ':' + seconds;

    // Update the clock every second
    setTimeout(updateClock, 1000);
}

// Initial call to update clock when the page loads
updateClock();