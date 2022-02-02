#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESPTelnet.h>
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(0); //Czujnik na GPIO01
DallasTemperature sensors(&oneWire);
DeviceAddress deviceAddress;
ESP8266WebServer server(80);
ESPTelnet telnet;
WiFiClient client;


//<<Konfiguracja ESP>>
const char* host_name = "esp0101";
int timer_delay = 6000; //600=1min
String apiKey = "2MDS8ZUTERU0IG3J";
uint8_t sensor1[8] = { 0x28, 0x9B, 0x44, 0x48, 0xF6, 0x0D, 0x3C, 0x95 };
uint8_t sensor2[8] = { 0x28, 0x0B, 0x6B, 0x48, 0xF6, 0xC7, 0x3C, 0xE0 };
int timer = 0;
int numberOfDevices = 0;
//<<Konfiguracja ESP>>


void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin("MSHOME", "1234567890");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname(host_name);
  ArduinoOTA.setPassword("812daniel");
  ArduinoOTA.begin();
  server.on("/", handleRootPath);
  server.begin();
  sensors.begin();
  numberOfDevices = sensors.getDeviceCount();
  pinMode(1, OUTPUT); //Trig PIN
  pinMode(3, INPUT); //Echo PIN
  telnet.begin();
  telnet.onConnect(onTelnetConnect);
  telnet.onInputReceived([](String str) {
    if (str == "ping") {
      telnet.println("> pong");
    }
    if (str == "uptime") {
      telnet.println("> Device up-time:");
      telnet.println(getDeviceUpTime());
    }
    if (str == "sensors") {
      telnet.println("> List of 1-wire device addresses:");
      sensors.requestTemperatures();
      for (int i = 0;  i < numberOfDevices;  i++) {
        sensors.getAddress(deviceAddress, i);
        telnet.println(getSensorAddress(deviceAddress));
        telnet.println(getTemperature(deviceAddress));
      }
    }
    if (str == "si") {
        telnet.print("> ");
        telnet.println(String(getDistance()));
    }
    if (str == "so") {
      for (int i = 0; i <= 30; i++) {
        telnet.print("> ");
        telnet.println(String(getDistance()));
        delay(500);
      }
    }
  });
}


void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  telnet.loop();
  if (timer == 0) {
    sensors.requestTemperatures();
    if (client.connect("api.thingspeak.com",80)) {
      String postStr = apiKey;
      postStr +="&field3=";
      postStr += String(getTemperature(sensor1));
      postStr +="&field4=";
      postStr += String(getTemperature(sensor2));
      postStr +="&field5=";
      postStr += String(getDistance());
      postStr += "\r\n\r\n";
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);
    }
    client.stop();
  }
  timer++;
  if (timer == timer_delay) timer = 0;
  delay(100);
}


float getDistance() {
  long duration;
  float distance;
  for (int i = 0; i <= 9; i++) {
    digitalWrite(1, LOW);
    delayMicroseconds(2);
    digitalWrite(1, HIGH);
    delayMicroseconds(10);
    digitalWrite(1, LOW);
    duration = pulseIn(3, HIGH);
    distance += duration * 0.034/2;
    delay(80);
  }
  distance = distance/10;
  return distance;
}


String getWiFiSignalInfo() {
  long rssi = WiFi.RSSI();
  if (rssi > -50) return "5/5";
  if (rssi <= -50 && rssi > -67) return "4/5";
  if (rssi <= -67 && rssi > -70) return "3/5";
  if (rssi <= -70 && rssi > -80) return "2/5";
  if (rssi <= -80) return "1/5";
}


void handleRootPath() {
  telnet.println("WWW Client connected!");
  String htmlbody = "<h2>Host name: " + String(host_name) + "</h2>";
  htmlbody +=       "<p>App version: " + String(__FILE__) + "</p>";
  htmlbody +=       "<p>Build date: " + String(__DATE__) + "</p>";
  htmlbody +=       "<p>WiFi signal: " + getWiFiSignalInfo() + "</p>";
  htmlbody +=       "<p>Device up-time: " + getDeviceUpTime() + "</p>";
  server.send(200, "text/html", htmlbody);
}

String getDeviceUpTime() {
  String result = "";
  unsigned long currentMillis = millis();
  unsigned long seconds = currentMillis / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  currentMillis %= 1000;
  seconds %= 60;
  minutes %= 60;
  hours %= 24;
  result += String(days) + " days " + String(hours) + " hours " + String(minutes) + " minutes " + String(seconds) + " seconds.";
  return result;
}


void onTelnetConnect(String ip) {
  telnet.println("Welcome on ESPTelnet server.");
  telnet.println("App version: " + String(__FILE__));
  telnet.println("Build date: " + String(__DATE__));
}


String getTemperature(DeviceAddress deviceAddress) {
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127) {
    tempC = 0;
  }
  return String(tempC);
}


String getSensorAddress(DeviceAddress deviceAddress) {
  String tempStr = "";
  for (uint8_t i = 0; i < 8; i++) {
    tempStr += "0x";
    if (deviceAddress[i] < 0x10) tempStr += "0";
    tempStr += String(deviceAddress[i], HEX);
    if (i < 7) tempStr += ", ";
  }
  return tempStr;
}
