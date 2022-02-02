void setup() {
  Serial.begin(115200);
  pinMode(15, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(15), ISR, FALLING);
}

void loop() {
  Serial.println("ping");
  delay(1000);
  Serial.println("pong");
  delay(1000);
}

ICACHE_RAM_ATTR void ISR() {
  Serial.println("interrupt1!");
}
