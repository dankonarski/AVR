#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 14
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
int numberOfDevices = 0;
int wifitry = 0;
DeviceAddress tempDeviceAddress;
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Konfiguracja
char *app_version = "1.0";
uint8_t sensor1[8] = { 0x28, 0xDC, 0xD7, 0x48, 0xF6, 0x26, 0x3C, 0x7E };
uint8_t sensor2[8] = { 0x28, 0x09, 0xBA, 0x48, 0xF6, 0xD9, 0x3C, 0x2D };
uint8_t sensor3[8] = { 0x28, 0x9D, 0x58, 0x48, 0xF6, 0x46, 0x3C, 0xC2 };
String apiKey = "Z5T7JPHT2RJQ82G8";
const char *ssid = "MSHOME";
const char *pass = "1234567890";
const char* server = "api.thingspeak.com";
WiFiClient client;

void setup() {
  Wire.begin(D2, D1);
  delay(500);
  lcd.begin();
  lcd.home();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TempKwatery");
  lcd.setCursor(13, 0);
  lcd.print(app_version);
  lcd.setCursor(0, 1);
  lcd.print("Ladowanie...");
  delay(5000);
  sensors.begin();
  numberOfDevices = sensors.getDeviceCount();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Czujniki: ");
  lcd.print(numberOfDevices);
  lcd.setCursor(0, 1);
  lcd.print("WiFi: ");
  lcd.print(ssid);
  delay(5000);
}

void loop() {
  lcd.backlight();
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
  delay(5000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Miesz. 3: ");
  lcd.print(printTemperature(sensor3),1);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Miesz. 4: ");
  lcd.print(printTemperature(sensor3),1);
  lcd.print((char)223);
  lcd.print("C");
  delay(5000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wysylam raport");
  lcd.setCursor(0, 1);
  lcd.print("Laczenie WiFi...");
  
  if (WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid, pass);    
  }
  
  wifitry = 0;
  
  while (WiFi.status() != WL_CONNECTED)
    {
    wifitry++;
    if (wifitry == 8) {
      break;
      }
    delay(1000);
    }
    
  if (wifitry == 8){
    //Brak WiFi
    lcd.setCursor(0, 1);
    lcd.print("Brak WiFi       ");
    delay(5000);
  }
  else {
    //Polaczono z WiFi
    lcd.setCursor(0, 1);
    lcd.print("Polaczono       ");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("Wysylam         ");
    if (client.connect(server,80)) {
      String postStr = apiKey;
      postStr +="&field1=";
      postStr += String(printTemperature(sensor1));
      postStr +="&field2=";
      postStr += String(printTemperature(sensor2));
      postStr +="&field3=";
      postStr += String(printTemperature(sensor3));
      postStr +="&field4=";
      postStr += String(printTemperature(sensor3));
      postStr +="&field5=";
      postStr += String(printTemperature(sensor3));
      postStr +="&field6=";
      postStr += String(printTemperature(sensor3));
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
      lcd.setCursor(0, 1);
      lcd.print("Sukces          ");
      delay(2000);
      }
      else {
        lcd.setCursor(0, 1);
        lcd.print("Brak Internetu  ");
        delay(5000);
      }
    client.stop();
  }
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Na kocie...");
  lcd.noBacklight();
  delay(60000);
  
}

float printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  return tempC;
}
