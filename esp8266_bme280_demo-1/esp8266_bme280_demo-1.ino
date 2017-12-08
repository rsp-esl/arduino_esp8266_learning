////////////////////////////////////////////////////////////////////////////////////////
// Author: Rawat S., Department of Electrical & Computer Engineering, KMUTNB, Thailand
// Date: 2016-11-20
// Software: Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Hardware: Boards with the ESP-12 module
////////////////////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include <Wire.h>
#include "BME280.h"

#define LED_PIN        (D4)

#define SCL_PIN        (D1)
#define SDA_PIN        (D2)  


#define DEV_ADDR       (0x76)

#define INTERVAL_MSEC  (5000)  // update interval: 5 sec

#define SEA_LEVEL_PRESSURE_PA   (101325)

BME280 bme;

char sbuf[64];
uint32_t ts;

void wifi_off() {
  WiFi.mode( WIFI_STA );
  WiFi.disconnect(); 
  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  delay(1);
}

uint8_t i2c_scan( ) {
  uint8_t error;
  uint8_t start_addr = 0x01;
  uint8_t end_addr = 0x7f;
  int founds;
  
  founds = 0;
  for ( uint8_t addr = start_addr; addr < end_addr; addr++ ) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(addr);
    error = Wire.endTransmission();

    if ( error == 0 ) {
      sprintf( sbuf, "Found a device at 0x%02X", addr );
      Serial.println( sbuf );
      founds++;
    }
    yield();
  }
  
  if ( founds == 0 ) {
    Serial.println( F("No I2C devices found") );
  } 
  else {
    Serial.printf( "Scan done, %d device(s) found\r\n", founds );
  }
  return founds;
}

void setup() {
  pinMode( LED_PIN, OUTPUT );
  digitalWrite( LED_PIN, LOW );
  wifi_off();
  delay(100);
  
  Serial.begin( 115200 );
  for ( int i=0; i < 10; i++ ) {
    delay(100);
    Serial.println();
  }
  Serial.flush();

  Wire.begin( SDA_PIN, SCL_PIN );
  Wire.setClock(100000);
  i2c_scan();

  delay(1000);
  
  if ( !bme.begin( DEV_ADDR, SDA_PIN, SCL_PIN, 400000 ) ) {
    Serial.println( F("Could not find a valid BME280 sensor, check wiring!") );
    Serial.printf( "Chip ID : 0x%02x\n", bme.readChipID() );
    while (1) {
        delay(1000);
    }

  } else {
    Serial.println( F("BME280 device detected.") );
    Serial.printf( "Chip ID : 0x%02x\n", bme.readChipID() );
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

