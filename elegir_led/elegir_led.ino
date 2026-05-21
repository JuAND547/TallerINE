void elegir_led();
void setup() {
  for (int i=8; i < 13; i++) { // Declarando LEDs
    pinMode(i, OUTPUT);
  }
}

void loop() {
  elegir_led();
  delay(1000);
}

void elegir_led() {
  byte j = random(8, 13);
  digitalWrite(j, HIGH);
  delay(1000);
  digitalWrite(j, LOW);
}
