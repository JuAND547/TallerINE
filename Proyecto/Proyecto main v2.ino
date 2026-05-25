//LIBRERÍAS



//-----------------//-----------------

//PINES Y VARIABLES GLOBALES
#define L1 2
#define L2 4
#define L3 6
#define L4 8
#define L5 10
int led[] = {L1, L2, L3, L4, L5};

#define B1 3
#define B2 5
#define B3 7
#define B4 9
#define B5 11
int boton[] = {B1, B2, B3, B4, B5};

#define buzzer 13

unsigned long t_inicial;
unsigned long t_reaccion = 0;
unsigned long inicio_juego;
unsigned long espera_entre_leds;
unsigned long inicio_espera;

int led_actual;
int boton_actual;


unsigned long t_reaccion_lim = 2000;     //t límite de reacción desde que se enciende un led random

int puntaje_actual;
int puntaje_total = 0;

bool jugador_listo = false;       //Si está listo el jugador
bool press = false;              //Si el jugador presionó el botón correspondiente


enum EstadoJuego {
  ESPERANDO_JUGADOR,
  ESPERANDO_LED,
  LED_ENCENDIDO,
  FIN_JUEGO
};

EstadoJuego estado;

//-----------------//-----------------

//DECLARACIÓN DE FUNCIONES
bool algun_boton_presionado();
int randomizar_espera();
void encender(int LED);
void apagar_leds();
void sonido(int tipo, int pin);
  enum Sonidos {
    COMIENZO,
    ACIERTO,
    FALLO,
    LED_ON
  };

int calcular_puntaje(unsigned long t);
//int enviar_t_reaccion();
//-----------------//-----------------

void setup() {
  
  Serial.begin(9600);

  for (int j = 0; j < 5; j++) {       //Pines de los leds
    pinMode(led[j], OUTPUT);
  }

  for (int j = 0; j < 5; j++) {       //Pines de los botones
    pinMode(boton[j], INPUT_PULLUP);
  }

  pinMode(buzzer, OUTPUT);

  randomSeed(analogRead(A0));         //Elijo seed para randomizaciones

  estado = ESPERANDO_JUGADOR;
}

//-----------------//-----------------

void loop() {

  switch (estado) {

    case ESPERANDO_JUGADOR:

      if (algun_boton_presionado()) {                // Check si el jugador está listo
        Serial.println("jugador listo");
        apagar_leds();
        delay(100);
        sonido(COMIENZO, buzzer);
        delay(1000);

        while(algun_boton_presionado()) {
        Serial.println("Esperando soltar boton");
        }

        inicio_cuenta_atrás = millis();
        inicio_espera = millis();
        espera_entre_leds = random(1000,3000);

        estado = ESPERANDO_LED;
      }
    break;

    case ESPERANDO_LED:

      if (millis() - inicio_espera >= espera_entre_leds) {

        led_actual = random(0,5);
        boton_actual = boton[led_actual];

        encender(led_actual);

        sonido(LED_ON, buzzer);

        t_inicial = millis();

        estado = LED_ENCENDIDO;
      }
    break;
    
    case LED_ENCENDIDO:

      press = (digitalRead(boton_actual) == LOW);

      t_reaccion = millis() - t_inicial;

      if (press) {

        puntaje_total += calcular_puntaje(t_reaccion);

        apagar_leds();

        sonido(ACIERTO, buzzer);

        Serial.print("T_reacción: "); Serial.println(t_reaccion);
        Serial.print("Puntaje total: "); Serial.println(puntaje_total);

        inicio_espera = millis();
        espera_entre_leds = random(1000,3000);

        estado = ESPERANDO_LED;
      }

      if (t_reaccion >= t_reaccion_lim) {

        apagar_leds();

        sonido(FALLO, buzzer);
        Serial.println("FALLO!");

        Serial.print("Puntaje total: "); Serial.println(puntaje_total);

        inicio_espera = millis();
        espera_entre_leds = random(1000,3000);

        estado = ESPERANDO_LED;
      }
    break;
    
    case FIN_JUEGO:

      apagar_leds();

      Serial.println("FIN");

    break;
  }
  
  if (millis() - inicio_juego >= tiempo_de_juego) {

    estado = FIN_JUEGO;
  }

}
  


    
  

  press = (digitalRead(boton_actual) == LOW);

  if (t_reaccion < t_reaccion_lim) {

    if (!press) {
      if (millis() - inicio_espera >= espera_entre_leds && esperando_led) {            //En lugar de usar delay(), checkeo que haya pasado el tiempo de espera
      encender(led_actual);
      sonido(LED_ON, buzzer);
      Serial.println("Encendiendo LED");

      t_inicial = millis();

      listo_sig_boton = false;
      esperando_led = false;
      led_encendido = true;
      }

      if (led_encendido) {
      t_reaccion = millis() - t_inicial;
      }

    } else {
      puntaje_total += calcular_puntaje(t_reaccion);
    
      Serial.print("T_reacción: "); Serial.println(t_reaccion);
      Serial.print("Puntaje total: "); Serial.println(puntaje_total);

      sonido(ACIERTO, buzzer);
      apagar_leds();
      
      t_reaccion = 0;
      listo_sig_boton = true;
      led_encendido = false;
    }

  } else {
    // 0 pts!!!
    Serial.println("FALLO!!");
    apagar_leds();
    sonido(FALLO, buzzer);
    t_reaccion = 0;
    listo_sig_boton = true;      //Listo para el siguiente botón
    led_encendido = false;
    delay(800); 
  }


  if (fin_de_juego) {
  //Fin del juego (se terminó el tiempo)
  }
  
}

//-----------------//-----------------

//FUNCIONES



void apagar_leds() {
  for (int j = 0; j < 5; j++) { 
  digitalWrite(led[j], LOW);
  }
}

void encender(int LED) {
  digitalWrite(led[LED], HIGH);
}


int calcular_puntaje(unsigned long t) {
  return(5000 / t);           //Cálculo del puntaje en base del tiempo de reacción       
}

void sonido(int tipo, int pin) {

  switch (tipo) {
    case COMIENZO:
      tone(pin, 1000, 150);
	    break;
    case ACIERTO:
      tone(pin, 1200, 100);
      break;
    case FALLO:
      tone(pin, 100, 100);
      break;
    case LED_ON:
      tone(pin, 1200, 60);
      break;
  }
}


bool algun_boton_presionado() {

    for (int i = 0; i < 5; i++) {

        if (digitalRead(boton[i]) == LOW) {
            return true;
        }
    }

    return false;
}


//-----------------//-----------------
