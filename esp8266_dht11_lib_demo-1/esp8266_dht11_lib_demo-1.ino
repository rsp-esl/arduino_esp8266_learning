//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-01
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
//////////////////////////////////////////////////////////////////////////

#include "DHT.h" // -> https://github.com/adafruit/DHT-sensor-library

// define the DHT sensor type to be used (tested: DHT11 and DHT22)
#define DHT_TYPE   DHT11 
//#define DHT_TYPE   DHT22 

#define DHT_PIN    (2)       // use the D4 pin (GPIO-2)

// note: use Vcc=3.3V for the DHTxx sensor

DHT dht( DHT_PIN, DHT_TYPE );

void setup() {
  Serial.begin( 115200 );
  Serial.println( F("\n\n\n\n") );
  delay(1000);
  Serial.println( F("ESP8266 DHTxx Demo...") );
  dht.begin();
}

// global variable used for sprintf()
char sbuf[32];

void loop() {
  float humid = dht.readHumidity();  // read the humidity
  float temp = dht.readTemperature(); // read temperature as Celsius

  // Check if any reads failed and exit early (to try again).
  if ( isnan(humid) || isnan(temp) ) {
     Serial.println( F("Failed to read from DHT sensor!") );
     delay(1000);
     return;
  }
  Serial.print( F("Humidity: ") );
  dtostrf( humid, 3, 1, sbuf );
  Serial.print( sbuf );
  Serial.print( F(" %RH, ") );
  Serial.print( F("Temperature: ") );
  dtostrf( temp, 3, 1, sbuf );
  Serial.print( sbuf );
  Serial.println( F(" deg.C") );
  // wait a few seconds between measurements.
  delay(2000);
}
//////////////////////////////////////////////////////////////////////////

