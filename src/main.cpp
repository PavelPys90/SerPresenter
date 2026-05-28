#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const char *ssid = "Presenter";
const char *password = "12345678";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
<title>SerPresenter</title>
<style>
  * { margin: 0; padding: 0; box-sizing: border-box; }
  html, body { height: 100%; }
  body {
    display: flex;
    flex-direction: column;
    background: #1a1a1a;
    font-family: -apple-system, sans-serif;
    -webkit-tap-highlight-color: transparent;
    user-select: none;
    -webkit-user-select: none;
  }
  header {
    padding: 12px 16px;
    text-align: center;
    color: #888;
    font-size: 14px;
    letter-spacing: 1px;
    border-bottom: 1px solid #333;
  }
  header span { color: #555; font-size: 11px; }
  .btn {
    flex: 1;
    display: flex;
    align-items: center;
    justify-content: center;
    font-size: 18vw;
    color: #fff;
    border: none;
    cursor: pointer;
    transition: background 0.1s;
    -webkit-appearance: none;
    outline: none;
    touch-action: manipulation;
  }
  .btn:active { filter: brightness(1.3); }
  .prev { background: #2d2d2d; }
  .next { background: #3a3a3a; }
  .status {
    position: fixed; top: 8px; left: 50%; transform: translateX(-50%);
    color: #555; font-size: 12px; pointer-events: none;
  }
</style>
</head>
<body>
<header>SerPresenter <span>v0.1.0</span></header>
<button class="btn prev" ontouchstart="send('prev')" onmousedown="send('prev')">&#9664;</button>
<button class="btn next" ontouchstart="send('next')" onmousedown="send('next')">&#9654;</button>
<div class="status" id="status"></div>
<script>
  let busy = false;
  function send(dir) {
    if (busy) return;
    busy = true;
    const s = document.getElementById('status');
    s.textContent = dir === 'next' ? 'next ▶' : '◀ prev';
    fetch('/' + dir).then(() => {
      s.textContent = 'ok';
      setTimeout(() => { s.textContent = ''; busy = false; }, 200);
    }).catch(() => {
      s.textContent = 'err';
      setTimeout(() => { s.textContent = ''; busy = false; }, 300);
    });
  }
</script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleNext() {
  Serial.println("NEXT");
  server.send(200, "text/plain", "NEXT");
}

void handlePrev() {
  Serial.println("PREV");
  server.send(200, "text/plain", "PREV");
}

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  server.on("/", handleRoot);
  server.on("/next", handleNext);
  server.on("/prev", handlePrev);
  server.begin();
}

void loop() {
  server.handleClient();
}
