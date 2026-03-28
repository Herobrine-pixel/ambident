#include "AmbientSenseAI.h"
#include <Wire.h>

// DHT
#include <DHT.h>
#define DHTTYPE DHT22

// BH1750
#include <BH1750.h>

// BMP280 (Adafruit BMP280)
#include <Adafruit_BMP280.h>

// DS18B20
#include <OneWire.h>
#include <DallasTemperature.h>

// Globals for sensor objects
static DHT *dht = nullptr;
static BH1750 *lightSensor = nullptr;
static Adafruit_BMP280 *bmp = nullptr;
static OneWire *oneWire = nullptr;
static DallasTemperature *dsSensors = nullptr;

AmbientSenseAI::AmbientSenseAI() {
  _gasPin = A0;
  _micPin = A2;
  _dhtPin = 2;
  _oneWirePin = 3;
  _gasVal = _micVal = 0;
  _lux = 0;
  _temp = NAN; _hum = NAN;
  _bmpTemp = NAN; _bmpPress = NAN;
  _dsTemp = NAN;
  _context = "Unknown";
  _lastUpdate = 0;
  _hasDHT = _hasBH1750 = _hasBMP280 = _hasDS18B20 = false;
}

void AmbientSenseAI::begin(uint8_t gasPin, uint8_t micPin, uint8_t dhtPin, uint8_t oneWirePin) {
  _gasPin = gasPin;
  _micPin = micPin;
  _dhtPin = dhtPin;
  _oneWirePin = oneWirePin;

  pinMode(_gasPin, INPUT);
  pinMode(_micPin, INPUT);

  Wire.begin(); // Use default I2C pins (A4 SDA, A5 SCL on UNO)

  // Initialize DHT
  dht = new DHT(_dhtPin, DHTTYPE);
  dht->begin();
  _hasDHT = true;

  // Initialize BH1750
  lightSensor = new BH1750();
  if (lightSensor->begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    _hasBH1750 = true;
  } else {
    _hasBH1750 = false;
  }

  // Initialize BMP280
  bmp = new Adafruit_BMP280();
  if (bmp->begin()) {
    _hasBMP280 = true;
  } else {
    _hasBMP280 = false;
  }

  // Initialize DS18B20
  oneWire = new OneWire(_oneWirePin);
  dsSensors = new DallasTemperature(oneWire);
  dsSensors->begin();
  #if defined(__AVR__)
  if (dsSensors->getDeviceCount() > 0) _hasDS18B20 = true;
  else _hasDS18B20 = false;
  #else
  // For other platforms, still mark as available if library loaded
  _hasDS18B20 = (dsSensors->getDeviceCount() > 0);
  #endif

  _detectSensors();
  update();
}

void AmbientSenseAI::_detectSensors() {
  delay(50);
  _gasVal = analogRead(_gasPin);
  _micVal = analogRead(_micPin);
  if (_hasBH1750) {
    _lux = lightSensor->readLightLevel();
  }
  if (_hasDHT) {
    float t = dht->readTemperature();
    float h = dht->readHumidity();
    if (!isnan(t)) _temp = t;
    if (!isnan(h)) _hum = h;
  }
  if (_hasBMP280) {
    _bmpTemp = bmp->readTemperature();
    _bmpPress = bmp->readPressure() / 100.0F;
  }
  if (_hasDS18B20) {
    dsSensors->requestTemperatures();
    _dsTemp = dsSensors->getTempCByIndex(0);
  }
}

void AmbientSenseAI::update() {
  _gasVal = analogRead(_gasPin);
  _micVal = analogRead(_micPin);
  if (_hasBH1750) {
    _lux = lightSensor->readLightLevel();
  }
  if (_hasDHT) {
    float t = dht->readTemperature();
    float h = dht->readHumidity();
    if (!isnan(t)) _temp = t;
    if (!isnan(h)) _hum = h;
  }
  if (_hasBMP280) {
    _bmpTemp = bmp->readTemperature();
    _bmpPress = bmp->readPressure() / 100.0F;
  }
  if (_hasDS18B20) {
    dsSensors->requestTemperatures();
    _dsTemp = dsSensors->getTempCByIndex(0);
  }
  _lastUpdate = millis();
  _classify();
}

void AmbientSenseAI::_classify() {
  String result = "Unknown";

  bool gasHigh = _gasVal > 700;
  bool gasMed = (_gasVal > 400 && _gasVal <= 700);
  bool bright = (_hasBH1750 ? _lux > 600 : analogRead(A1) > 600);
  bool dark = (_hasBH1750 ? _lux < 200 : analogRead(A1) < 200);
  bool loud = _micVal > 600;
  bool noisy = _micVal > 400 && _micVal <= 600;

  // Use DHT/BMP/DS18B20 for temp/humidity if available
  float tempBest = NAN;
  if (_hasDS18B20) tempBest = _dsTemp;
  else if (_hasBMP280) tempBest = _bmpTemp;
  else if (_hasDHT) tempBest = _temp;

  float humBest = NAN;
  if (_hasDHT) humBest = _hum;

  if (gasHigh && (!isnan(tempBest) && tempBest > 55)) {
    result = "Fire Risk";
  } else if (gasMed && loud && bright) {
    result = "Cooking";
  } else if (!loud && dark) {
    result = "Sleeping";
  } else if (noisy && bright) {
    result = "Conversation";
  } else if (loud && !bright) {
    result = "Pet / Noise Spike";
  } else if (!isnan(humBest) && humBest > 85 && dark) {
    result = "Raining Indoors / High Humidity";
  } else if (!isnan(tempBest) && tempBest > 35) {
    result = "Hot Room";
  } else if (!isnan(tempBest) && tempBest < 10) {
    result = "Cold Room";
  } else if (bright && !loud) result = "Idle / Lights On";
  else if (!bright && !loud) result = "Quiet / Dim";
  else result = "Unknown";

  _context = result;
}

String AmbientSenseAI::getContext() {
  if (millis() - _lastUpdate > 2000) update();
  return _context;
}

bool AmbientSenseAI::triggered(const String &event) {
  return _context.equalsIgnoreCase(event);
}

int AmbientSenseAI::gasRaw() { return _gasVal; }
int AmbientSenseAI::micRaw() { return _micVal; }
unsigned long AmbientSenseAI::lux() { return _lux; }
float AmbientSenseAI::temperature() { return _temp; }
float AmbientSenseAI::humidity() { return _hum; }
float AmbientSenseAI::bmpTemperature() { return _bmpTemp; }
float AmbientSenseAI::bmpPressure() { return _bmpPress; }
float AmbientSenseAI::ds18b20Temp() { return _dsTemp; }
