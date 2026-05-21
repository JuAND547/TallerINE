byte elegir_led();
const int led1 = 1;
const int led2= 2;
const int led3 = 3;
const int led4 = 4;
const int led5 = 5;

leds[]= {led1, led2, led3, led4, led5};
void setup() {
  

}

void loop() {

  elegir_led();
  
}

byte elegir_led() {
  byte j = random(6);
  return leds[j];
}
