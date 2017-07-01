//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-01
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
//////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include <Wire.h>
#include "BH1750.h"            // -> https://github.com/claws/BH1750

#define LED_PIN        (D4)    // D4 pin (GPIO-2)
#define SDA_PIN        (D2)    // D2 pin (GPIO-4)
#define SCL_PIN        (D1)    // D1 pin (GPIO-5)

#define DEV_ADDR       (0x23)  // set the I2C device address (BH1750)

#define INTERVAL_MSEC  (1000)  // update interval: 1 sec

BH1750 bh( DEV_ADDR );         // create BH1750 object

// global variables
char sbuf[64];                 // char buffer for sprintf()
uint32_t ts;                   // used to save timestamp 

void setup() {
  pinMode( LED_PIN, OUTPUT );
  digitalWrite( LED_PIN, LOW );
  Serial.begin( 115200 );

  for ( int i=0; i < 10; i++ ) {
    delay(100);
    Serial.println();
  }
  Serial.flush();

  WiFi.disconnect();
  WiFi.mode( WIFI_OFF ); // disable WiFi

  Serial.println( F("BH1750 Sensor Reading...") );
  bh.begin( BH1750_CONTINUOUS_HIGH_RES_MODE,
            SDA_PIN, SCL_PIN, 400000 /* set I2C frequency */ );
  ts = millis();
}

void process() {
  uint16_t lux = bh.readLightLevel();  // read the sensor value
   
  String str;
  str += "Light: ";
  dtostrf( lux, 6, 1, sbuf );
  str += sbuf;
  str += " Lux";

  Serial.println( str.c_str() );
}

void loop() {
  if ( millis() - ts >= INTERVAL_MSEC ) {
     ts += INTERVAL_MSEC;
     digitalWrite( LED_PIN, HIGH );
     process();
     digitalWrite( LED_PIN, LOW );
  }
  delay(1); // delay for 1 msec
}
//////////////////////////////////////////////////////////////////////////
