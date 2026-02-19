// Adafruit IO Publish & Subscribe Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration - Librerias ***********************************/

#include "config.h"
#include "Wire.h"

/************************ Variables y definiciones *******************************/
// DEFINICIONES IMPORTANTES
#define IO_LOOP_DELAY 5000

#define SLAVE_ADDR 0x09
#define SDA_PIN 21
#define SCL_PIN 22

// VARIABLES GLOBALES
unsigned long lastUpdate = 0;

uint8_t dato_activo = 0;
uint8_t dato_peso = 0;
uint8_t dato_distancia = 0;
uint8_t dato_color = 0;

uint8_t feed_activo = 0;
uint8_t feed_peso = 0;
uint8_t feed_distancia = 0;
uint8_t feed_color = 0;

volatile bool nuevo_dato = false;
volatile uint8_t lastByte = 0;

typedef struct {
  uint8_t activo;
  uint8_t color;
  uint8_t distancia;
  uint8_t peso;
} datosI2C;

datosI2C sensores;

// Configuracion de los FEEDS 
//AdafruitIO_Feed *counter = io.feed("counter");
AdafruitIO_Feed *pesoFeed = io.feed("peso");
AdafruitIO_Feed *distanciaFeed = io.feed("distancia");
AdafruitIO_Feed *colorFeed = io.feed("color");
AdafruitIO_Feed *dcFeed = io.feed("motor_dc");

/************************ Funciones *******************************/
void onReceive(int numBytes) {
  if (numBytes == sizeof(datosI2C)) {

    uint8_t *ptr = (uint8_t*)&sensores;

    for (int i = 0; i < sizeof(datosI2C); i++) {
      if (Wire.available()) {
        ptr[i] = Wire.read();
      }
    }
    nuevo_dato = true;
  }
}

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  //dcFeed->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  //dcFeed->get();

  Wire.begin(SLAVE_ADDR, SDA_PIN, SCL_PIN, 100000); // Orden correcto
  Wire.onReceive(onReceive);

  Serial.println("ESP32 listo como Slave I2C");

}

void newDATA() {
  if (dato_activo != sensores.activo) {
    dato_activo = sensores.activo;
    feed_activo = 1;
  }
  if (dato_peso != sensores.peso) {
    dato_peso = sensores.peso;
    feed_peso = 1;
  }
  if (dato_distancia != sensores.distancia) {
    dato_distancia = sensores.distancia;
    feed_distancia = 1;
  }
  if (dato_color != sensores.color) {
    dato_color = sensores.color;
    feed_color = 1;
  }
}

/************************ LOOP principal *******************************/

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  if (nuevo_dato) {
    newDATA();
  }

  if (millis() > (lastUpdate + IO_LOOP_DELAY)) {

    if (feed_distancia){
      Serial.print("Distancia sending -> ");
      Serial.println(sensores.distancia);
      distanciaFeed->save(sensores.distancia);

      feed_distancia = 0;
    }
    else if (feed_peso){
      Serial.print("Peso sending -> ");
      Serial.println(sensores.peso);
      pesoFeed->save(sensores.peso);

      feed_peso = 0;
    }
    else if (feed_color){
      Serial.print("Color sending -> ");
      Serial.println(sensores.color);
      colorFeed->save(sensores.color);

      feed_color = 0;
    }
    if (feed_activo){
      Serial.print("Activo sending -> ");
      Serial.println(sensores.activo);
      dcFeed->save(sensores.activo);

      feed_activo = 0;
    }
    // after publishing, store the current time
    lastUpdate = millis();
  }

  // if (nuevo_dato){
  //   nuevo_dato = false;
  //   Serial.printf("Recibido: 0x%02X\n", sensores.activo);
  //   Serial.printf("Recibido: 0x%02X\n", sensores.color);
  //   Serial.printf("Recibido: 0x%02X\n", sensores.distancia);
  //   Serial.printf("Recibido: 0x%02X\n", sensores.peso);
  // }
}

// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.

// void handleMessage(AdafruitIO_Data *data) {

//   Serial.print("received <- ");
//   Serial.println(data->value());

// }
