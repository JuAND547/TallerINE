#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd_1(0x27, 16, 2);

unsigned long inicio_cuenta_atras;
unsigned long tiempo_transcurrido;
int puntaje_anterior = 0;
int segundos_restantes;

const unsigned long tiempo_de_juego = 60000;

void beep_poco_tiempo();

bool beep[] = {false, false, false};

//-----------------------------

void setup()
{
  Serial.begin(9600);

  lcd_1.init();
  lcd_1.backlight();

}

void loop()
{

  if (estado == ESPERANDO_LED || estado == LED_ENCENDIDO) {

    tiempo_transcurrido = millis() - inicio_cuenta_atras;

    segundos_restantes = (tiempo_de_juego - tiempo_transcurrido) / 1000;

    beep_poco_tiempo();

    if (segundos_restantes > 0) {
      lcd_1.setCursor(0,0);
      lcd_1.print("Puntaje   Tiempo");

      lcd_1.setCursor(12,1);
      lcd_1.print("   ");
      lcd_1.print(segundos_restantes);
    
      if (puntaje_anterior != puntaje_total) {

        puntaje_anterior = puntaje_total;

        lcd_1.setCursor(2,1);
        lcd_1.print(puntaje_total);

      }

    } else {

      lcd_1.clear();
      segundos_restantes = 0;

      estado = INGRESAR_NOMBRE;
    }

  }
  
}

//-----------------------------

void beep_poco_tiempo() {

  for (int i = 3; i > 0; i--) {

    if (segundos_restantes == i && !beep[i-1]) {
    sonido(POCO_TIEMPO, buzzer);
    beep[i-1] = true;
    }

  }

}



