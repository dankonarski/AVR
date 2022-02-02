#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define ONE_WIRE_BUS 14
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);
uint8_t sensor1[8] = { 0x28, 0xDC, 0xD7, 0x48, 0xF6, 0x26, 0x3C, 0x7E };
uint8_t sensor2[8] = { 0x28, 0x09, 0xBA, 0x48, 0xF6, 0xD9, 0x3C, 0x2D };
uint8_t sensor3[8] = { 0x28, 0x9D, 0x58, 0x48, 0xF6, 0x46, 0x3C, 0xC2 };

void setup(void)
{
  Serial.begin(9600);
  delay(500);
  Wire.begin(D2, D1);
  lcd.begin();
  lcd.home();
  sensors.begin();
}

void loop(void)
{
  sensors.requestTemperatures();
  
  Serial.print("Sensor 1: ");
  printTemperature(sensor1);
  
  Serial.print("Sensor 2: ");
  printTemperature(sensor2);
  
  Serial.print("Sensor 3: ");
  printTemperature(sensor3);
  
  Serial.println();
  delay(1000);
}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print(tempC, 1);
}
