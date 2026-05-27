//LIBRERÍAS

#include <WiFi.h>
#include <HTTPClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_TEMPLATE_ID "TU_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "JuegoReflejos"
#define BLYNK_AUTH_TOKEN "TU_AUTH_TOKEN"

char ssid[] = "TU_WIFI";
char pass[] = "PASSWORD";

String scriptURL = "https://script.google.com/macros/s/AKfycbxEP_72Yg_FaDb0ZT7je-9UUNNxtNh0MIzTwpgDGNts08JVuqg0NKNOdoqspMZ26v1amg/exec";

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

#define pin_nombre V0
#define pin_puntaje V1

unsigned long t_inicial;
unsigned long t_reaccion = 0;
//unsigned long inicio_juego;     (uso contador)
unsigned long espera_entre_leds;
unsigned long inicio_espera;

int led_actual;
int boton_actual;


unsigned long t_reaccion_lim = 2000;     //t límite de reacción desde que se enciende un led random

int puntaje_actual;
int puntaje_total = 0;

String nombre_jugador = "";

bool jugador_listo = false;       //Si está listo el jugador
bool press = false;              //Si el jugador presionó el botón correspondiente
bool sonido_final_rep = false;
bool ingresar_mensaje_final = false;

enum EstadoJuego {
  ESPERANDO_JUGADOR,
  ESPERANDO_LED,
  LED_ENCENDIDO,
  INGRESAR_NOMBRE,
  FIN_JUEGO
};

EstadoJuego estado;

//-----------------//-----------------

//DECLARACIÓN DE FUNCIONES
bool algun_boton_presionado();
void encender(int LED);
void apagar_leds();
void sonido(int tipo, int pin);
  enum Sonidos {
    COMIENZO,
    ACIERTO,
    FALLO,
    LED_ON,
    POCO_TIEMPO,
    FIN
  };

void est_esperando_jugador();
void est_esperando_led();
void est_led_encendido();
void est_ingresar_nombre();
void est_fin_juego();


int calcular_puntaje(unsigned long t);

void enviar(String nombre, int puntaje);

//-----------------//-----------------

void setup() {
  
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

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

  Blynk.run();
  
  switch (estado) {

    case ESPERANDO_JUGADOR:
      est_esperando_jugador();
    break;

    case ESPERANDO_LED:
      est_esperando_led();
    break;
    
    case LED_ENCENDIDO:
      est_led_encendido();
    break;
    
    case INGRESAR_NOMBRE:
      est_ingresar_nombre();
    break;
    
    case FIN_JUEGO:

      apagar_leds();

      Serial.println("FIN");

    break;
    
  }
  
}

//-----------------//-----------------

//FUNCIONES

void est_esperando_jugador() {

  lcd_1.setCursor(0,0);
  lcd_1.print("Presione");
  lcd_1.setCursor(0,1);
  lcd_1.print("cualquier boton");

  if (algun_boton_presionado()) {                // Check si el jugador está listo
    Serial.println("jugador listo");

    apagar_leds();
    lcd_1.clear();

    delay(50);
    sonido(COMIENZO, buzzer);
    delay(1000);

    while(algun_boton_presionado()) {
      Serial.println("Esperando soltar boton");
    }

  inicio_cuenta_atras = millis();
  inicio_espera = millis();
  espera_entre_leds = random(1000,3000);

  estado = ESPERANDO_LED;

  }

}

void est_esperando_led() {

  if (millis() - inicio_espera >= espera_entre_leds) {

    led_actual = random(0,5);
    boton_actual = boton[led_actual];

    apagar_leds();
    encender(led_actual);

    sonido(LED_ON, buzzer);

    t_inicial = millis();

    estado = LED_ENCENDIDO;
  }

}

void est_led_encendido() {

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

}

void est_ingresar_nombre() {

  apagar_leds();
  sonido(FIN, buzzer);

  
  if (!ingresar_mensaje_final) {
    Serial.println("Ingresar nombre en Blynk");
    ingresar_mensaje_final = true;
  }

  lcd_1.setCursor(0,0);
  lcd_1.print("Ingresar nombre");
  
}

BLYNK_WRITE(pin_nombre) {

  nombre_jugador = param.asString();

  enviar(nombre_jugador, puntaje_total);

  Serial.print("Nombre: ");
  Serial.println(nombre_jugador);

  estado = FIN_JUEGO;
}

void est_fin_juego() {

}

//...............................

void apagar_leds() {
  for (int j = 0; j < 5; j++) { 
  digitalWrite(led[j], LOW);
  }
}

void encender(int LED) {
  digitalWrite(led[LED], HIGH);
}


int calcular_puntaje(unsigned long t) {
  if (t != 0) {
    return(5000 / t);           //Cálculo del puntaje en base del tiempo de reacción    
  } else {
    return 0;
  }   
}

void sonido(int tipo, int pin) {

  switch (tipo) {

    case COMIENZO:
      tone(pin, 1000, 150);
	  break;

    case ACIERTO:
      tone(pin, 1800, 100);
    break;

    case FALLO:
      tone(pin, 100, 100);
    break;

    case LED_ON:
      tone(pin, 1200, 60);
    break;

    case POCO_TIEMPO:
      tone(pin, 1400, 60);
    break;

    case FIN:
      if (!sonido_final_rep) {
        tone(pin, 1000, 60);
        delay(200);
        tone(pin, 1000, 60);
        delay(200);
        tone(pin, 1000, 60);
        sonido_final_rep = true;
      }
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

void enviar(String nombre, int puntaje) {

  if(WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    String url = scriptURL +
                 "?nombre=" + nombre +
                 "&puntaje=" + String(puntaje);

    http.begin(url);

    int httpCode = http.GET();

    Serial.println(httpCode);

    http.end();
  }

}


//-----------------//-----------------
