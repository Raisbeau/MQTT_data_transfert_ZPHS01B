#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include <PubSubClient.h>
#include <AirQualitySensor.h>
const char* ssid       = "your wifi ssid";
const char* password   = "your wifi password";
///// MQTT
const char* mqttServer = "public.mqtthq.com";
const char* mqttTopic = "airquality";
const int mqttPort = 1883; // MQTT default port
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
////////
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
unsigned long p_millis = 0;
unsigned long mqtt_millis = 0;
const int logPeriode=1000;
AirQualitySensor airSensor(13, 12); // RX, TX pins for the software serial port
String getLocalTime();

void reconnect();

void setup()
{
  Serial.begin(115200);
  airSensor.begin();
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      if (millis()-p_millis>=logPeriode*60){
        p_millis=millis();
        ESP.restart();
      }
  }
  p_millis=0;
  Serial.println("CONNECTED");
    // Set MQTT server and callback function
  mqttClient.setServer(mqttServer, mqttPort);
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  getLocalTime();
  //disconnect WiFi as it's no longer needed
  //WiFi.disconnect(true);
  //WiFi.mode(WIFI_OFF);
}

void loop()
{
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();

  if(millis()-p_millis>=logPeriode*60){
  p_millis=millis();
  String logTime = getLocalTime();
  
  airSensor.readData();

 String payload = logTime+";"+"Temperature;"+String(airSensor.getTemperature())+";°C;"+"Humidity;"+String(airSensor.getHumidity())+";%;";
payload+="PM1;"+String(airSensor.getPM1())+";ug/m3"+";PM2.5;"+String(airSensor.getPM2_5())+";ug/m3"+";PM10;"+String(airSensor.getPM10())+";ug/m3"+";CO2;"+String(airSensor.getCO2())+";ppm"+";CO;"+String(airSensor.getCO())+";ppm";
payload+=";CH2O;"+String(airSensor.getCH2O())+";mg/m3"+";O3;"+String(airSensor.getO3())+";ppm"+";NO2;"+String(airSensor.getNO2())+";ppm";
  // Send data to MQTT topic
  Serial.println(payload);
  mqttClient.publish(mqttTopic, payload.c_str());
  }
}

String getLocalTime()
{
  struct tm timeinfo;
  while(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    //return;
  }
  String nowDate = String(timeinfo.tm_mday)+"/"+String(timeinfo.tm_mon+1)+"/"+String(1900+timeinfo.tm_year)+" " +String(timeinfo.tm_hour)+":"+String(timeinfo.tm_min)+":"+String(timeinfo.tm_sec);
  return nowDate;
}

void reconnect() {
  // Loop until connected to MQTT broker
  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (mqttClient.connect("ArduinoClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      if ((millis()-mqtt_millis>=logPeriode)*60){
        ESP.restart();
      }
    }
  }
}