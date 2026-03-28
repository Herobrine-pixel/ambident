# AmbientSenseAI

**Version:** 1.1.0  
**Author:** Herobrine Sah (Herobrine Pixel)

AmbientSenseAI reads common environmental sensors and classifies the environment into human-readable **context labels** such as *Cooking*, *Sleeping*, *Fire Risk*.

This release includes built-in support for:
- DHT11 / DHT22 (temperature + humidity) — enabled by default (DHT library required)
- BH1750 (I²C lux) — default I²C pins A4 (SDA) / A5 (SCL) on Arduino UNO
- BMP280 (pressure + temperature)
- DS18B20 (1-Wire temperature probe)
- MQ-series gas sensors (e.g., MQ-2)
- Analog microphone modules (e.g., KY-037, MAX4466)

---

## Dependencies (automatically listed in library.properties)
- Adafruit Unified Sensor
- DHT sensor library
- BH1750
- BMP280 (Adafruit_BMP280)
- OneWire
- DallasTemperature

These libraries will be installed automatically by the Arduino Library Manager if you import AmbientSenseAI there.

---

## Wiring (default Arduino UNO)
### MQ-2 (Gas):
- VCC -> 5V
- GND -> GND
- AOUT -> A0

### Analog Microphone (KY-037 / MAX4466):
- VCC -> 5V (or 3.3V depending on module)
- GND -> GND
- OUT -> A2

### DHT11 / DHT22:
- VCC -> 5V
- GND -> GND
- DATA -> D2 (library default; change in begin() if needed)

### BH1750 (I²C):
- VCC -> 3.3V or 5V (check module)
- GND -> GND
- SDA -> A4 (SDA on UNO)
- SCL -> A5 (SCL on UNO)

### BMP280 (I²C):
- VCC -> 3.3V
- GND -> GND
- SDA -> A4
- SCL -> A5

### DS18B20 (1-Wire):
- DATA -> D3 (default)
- Pull-up resistor 4.7k between DATA and VCC
- VCC -> 5V
- GND -> GND

---

## WebApp
Folder `webapp/` contains a Web Serial-based dashboard (`index.html`, `app.js`, `style.css`).

Important notes:
- The webapp **does NOT require any CSS to function** — the JavaScript logic parses serial lines and displays context even if `style.css` is missing. CSS is **optional** and only changes appearance.
- A modern browser with Web Serial support (Chrome/Edge) is required. On desktop, Web Serial runs in secure contexts (https or localhost). On Android, use Chrome that supports Web Serial.
- Upload any example (e.g., `All_Sensors_Demo`) to your board, open the webapp, click Connect, and choose the Arduino port. The page reads lines like `Context: Cooking` and displays them live.
- CSS is included in `webapp/style.css` for a polished UI and dark mode — but again it is not required for functionality.

---

## Usage
Include the library in your sketch:

```cpp
#include <AmbientSenseAI.h>
AmbientSenseAI env;

void setup() {
  Serial.begin(9600);
  env.begin(); // defaults: gas A0, mic A2, dht D2, ds18b20 D3
}

void loop() {
  env.update();
  Serial.println(env.getContext());
  delay(1000);
}
```

---

## Notes and calibration
- MQ sensors require warm-up and calibration; treat thresholds as starting points.
- BH1750 gives lux values — use those for robust light decisions instead of raw LDR readings.
- DS18B20 is accurate for ambient temperatures; BMP280 provides pressure which can be used for altitude-based context.
- All examples compile on Arduino Uno; PlatformIO config included.

---

## License
MIT
