# Water Tank Monitor — Arduino Nano ESP32

An IoT water-level monitoring system built around the **Arduino Nano ESP32** and an **HC-SR04 ultrasonic sensor**. It measures the water level in a tank, displays it on a 16×2 I²C LCD, serves a live animated dashboard over Wi-Fi, and automatically controls a pump with audible/visual alerts.

---

## Features

- 📏 **Non-contact level sensing** with an ultrasonic distance sensor
- 🖥️ **Local LCD display** showing the current water level as a percentage
- 🌐 **Built-in Wi-Fi web dashboard** with an animated wave visualization that auto-refreshes every 2 seconds
- 🚰 **Automatic pump control** — turns ON when the tank is empty, OFF when full
- 🔔 **Buzzer alarm** and blinking red LED when the tank runs dry
- 🟢 **Status LEDs**: Green (full), Yellow (pump running), Red (empty)
- ⚙️ **Two-point calibration** (empty / full levels) saved to EEPROM and preserved across reboots
- 🛡️ **Misreading filter** — rejects implausible jumps (level can only rise while the pump runs, and only fall while it's off)
- 🔄 **Reset button** to wipe calibration and re-calibrate at any time

---

## Hardware Requirements

| Component | Qty | Notes |
|---|---|---|
| Arduino Nano ESP32 | 1 | Any ESP32 board works with minor pin tweaks |
| HC-SR04 Ultrasonic Sensor | 1 | Mounted at the top of the tank, facing down |
| 16×2 I²C LCD Display | 1 | PCF8574 backpack, default address `0x27` |
| Relay Module (5 V) | 1 | To switch the pump safely |
| Water Pump | 1 | Wired through the relay |
| Buzzer (active) | 1 | |
| LEDs (Red, Yellow, Green) | 1 each | with 220 Ω current-limiting resistors |
| Push Buttons | 2 | One for calibration, one for reset |
| Jumper wires, breadboard, 5 V PSU | — | |

> ⚠️ **Warning:** The pump is mains/high-current. Use a properly rated relay and observe electrical safety. Never wire mains directly to the Nano.

---

## Pin Connections

| Function | Nano ESP32 Pin |
|---|---|
| Ultrasonic TRIG | D3 |
| Ultrasonic ECHO | D4 |
| Red LED | D2 |
| Yellow LED | D5 |
| Green LED | D12 |
| Pump (via relay) | D7 |
| Buzzer | D10 |
| Calibration / Reset-Calibration Button | D11 |
| Reset Button | D9 |
| LCD SDA / SCL | A4 / A5 (default I²C) |

---

## Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software) 2.x
- **Arduino ESP32 Boards** package (install via Boards Manager → search "esp32" by Espressif Systems)
- Libraries (install via Library Manager):
  - `WiFi` *(bundled with ESP32 core)*
  - `WebServer` *(bundled with ESP32 core)*
  - `EEPROM` *(bundled with ESP32 core)*
  - `LiquidCrystal_I2C` by Frank de Brabander (or compatible fork)

---

## Installation

1. **Clone or download** this repository.
2. Open `water_level_monitor2.ino` in the Arduino IDE — keep `waterlevel.h` in the same folder.
3. In the IDE, select **Tools → Board → Arduino Nano ESP32**.
4. Update your Wi-Fi credentials at the top of the sketch:
   ```cpp
   const char* ssid     = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
5. Connect the board via USB, pick the correct port, and click **Upload**.
6. Open the **Serial Monitor** at **9600 baud** to see the assigned IP address.

---

## Calibration

On first boot — or after pressing the reset-calibration button — the device enters **calibration mode**. The LCD will guide you through two steps:

1. **Set Empty Level**
   - Make sure the tank is empty (or hold the sensor at the "empty" reference distance).
   - LCD shows: `PUSH BUTTON TO SET EMPTY LEVEL`.
   - Press the **calibration button** (D11). The current sensor reading is saved to EEPROM address `0`.

2. **Set Full Level**
   - Fill the tank (or hold the sensor at the "full" reference distance).
   - LCD shows: `PUSH BUTTON TO SET FULL LEVEL`.
   - Press the button again. The reading is saved to EEPROM address `1`.

The device then switches to normal monitoring mode. Calibration is retained across power cycles. To re-calibrate, press the button during normal operation — it wipes EEPROM and restarts the calibration flow.

---

## Usage

Once calibrated, the system runs autonomously:

- The **LCD** displays the current water level as a percentage.
- The **web dashboard** is reachable at the IP shown on the Serial Monitor (e.g. `http://192.168.1.42/`) from any device on the same Wi-Fi network.
- **Pump logic:**
  - At **0 %** → pump turns **ON**, yellow LED on, red LED + buzzer pulse.
  - Pump runs until the level reaches **100 %**, then turns **OFF** and the green LED lights up.

### Web Dashboard

The dashboard shows an animated water-filled bowl with a live percentage that polls the `/readADC` endpoint every 2 seconds. The wave height updates smoothly with the measured level. A short explainer of the sensing method is shown at the bottom.

---

## How It Works

1. **Distance measurement** — The TRIG pin pulses HIGH for 10 µs. The ECHO pin's HIGH duration is converted to distance:
   `distance_cm ≈ 0.017 × duration_µs`
2. **Percentage calculation** — Linearly mapped between the calibrated `empty_level` and `full_level` distances.
3. **Misreading filter** — Because ultrasonic sensors can produce occasional spikes, the firmware enforces physical plausibility:
   - While the pump is **ON**, the level can only **rise**.
   - While the pump is **OFF**, the level can only **fall**.
4. **Web server** — A lightweight `WebServer` on port 80 serves the HTML page from PROGMEM (`waterlevel.h`) and answers AJAX polls with the current percentage.

---

## Project Structure

```
.
├── water_level_monitor2.ino   # Main Arduino sketch (logic, sensors, Wi-Fi)
├── waterlevel.h               # HTML/CSS/JS dashboard served by the ESP32
└── README.md                  # This file
```

---

## Troubleshooting

| Symptom | Likely Cause / Fix |
|---|---|
| LCD shows nothing or random blocks | Wrong I²C address — try `0x3F` instead of `0x27`. Check SDA/SCL wiring. |
| Stuck on "Connecting to WiFi..." | Check SSID/password. The Nano ESP32 only supports 2.4 GHz Wi-Fi. |
| Percentage always shows `---%` on the web page | Browser can't reach the device — confirm the IP, same network, and no firewall blocking port 80. |
| Distance reading is `0` or wildly unstable | Ultrasonic sensor needs **5 V** — if powered from 3.3 V it may misbehave. Also check TRIG/ECHO wiring. |
| Pump won't stop / won't start | Verify the relay's logic level (active-LOW vs active-HIGH) and re-run calibration. |
| Calibration won't save | EEPROM commit can fail on first run — re-flash and try again later. |

---

## Possible Improvements

- Move Wi-Fi credentials to a configuration file or WiFiManager captive portal
- Add MQTT or HTTP-push notifications when the tank goes empty/full
- Log historical levels to an SD card or cloud service
- Add OTA firmware updates
- Replace blocking `delay()` calls with a non-blocking scheduler

---

## License

Released under the MIT License — feel free to use, modify, and share.
