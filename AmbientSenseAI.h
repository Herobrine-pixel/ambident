#pragma once
#include <Arduino.h>

#ifndef AMBIENTSENSEAI_VERSION
#define AMBIENTSENSEAI_VERSION "1.1.0"
#endif

// Optional libraries (headers included in .cpp where needed)
class AmbientSenseAI {
  public:
    AmbientSenseAI();

    // begin: pins optional. For BH1750 (I2C) defaults to Wire on A4/A5 for UNO.
    void begin(uint8_t gasPin = A0, uint8_t micPin = A2, uint8_t dhtPin = 2, uint8_t oneWirePin = 3);

    // update reads all available sensors and classifies
    void update();

    // get current label
    String getContext();

    // check event
    bool triggered(const String &event);

    // Raw accessors
    int gasRaw();
    int micRaw();
    unsigned long lux(); // BH1750
    float temperature(); // from best available source
    float humidity(); // from DHT if available
    float bmpTemperature();
    float bmpPressure();
    float ds18b20Temp();

  private:
    uint8_t _gasPin, _micPin, _dhtPin, _oneWirePin;
    int _gasVal, _micVal;
    unsigned long _lux;
    float _temp, _hum;
    float _bmpTemp, _bmpPress;
    float _dsTemp;
    String _context;
    unsigned long _lastUpdate;

    // availability flags
    bool _hasDHT, _hasBH1750, _hasBMP280, _hasDS18B20;

    void _detectSensors();
    void _readSensors();
    void _classify();
};
