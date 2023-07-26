#include <Arduino.h>
#include <AirQualitySensor.h>

AirQualitySensor airSensor(10, 11); // RX, TX pins for the software serial port

void setup() {
  airSensor.begin();
}

void loop() {
  airSensor.readData();

  // Print interpreted values
  Serial.print("PM1.0: ");
  Serial.print(airSensor.getPM1());
  Serial.println(" ug/m3");

  Serial.print("PM2.5: ");
  Serial.print(airSensor.getPM2_5());
  Serial.println(" ug/m3");

  Serial.print("PM10: ");
  Serial.print(airSensor.getPM10());
  Serial.println(" ug/m3");

  Serial.print("CO2: ");
  Serial.print(airSensor.getCO2());
  Serial.println(" ppm");

  Serial.print("Temperature: ");
  Serial.print(airSensor.getTemperature());
  Serial.println(" ℃");

  Serial.print("Humidity: ");
  Serial.print(airSensor.getHumidity());
  Serial.println("%RH");

  Serial.print("CH2O: ");
  Serial.print(airSensor.getCH2O());
  Serial.println(" mg/m3");

  Serial.print("CO: ");
  Serial.print(airSensor.getCO());
  Serial.println(" ppm");

  Serial.print("O3: ");
  Serial.print(airSensor.getO3());
  Serial.println(" ppm");

  Serial.print("NO2: ");
  Serial.print(airSensor.getNO2());
  Serial.println(" ppm");

  delay(20000); // Delay for better readability
}
