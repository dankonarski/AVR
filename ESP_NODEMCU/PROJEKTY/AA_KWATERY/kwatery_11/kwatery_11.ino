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
DallasTemperature sensors(&oneWire);
ESP8266WebServer server(80);
WiFiClient client;
ESPTelnet telnet;


String app_version = "1.1"; //Wersja firmware
int numberOfDevices = 0;
int lcdclock = 0;  //zegar dla wyswietlania na LCD
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
  lcd.begin();
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
  numberOfDevices = sensors.getDeviceCount();
  delay(100);

  lcd.clear();
  lcd.setCursor(0, 0);
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
    }

  if (tsclock == 0) {TSTempUpload();}
  tsclock++;
  if (tsclock == 6000) {tsclock = 0;}

  delay(100);
}


void LCDPrintTemp(int dclock) {
  if (dclock == 0) {
    sensors.requestTemperatures();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Miesz. 1: ");
    lcd.print(printTemperature(sensor1),1);
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Miesz. 2: ");
    lcd.print(printTemperature(sensor2),1);
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
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(printTemperature(sensor1));
    postStr +="&field2=";
    postStr += String(printTemperature(sensor2));
    postStr +="&field3=";
    postStr += String(printTemperature(sensor3));
    postStr +="&field4=";
    postStr += String(printTemperature(sensor4));
    postStr +="&field5=";
    postStr += String(printTemperature(sensor5));
    postStr +="&field6=";
    postStr += String(printTemperature(sensor6));
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
