const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
    :root {
        --water-level: 150px;
        --primary-color: #039be4;
        --accent-color: #00979c;
        --background-color: #f5f5f5;
    }
    
    body {
        margin: 0;
        font-family: Segoe, "Segoe UI", "DejaVu Sans", "Trebuchet MS", Verdana, sans-serif;
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;
        height: 100vh;
        background: var(--background-color);
        overflow: hidden;
    }

    .title {
        position: absolute;
        top: 3vh;
        left: 0;
        width: 100vw;
        font-size: 4em;
        font-weight: bold;
        text-align: center;
        color: var(--accent-color);
        text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.2);
        animation: fadeIn 1s ease-in;
    }

    .arduino-logo {
        position: absolute;
        top: 20px;
        right: 20px;
        width: 100px;
        height: 100px;
		background: url('data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSI3MjAiIGhlaWdodD0iNDkwIiBzdHJva2U9IiMwMDk3OWMiPjxnIGZpbGw9Im5vbmUiPjxwYXRoIGQ9Ik0xNzQgMzBDNzguOSAyMi40My0xIDEzNSAzOCAyMjJjMzggOTQuNCAxODEgMTIwIDI0OSA0NC40IDY1LjktNjQuNiA5MS4zLTE2NCAxNzItMjE1IDgyLTU2IDIxNC0zLjEyIDIzMCA5Ni40IDE1LjYgOTUuMy04Ny4xIDE4OC0xODAgMTYwLTY4LjctMTUuNS0xMTgtNzQuMi0xNDYtMTM2LTM0LjUtNTktNzkuNi0xMjItMTUwLTEzOC0xMi41LTIuODQtMjUuNC0zLjk2LTM4LjItMy45NHoiIHN0cm9rZS13aWR0aD0iNTkiLz48cGF0aCBkPSJtMTE4IDE2NWgxMjAiIHN0cm9rZS13aWR0aD0iMjUiLz48cGF0aCBkPSJtNDg2IDE2NWgxMDBtLTUxLTUwdjEwMCIgc3Ryb2tlLXdpZHRoPSIzMiIvPjwvZz48cGF0aCBkPSJNODQgNDg1bC02LjQtMjNIMzdsLTYuNiAyM0g4LjdsMzQuNC0xMTZoMzAuNmwzNC40IDExNnptLTI2LjQtOTVsLTE1LjMgNTQuMWgzMC40ek0xODAgNDg1cy0xMS43LTI3LjYtMTguMy00MWMtMy44My01Ljc2LTExLjQtOC4wMi0xOC03LjEtLjYgMTUuNy0uNDggNDguMS0uNDggNDguMWgtMjEuOVYzNjlzMzMtLjk3IDQ5LjQgMS4yYzM0LjcgMy45MiAzNyA1NC40IDIuMTMgNTkuNyA2LjU0IDIuNiAxMC41IDguNjUgMTMuMyAxNC44bDE3LjUgMzYuMmMxLjIzIDIuNDggMS4yIDQuNC0yLjM0IDQuMzh6bS01LjQyLTgyLjFjLS4xLTE3LjktMTgtMTUuOS0zMS4zLTE2djMyLjhjMTUuNy41MyAzMS0xLjA3IDMxLjMtMTYuOHpNMzAyIDQ1NWMtMTAuNiAyNC41LTMzLjggMzAuMi01NS44IDI5LjhoLTI2Ljl2LTExNmw0MC45LjJjNTAuNiAxLjY1IDUyLjQgNTYuMiA0MS43IDg2LjR6bS0yOC02MS4zYy0xMC4yLTguNzYtMjEuNC02Ljk3LTMyLjgtNi45N3Y3OS43YzEwIDAgMjAuOS44IDI5LjQtNS40IDE4LjEtMTEuNCAxNS44LTU0LjYgMy4zNS02Ny4zek00MDcgNDQyYzIuMTcgNTguNi04MS41IDU4LjQtODQgMTIuNy0xLjUzLTE0LS4yNy0yOC0uNjgtNDJ2LTQ0LjJoMjEuOWwuNCA4M2MyLjU3IDIyLjkgMzMgMjAuOSAzNy42IDguMiA0LjY2LTEwLjEgMi41LTIxLjUgMi45OC0zMi4zdi01OC45aDIxLjlWNDQyem0yMS01NnYtMTcuOWg3My41VjM4NmgtMjUuNnY4MC40aDI1LjZ2MTguMUg0Mjh2LTE4LjFoMjUuNlYzODZ6bTE1NSA5OWwtMzcuNi04NC40VjQ4NWgtMTkuOVYzNjloMjYuOWwzNy40IDgzLjZjLjEtNiAuMDQtODMuNi4wNC04My42aDE5Ljl2MTE2em0xMTctMTBjLTI0LjMgMjAuOS02Ni43IDE2LjMtNzUuMS0yNC45LTExLjUtNjIuNiAyMi40LTk4LjYgNjkuNi03Ny42IDI5IDE2LjMgMjcuNCA4My40IDUuNSAxMDN6bS0yOS4zLTg4LjdjLTIwLjctLjU1LTIzLjcgMjAuNi0yNC41IDMwLjgtLjE3IDE5LjMtMS44OCA1MS40IDI0LjkgNTAgOC4zIDAgMTUuNS02LjAyIDE4LjItMTMuNyA1LjQ1LTEyLjEgOS41Ni02OC4xLTE4LjYtNjcuMXpNNjc2IDEzLjR2Mi4zNGg1LjE1djEzLjZoMi42NnYtMTMuNmg1LjJWMTMuNHptMTQuOCAwdjE1LjloMi40M1YxNS43bDQuMyAxMy42aDIuNGw0LjEtMTMuM3YxMy4zaDIuNFYxMy40aC0zLjhsLTMuOSAxMi43LTQtMTIuN3oiIGZpbGw9IiMwMDk3OWMiLz48dGl0bGU+d2lraTpCZWhuYW0gTjwvdGl0bGU+PC9zdmc+') center/contain no-repeat;        
		
    }

    .pcnt {
        position: absolute;
        top: 35%;
        left: 0;
        width: 100%;
        font-size: 2.5em;
        font-weight: bold;
        text-align: center;
        color: var(--accent-color);
        z-index: 100;
        text-shadow: 0 0 10px rgba(255, 255, 255, 0.8);
        animation: pulse 2s infinite;
    }

    .bowl {
        position: relative;
        border-radius: 50%;
        width: 150px;
        height: 150px;
        box-shadow: 
            inset 0 -5px 0 0 rgba(0, 0, 0, 0.5),
            inset 0 -20px 5px 0 rgba(0, 0, 0, 0.2),
            inset -15px 0 5px 0 rgba(0, 0, 0, 0.1),
            inset 15px 0 5px 0 rgba(0, 0, 0, 0.1),
            0 10px 20px rgba(0, 0, 0, 0.2);
        background: radial-gradient(ellipse at center, rgba(0, 0, 0, 0) 0%, rgba(0, 0, 0, 0) 76%, rgba(0, 0, 0, 0.65) 100%);
        margin: 20px;
        transform: scale(4);
        animation: float 3s ease-in-out infinite;
    }

    .bowl:before {
        overflow: hidden;
        border-radius: 50%;
        content: "";
        box-shadow: inset 0 -5px 0 0 rgba(0, 0, 0, 0.5),
                    inset 0 -20px 5px 0 rgba(0, 0, 0, 0.2),
                    inset -15px 0 5px 0 rgba(0, 0, 0, 0.1),
                    inset 15px 0 5px 0 rgba(0, 0, 0, 0.1);
        background: radial-gradient(ellipse at center, rgba(0, 0, 0, 0) 0%, rgba(0, 0, 0, 0) 60%, rgba(0, 0, 0, 0.65) 81%, black 100%);
        position: absolute;
        width: 150px;
        height: 150px;
        z-index: 2;
    }

    .bowl .inner {
        border-radius: 50%;
        width: 150px;
        height: 150px;
        background: radial-gradient(ellipse at center, rgba(0, 0, 0, 0) 0%, rgba(0, 0, 0, 0) 76%, rgba(0, 0, 0, 0.65) 100%);
        overflow: hidden;
        -webkit-backface-visibility: hidden;
        -webkit-transform: translate3d(0, 0, 0);
    }

    .bowl .fill {
        animation: fillAction 4s cubic-bezier(0.2, 0.6, 0.8, 0.4) forwards;
    }

    .bowl .waveShape {
        animation: waveAction 0.5s linear infinite;
        width: 300px;
        height: 150px;
        fill: var(--primary-color);
        opacity: 0.8;
    }

    .footer {
        position: absolute;
        bottom: 3vh;
        left: 0;
        width: 96vw;
        font-size: 1.5em;
        padding: 0 2vw;
        text-align: center;
        color: var(--accent-color);
        background: rgba(255, 255, 255, 0.9);
        border-radius: 10px;
        box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
        animation: slideUp 1s ease-out;
    }

    /* Animations */
    @keyframes fillAction {
        0% { transform: translate(0, 150px); }
        100% { transform: translate(0, var(--water-level)); }
    }

    @keyframes waveAction {
        0% { transform: translate(-150px, 0) rotateZ(0deg); }
        100% { transform: translate(0, 0) rotateZ(1deg); }
    }

    @keyframes float {
        0%, 100% { transform: scale(4) translateY(0); }
        50% { transform: scale(4) translateY(-10px); }
    }

    @keyframes pulse {
        0%, 100% { transform: scale(1); }
        50% { transform: scale(1.05); }
    }

    @keyframes rotate {
        from { transform: rotate(0deg); }
        to { transform: rotate(360deg); }
    }

    @keyframes fadeIn {
        from { opacity: 0; transform: translateY(-20px); }
        to { opacity: 1; transform: translateY(0); }
    }

    @keyframes slideUp {
        from { transform: translateY(100px); opacity: 0; }
        to { transform: translateY(0); opacity: 1; }
    }

    /* Background animation */
    .background {
        position: fixed;
        top: 0;
        left: 0;
        width: 100%;
        height: 100%;
        z-index: -1;
        background: linear-gradient(45deg, #f5f5f5, #e0f7fa);
        animation: gradientBG 10s ease infinite;
    }

    @keyframes gradientBG {
        0% { background-position: 0% 50%; }
        50% { background-position: 100% 50%; }
        100% { background-position: 0% 50%; }
    }

    /* Ripple effect for percentage display */
    .pcnt::after {
        content: '';
        position: absolute;
        top: 50%;
        left: 50%;
        width: 100px;
        height: 100px;
        background: rgba(3, 155, 228, 0.1);
        border-radius: 50%;
        transform: translate(-50%, -50%);
        animation: ripple 2s linear infinite;
    }

    @keyframes ripple {
        /* 0% { width: 0px; height: 0px; opacity: 0.5; }
        100% { width: 500px; height: 500px; opacity: 0; } */
        0% { width: 0px; height: 0px; opacity: 0.5; }
        100% { width: 100%; height: 100%; opacity: 0; }
    }
</style>
<body>
    <div class="background"></div>
    <div class="arduino-logo"></div>
    <div class="title">WATER TANK MONITOR</div>
    <div class="bowl">
        <div class="pcnt" id="ADCValue">---%</div>
        <div class="inner">
            <div class="fill">
                <svg version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px" width="300px" height="300px" viewBox="0 0 300 300" enable-background="new 0 0 300 300" xml:space="preserve">
                    <path class="waveShape" d="M300,300V2.5c0,0-0.6-0.1-1.1-0.1c0,0-25.5-2.3-40.5-2.4c-15,0-40.6,2.4-40.6,2.4
                    c-12.3,1.1-30.3,1.8-31.9,1.9c-2-0.1-19.7-0.8-32-1.9c0,0-25.8-2.3-40.8-2.4c-15,0-40.8,2.4-40.8,2.4c-12.3,1.1-30.4,1.8-32,1.9
                    c-2-0.1-20-0.8-32.2-1.9c0,0-3.1-0.3-8.1-0.7V300H300z" />
                </svg>
            </div>
        </div>
    </div>
    <div class="footer">
        <p>Water level in the tank is measured using an ultrasonic sensor. The sensor sends a sound wave and measures the time it takes for the sound wave to bounce back. The time is then converted to distance and displayed as the water level in the tank, based on the calibrated empty and full levels.</p>
    </div>

    <script>
        setInterval(function() { getData() }, 2000);

        function getData() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    let value = this.responseText;
                    document.getElementById("ADCValue").innerHTML = `${value}%`;
                    document.documentElement.style.setProperty('--water-level', `${parseInt(150 - parseInt(value, 10) * 1.5, 10)}px`);
                }
            };
            xhttp.open("GET", "readADC", true);
            xhttp.send();
        }
    </script>
</body>
</html>
)=====";