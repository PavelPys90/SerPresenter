# SerPresenter

A WiFi-based presentation remote built with an ESP8266 D1 mini. Control PDF slides on your laptop from your smartphone — no extra apps needed, just a browser.

## How it works

```
Smartphone ──WiFi──> D1 mini ──USB/Serial──> Laptop
   (browser)          (AP + web server)       (Python host)
```

- The D1 mini creates a WiFi access point and serves a web page with **Previous** and **Next** buttons.
- Your phone connects to that WiFi and opens the page.
- Tapping a button triggers an HTTP request to the D1 mini, which sends a command over USB serial.
- A Python script on the laptop reads the serial port and simulates **Left** / **Right** arrow key presses via `xdotool`.

## Hardware

- [Wemos D1 mini](https://www.wemos.cc/en/latest/d1/d1_mini.html) (ESP8266)
- Micro-USB cable

## Setup

### 1. Flash the firmware

```bash
platformio run --target upload
```

### 2. Install host dependencies

```bash
# Python serial library
pip install pyserial

# Key simulation (Linux / X11)
sudo apt install xdotool
```

### 3. Run the host

```bash
./presenter_host.py
```

It auto-detects the D1 mini by its USB chip (CH340). If detection fails, specify the port manually:

```bash
./presenter_host.py --port /dev/ttyUSB0
```

## Usage

1. Plug the D1 mini into your laptop's USB port.
2. Start the host script.
3. On your phone, connect to WiFi **Presenter** (password: `12345678`).
4. Open a browser and go to **http://192.168.4.1**.
5. Tap **◀** or **▶** to navigate slides.

Works with any PDF viewer or presentation app that responds to Left/Right arrow keys (Firefox, Chromium, Okular, LibreOffice Impress, etc.).

## Customization

Edit `src/main.cpp` to change the WiFi credentials:

```cpp
const char *ssid = "Presenter";
const char *password = "12345678";
```

Then rebuild and re-flash.

## Host options

| Flag | Description |
|---|---|
| `--port /dev/ttyUSB0` | Use a specific serial port |
| `--baudrate 115200` | Set baud rate (default: 115200) |
| `--list` | List available serial ports |
| `--dry-run` | Print commands without simulating keys |

## License

MIT
