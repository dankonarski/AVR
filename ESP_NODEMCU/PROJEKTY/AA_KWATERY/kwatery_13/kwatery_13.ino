#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESPTelnet.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(14);
OneWire oneWire2(13);
DallasTemperature sensors(&oneWire);
DallasTemperature sensors2(&oneWire2);
ESP8266WebServer server(80);
WiFiClient client;
ESPTelnet telnet;


String app_version = "1.1"; //Wersja firmware
int numberOfDevices = 0;
int lcdclock = 0;  //zegar dla wyswietlania na LCD
int tryclock = 0; //zegar dla ponowien LCD
int tsclock = 0; //zegar dla thingspeak
uint8_t sensor1[8] = { 0x28, 0x09, 0xBA, 0x48, 0xF6, 0xD9, 0x3C, 0x2D }; //MIESZKANIE 1
uint8_t sensor2[8] = { 0x28, 0x28, 0x42, 0x48, 0xF6, 0xEA, 0x3C, 0x4C }; //MIESZKANIE 2
uint8_t sensor3[8] = { 0x28, 0x9D, 0x58, 0x48, 0xF6, 0x46, 0x3C, 0xC2 }; //MIESZKANIE 3
uint8_t sensor4[8] = { 0x28, 0x9A, 0xAF, 0x48, 0xF6, 0x33, 0x3C, 0xF9 }; //MIESZKANIE 4
uint8_t sensor5[8] = { 0x28, 0x9F, 0xD4, 0x48, 0xF6, 0x9F, 0x3C, 0xAC }; //MIESZKANIE 5
uint8_t sensor6[8] = { 0x28, 0xDC, 0xD7, 0x48, 0xF6, 0x26, 0x3C, 0x7E }; //CENTRALNE
String apiKey = "Z5T7JPHT2RJQ82G8";
const char *ssid = "MSHOME";
const char *pass = "1234567890";
const char* tsserver = "api.thingspeak.com";


void setup() {
  Wire.begin(D2, D1);
  lcd.init();
  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TempKwatery");
  lcd.setCursor(13, 0);
  lcd.print(app_version);
  lcd.setCursor(0, 1);
  lcd.print("Ladowanie...");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin("MSHOME", "1234567890");
  while (WiFi.status() != WL_CONNECTED){ delay(500);};
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  delay(100);
  
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname("esp1201");
  ArduinoOTA.setPassword("812daniel");
  ArduinoOTA.begin();
  delay(100);
  
  server.on("/", handleRootPath);
  server.begin();
  telnet.begin();
  delay(100);
  
  sensors.begin();
  sensors2.begin();
  numberOfDevices = sensors.getDeviceCount();
  numberOfDevices += sensors2.getDeviceCount();
  delay(100);
  pinMode(12, OUTPUT); //Trig PIN
  pinMode(16, INPUT); //Echo PIN
  pinMode(15, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(15), ISR, FALLING);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.backlight();
  lcd.print("Czujniki: ");
  lcd.print(numberOfDevices);
  lcd.setCursor(0, 1);
  lcd.print("WiFi: ");
  lcd.print(getWiFiSignalInfo());
  delay(3000);
}


void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  telnet.loop();

  LCDPrintTemp(lcdclock);
  if (lcdclock >= 0 && lcdclock <= 150) { lcdclock++;}
  if (lcdclock == 150) {
    lcdclock = 0;
    tryclock++;
    }
  if (tryclock == 3) {
    lcd.clear();
    lcd.noBacklight();
    lcdclock = 151;
  }

  if (tsclock == 0) {TSTempUpload();}
  tsclock++;
  if (tsclock == 6000) {tsclock = 0;}
  delay(100);
}

ICACHE_RAM_ATTR void ISR() {
  lcd.clear();
  lcd.backlight();
  tryclock = 0;
  lcdclock = 0;
}



void LCDPrintTemp(int dclock) {
  if (dclock == 0) {
    sensors2.requestTemperatures();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Miesz. 1: ");
    lcd.print(printTemperature2(sensor1),1);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Miesz. 2: ");
    lcd.print(printTemperature2(sensor2),1);
    lcd.print((char)223);
    lcd.print("C");
  }
  if (dclock == 50) {
    sensors.requestTemperatures();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Miesz. 3: ");
    lcd.print(printTemperature(sensor3),1);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Miesz. 4: ");
    lcd.print(printTemperature(sensor4),1);
    lcd.print((char)223);
    lcd.print("C");
  }
  if (dclock == 100) {
    sensors.requestTemperatures();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Miesz. 5: ");
    lcd.print(printTemperature(sensor5),1);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Centr.  : ");
    lcd.print(printTemperature(sensor6),1);
    lcd.print((char)223);
    lcd.print("C");
  }
}

void TSTempUpload () {
  if (client.connect(tsserver,80)) {
    sensors.requestTemperatures();
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(printTemperature2(sensor1));
    postStr +="&field2=";
    postStr += String(printTemperature2(sensor2));
    postStr +="&field3=";
    postStr += String(printTemperature(sensor3));
    postStr +="&field4=";
    postStr += String(printTemperature(sensor4));
    postStr +="&field5=";
    postStr += String(printTemperature(sensor5));
    postStr +="&field6=";
    postStr += String(printTemperature(sensor6));
    postStr +="&field7=";
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
    client.stop();
  }
}

void handleRootPath() {
  telnet.println("WWW Client connected!");
  String htmlbody = "<h2>ESP1201</h2>";
  htmlbody += "<p>WiFi signal strength: ";
  htmlbody += getWiFiSignalInfo();
  htmlbody += "</p>";
  htmlbody += "<p>Sensors: ";
  htmlbody += numberOfDevices;
  htmlbody += "</p>";
  htmlbody += "<p>Device Up Time: ";
  htmlbody += DeviceUpTime();
  htmlbody += "s</p>";
  server.send(200, "text/html", htmlbody);
}


float printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127) {
    tempC = 0;
  }
  return tempC;
}


float printTemperature2(DeviceAddress deviceAddress2)
{
  float tempC = sensors2.getTempC(deviceAddress2);
  if (tempC == -127) {
    tempC = 0;
  }
  return tempC;
}


String getWiFiSignalInfo()  {
  long rssi = WiFi.RSSI();
  if (rssi > -50) {
    return "5/5";
  }
  else if (rssi <= -50 && rssi > -67) {
    return "4/5";
  }
  else if (rssi <= -67 && rssi > -70) {
    return "3/5";
  }
  else if (rssi <= -70 && rssi > -80) {
    return "2/5";
  }
  else if (rssi <= -80) {
    return "1/5";
  }
}


String DeviceUpTime() {
  long unsigned uptime = millis();
  uptime = uptime/1000;
  return String(uptime);
}


float getDistance() {
  long duration;
  float distance;
  for (int i = 0; i <= 9; i++) {
    digitalWrite(12, LOW);
    delayMicroseconds(2);
    digitalWrite(12, HIGH);
    delayMicroseconds(10);
    digitalWrite(12, LOW);
    duration = pulseIn(16, HIGH);
    distance += duration * 0.034/2;
    delay(80);
  }
  distance = distance/10;
  if (distance > 60) return 0;
  if (distance <= 60 && distance > 55) return 10;
  if (distance <= 55 && distance > 50) return 20;
  if (distance <= 50 && distance > 45) return 30;
  if (distance <= 45 && distance > 40) return 40;
  if (distance <= 40 && distance > 35) return 50;
  if (distance <= 35 && distance > 30) return 60;
  if (distance <= 30 && distance > 25) return 70;
  if (distance <= 25 && distance > 20) return 80;
  if (distance <= 20 && distance > 15) return 90;
  if (distance <= 15 && distance > 10) return 95;
  if (distance <= 10) return 100;
}
