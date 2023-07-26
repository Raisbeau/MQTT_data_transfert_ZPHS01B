#ifndef AirQualitySensor_h
#define AirQualitySensor_h

#include <Arduino.h>
#include <SoftwareSerial.h>

class AirQualitySensor {
public:
  AirQualitySensor(uint8_t rxPin, uint8_t txPin);

  void begin();
  void readData();
  int getPM1();
  int getPM2_5();
  int getPM10();
  int getCO2();
  float getTemperature();
  int getHumidity();
  float getCH2O();
  float getCO();
  float getO3();
  float getNO2();

private:
  SoftwareSerial _mySerial;
  byte _receivedValues[26];
  byte _values[9];

  byte calculateChecksum();
};

#endif
