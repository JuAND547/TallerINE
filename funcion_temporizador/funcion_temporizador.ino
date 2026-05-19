#include <Adafruit_LiquidCrystal.h>
void temporizador();

void setup()
{
}

void loop()
{
  temporizador();
}

void temporizador() {
  Adafruit_LiquidCrystal lcd_1(0);
  lcd_1.begin(16, 2);
  lcd_1.print("Tiempo restante");
  byte seconds = 60;
  while (seconds > 0) {
  lcd_1.setCursor(0, 1);
  lcd_1.print(seconds);
  lcd_1.setBacklight(1);
  delay(1000); 
  seconds --;
  lcd_1.setCursor(1, 1);
  lcd_1.print(" ");
  }
  lcd_1.setCursor(0, 1);
  lcd_1.print("0");
}
