#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Wire.begin(D2, D1);
  lcd.begin();
  lcd.home();

}

void loop() { 
  lcd.clear();
  lcd.print("Bartek");
  delay(5000);
  
  lcd.clear();
  lcd.print("lubi");
  delay(5000);
  
  lcd.clear();
  lcd.print("psotowac");
  delay(5000);
  
}
