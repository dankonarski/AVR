#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESPTelnet.h>
ESP8266WebServer server(80);
ESPTelnet telnet;

//<<Konfiguracja ESP
const char* host_name = "esp0103";

//Konfiguracja ESP>>


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
  delay(500);
  server.on("/", handleRootPath);
  server.begin();
  telnet.begin();
  telnet.onConnect(onTelnetConnect);
  telnet.onInputReceived([](String str) {
    if (str == "ping") {
      telnet.println("> pong");
    }
    if (str == "uptime") {
      telnet.println("> " + getDeviceUpTime());
    }
  });
  
}


void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  telnet.loop();
  delay(100);
  
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
