#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
String apiKey = "Z5T7JPHT2RJQ82G8";
const char *ssid = "MSHOME";
const char *pass = "1234567890";
const char* server = "api.thingspeak.com";
#define ONE_WIRE_BUS 14
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
WiFiClient client;

void setup()
{
Serial.begin(9600);
delay(10);
 
Serial.println("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, pass);
 
while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
 
}
 
void loop()
{
sensors.requestTemperatures();
float tempC = sensors.getTempCByIndex(0);
float tempF = sensors.getTempFByIndex(0);
if ((tempC == -127.00) || (tempF == -196))
{
Serial.println("Failed to read from sensor!");
delay(1000);
}
else
{
Serial.print("Temperature in Celsius: ");
Serial.println(tempC);
Serial.print("Temperature in Fahrenheit: ");
Serial.println(tempF);
delay(1000);
}
 
if (client.connect(server,80)) //184.106.153.149 or api.thingspeak.com
{
String postStr = apiKey;
postStr +="&field1=";
postStr += String(tempC);
postStr +="&field2=";
postStr += String(tempF);
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
Serial.println("Sent data to Thingspeak");
}
client.stop();
Serial.println("Delay of 15 Sec");
// thingspeak needs minimum 15 sec delay between updates
delay(5000);
}
