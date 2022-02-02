String input;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);                  
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LED ON");
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED OFF");
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
