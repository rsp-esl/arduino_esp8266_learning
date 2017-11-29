////////////////////////////////////////////////////////////////////////////////////////
// Author: Rawat S., Department of Electrical & Computer Engineering, KMUTNB, Thailand
// Date: 2016-11-20
// Software: Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Hardware: Boards with the ESP-12 module
////////////////////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include <Wire.h>
#include "BME280.h"

#define LED_PIN        (D4)    // D4 pin (GPIO-2)

#define SDA_PIN        (D2)    // D2 pin (GPIO-4)
#define SCL_PIN        (D1)    // D1 pin (GPIO-5)

#define DEV_ADDR       (0x76)

#define INTERVAL_MSEC  (5000)  // update interval: 5 sec

#define SEA_LEVEL_PRESSURE_PA   (101325)

BME280 bme;

char sbuf[64];
uint32_t ts;

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
  WiFi.mode( WIFI_OFF );

  if (!bme.begin( DEV_ADDR, SDA_PIN, SCL_PIN, 400000 ) ) {
    Serial.println( "Could not find a valid BME280 sensor, check wiring!" );
    while (1) { delay(5); }
  } else {
    Serial.println( F("BME280 device detected.") );
  }
  Serial.println( F("BME280 sensor reading...") );
  ts = millis();
}

void process() {
  float temperature, humidity, pressure, altitude;
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() /100.0f; // hPa
  altitude = bme.readAltitude( SEA_LEVEL_PRESSURE_PA );

  String str;
  str += "Temperature: ";
  dtostrf( temperature, 3, 1, sbuf );
  str += sbuf;
  str += " deg.C";

  str += ", Humidity: ";
  dtostrf( humidity, 3, 1, sbuf );
  str += sbuf;
  str += " %RH";

  str += ", Air Pressure: ";
  dtostrf( pressure, 5, 1, sbuf );
  str += sbuf;
  str += " hPa";

  str += ", Altitude: ";
  dtostrf( altitude, 5, 1, sbuf );
  str += sbuf;
  str += " m";

  Serial.println( str.c_str() );
}

void loop() {
  if ( millis() - ts >= INTERVAL_MSEC ) {
     ts += INTERVAL_MSEC;
     digitalWrite( LED_PIN, HIGH );
     process();
     digitalWrite( LED_PIN, LOW );
  }
  delay(1); 
}
////////////////////////////////////////////////////////////////////////////////////////

