#include "AirQualitySensor.h"

AirQualitySensor::AirQualitySensor(uint8_t rxPin, uint8_t txPin) : _mySerial(rxPin, txPin) {
}

void AirQualitySensor::begin() {
  Serial.begin(9600); // Set the baud rate for the serial monitor
  _mySerial.begin(9600); // Set the baud rate for the software serial port
}

void AirQualitySensor::readData() {
  byte values[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79}; // Hexadecimal values to send

  Serial.print("Sending: ");
  for (int i = 0; i < sizeof(values); i++) {
    Serial.print("0x");
    if (values[i] < 0x10) {
      Serial.print("0"); // Print leading zero for single-digit values
    }
    Serial.print(values[i], HEX); // Print the hexadecimal value
    Serial.print(" ");
    _mySerial.write(values[i]); // Send the value to the software serial port
  }
  Serial.println();

  int receivedCount = 0;

  while (receivedCount < 26) {
    if (_mySerial.available()) {
      byte receivedValue = _mySerial.read(); // Read the byte from the software serial port
      _receivedValues[receivedCount] = receivedValue;
      receivedCount++;
    }
  }
}

int AirQualitySensor::getPM1() {
  return _receivedValues[2] * 256 + _receivedValues[3];
}

int AirQualitySensor::getPM2_5() {
  return _receivedValues[4] * 256 + _receivedValues[5];
}

int AirQualitySensor::getPM10() {
  return _receivedValues[6] * 256 + _receivedValues[7];
}

int AirQualitySensor::getCO2() {
  return _receivedValues[8] * 256 + _receivedValues[9];
}

float AirQualitySensor::getTemperature() {
  return ((_receivedValues[11] * 256 + _receivedValues[12]) - 500) * 0.1 + 7;
}

int AirQualitySensor::getHumidity() {
  return _receivedValues[13] * 256 + _receivedValues[14];
}

float AirQualitySensor::getCH2O() {
  return (_receivedValues[15] * 256 + _receivedValues[16]) * 0.001;
}

float AirQualitySensor::getCO() {
  return (_receivedValues[17] * 256 + _receivedValues[18]) * 0.1;
}

float AirQualitySensor::getO3() {
  return (_receivedValues[19] * 256 + _receivedValues[20]) * 0.01;
}

float AirQualitySensor::getNO2() {
  return (_receivedValues[21] * 256 + _receivedValues[22]) * 0.01;
}
