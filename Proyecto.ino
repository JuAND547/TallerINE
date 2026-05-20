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

#define buzzer 1

unsigned long t_inicial;
int t_reaccion;
const int t_reaccion_lim = 2000;     //t límite de reacción desde que se enciende un led random
int espera;
int led_actual;
int boton_actual;

const int tiempo_de_juego = 60000    //t límite de juego en ms

int puntaje_actual;
int puntaje_total = 0;

bool jugador_listo = false       //Si está listo el jugador
bool press = false;              //Si el jugador presionó el botón correspondiente
//-----------------//-----------------

//DECLARACIÓN DE FUNCIONES
bool algun_boton_presionado();
int randomizar_espera();
void led_random_ON();
void apagar_leds();
void sonidoAcierto();
void empezar_contador();        //maybe se puede hacer sin función (con un while que chequee que no se presionó un botón ni se encendió un led)
int enviar_t_reaccion();
int calcular_puntaje(int t);
int hallar_boton(int L);
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
}

//-----------------//-----------------

void loop() {
  while (!jugador_listo) {                        //"Para empezar presiona cualquier botón"
    t_espera = millis();
    if (algun_boton_presionado()) {                // Check si el jugador está listo
      jugador_listo = true;
      apagar_leds();
      t_trascurrido = millis() - t_espera
      delay(2000);
      // + sonido de indicación de comienzo
      // O podría ser cuenta regresiva 3s con beep cada seg
    }
  }
  
  while (t_trascurrido < tiempo_de_juego) {
    
    t_trascurrido = millis() - t_espera;
    press = !digitalRead(boton_actual);

    if (!press) {                    //&& ningún led prendido???
      espera = random(1000, 5000);
      delay(espera);                //Buscar alternativa para esto, ya que jode el loop()
      led_random_ON();
      boton_actual = hallar_boton(led_actual);
      //empezar_contador();
      t_inicial = millis();
    } //else 

    if (press)   {
      t_reaccion = millis() - t_inicial;
      puntaje_total += calcular_puntaje(t_reaccion);
    
      Serial.print("T_reacción: "); Serial.println(t_reaccion);
      Serial.print("Puntaje total: "); Serial.println(puntaje_total);
    }
  }

  //Fin del juego (se terminó el tiempo)

}

//-----------------//-----------------

//FUNCIONES



void apagar_leds() {
  for (int j = 0; j < 5; j++) { 
  digitalWrite(led[j], LOW);
  }
}

void led_random_ON() {
  led_actual = random(0, 4)
  digitalWrite(led[led_actual], HIGH);
}

void sonidoAcierto() {

    tone(buzzer, 1200);

    delay(100);

    noTone(buzzer);
}

int hallar_boton(int L) {
  switch (L) {
    case L1: return(B1);
    case L2: return(B2);
    case L3: return(B3);
    case L4: return(B4);
    case L5: return(B5);
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
