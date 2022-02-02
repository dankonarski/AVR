#include <OneWire.h>
#include <DallasTemperature.h>

String input;
OneWire oneWire(4);
DallasTemperature sensors(&oneWire);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  sensors.begin();
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("STOP");
  delay(1000);

  if(Serial.available()){
        input = Serial.readStringUntil('\n');
        if(input == "EXIT"){
          input == "";
          Serial.println("Bye!");
          exit(0);
        }
    }
}
