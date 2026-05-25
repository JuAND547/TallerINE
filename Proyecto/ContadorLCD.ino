#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd_1(0);

unsigned long inicio_cuenta_atras;
unsigned long tiempo_transcurrido;

const unsigned long tiempo_de_juego = 60000;

bool fin_de_juego = false;

void setup()
{
  Serial.begin(9600);

  lcd_1.begin(16, 2);

  pinMode(20, OUTPUT);
  pinMode(21, OUTPUT);

 
  
}

void loop()
{

  //while (!jugador_listo) Hace falta aquí???

  tiempo_transcurrido = millis() - inicio_cuenta_atras;

  int segundos_restantes = (tiempo_de_juego - tiempo_transcurrido) / 1000;

  if (segundos_restantes > 0) {
    lcd_1.setCursor(0,0);
    lcd_1.print("Puntaje   Tiempo");

    lcd_1.setCursor(12,1);
    lcd_1.print(segundos_restantes);
    

  } else {
    lcd_1.clear();
    segundos_restantes = 0;
    fin_de_juego = true;
  }
}
  
