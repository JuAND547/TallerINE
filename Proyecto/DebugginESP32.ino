/************************************************
  ESP32 + Blynk + Google Sheets
  Prueba mínima funcional

  Hace:
  - conecta WiFi
  - recibe nombre desde Blynk
  - manda nombre y puntaje a Google Sheets
************************************************/

#define BLYNK_TEMPLATE_ID "TMPL2ELn8n1uh"
#define BLYNK_TEMPLATE_NAME "Juego de Reflejos"
#define BLYNK_AUTH_TOKEN "XmKnhiewNpY5DHpLiPz9OuHz7wtoCgXA"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <HTTPClient.h>

char ssid[] = "wifing";
char pass[] = "wifing-pub";

// URL Google Apps Script
String scriptURL = "https://script.google.com/macros/s/AKfycbxEP_72Yg_FaDb0ZT7je-9UUNNxtNh0MIzTwpgDGNts08JVuqg0NKNOdoqspMZ26v1amg/exec";

// Variables
String nombre_jugador = "";
int puntaje = 250;

#define PIN_NOMBRE V0

//--------------------------------------------------
// FUNCIÓN ENVIAR A GOOGLE SHEETS
//--------------------------------------------------

void enviar(String nombre, int puntaje) {

  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    String url = scriptURL +
                 "?nombre=" + nombre +
                 "&puntaje=" + String(puntaje);

    Serial.println("Enviando datos...");
    Serial.println(url);

    http.begin(url);

    int httpCode = http.GET();

    Serial.print("HTTP Response code: ");
    Serial.println(httpCode);

    http.end();
  }
  else {
    Serial.println("WiFi desconectado");
  }
}

//--------------------------------------------------
// RECIBIR NOMBRE DESDE BLYNK
//--------------------------------------------------

BLYNK_WRITE(PIN_NOMBRE) {

  nombre_jugador = param.asString();

  Serial.print("Nombre recibido: ");
  Serial.println(nombre_jugador);

  enviar(nombre_jugador, puntaje);
}

//--------------------------------------------------
// SETUP
//--------------------------------------------------

void setup() {

  Serial.begin(115200);

  Serial.println("Conectando WiFi...");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Serial.println("WiFi conectado");
}

//--------------------------------------------------
// LOOP
//--------------------------------------------------

void loop() {

  Blynk.run();
}