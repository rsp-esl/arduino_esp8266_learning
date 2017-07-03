//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-03
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
//////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include <OneWire.h>     // We need the OneWire library for DS18B20.

// -> https://github.com/milesburton/Arduino-Temperature-Control-Library
#include "DallasTemperature.h"

// Note: add 4.7k pull-up resistor to DATA pin of DS18B20 and use VCC=3.3V
#define DATA_PIN   (2) // GPIO-2 = D4 

OneWire oneWire( DATA_PIN );
DallasTemperature ds( &oneWire ); 

char sbuf[32];
String tempC;
uint8_t device_cnt; // DS18B20 count
DeviceAddress addr;

void setup() {
  Serial.begin(115200);
  Serial.println( "\n\n\n" );
  delay(100);
  Serial.println( "ESP8266 DS18B20 Demo..." );
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);

  do { 
    ds.begin(); // enumerate onewire devices
    device_cnt = ds.getDeviceCount();
    if ( device_cnt > 0 ) 
       break;
    Serial.println( "No OneWire device(s) found... rescan" );
    delay(1000);
  } while (1);

  Serial.print( "Number of OneWire device(s) found: " );
  Serial.println( device_cnt );
  for ( uint8_t i=1; i <= device_cnt; i++ ) {
     ds.getAddress( addr, (i-1) );
     String str = "";
     str += i;
     str += ") ";
     for ( int b=0; b < 8; b++ ) {
       sprintf( sbuf, "%02X", addr[b] );
       str += sbuf;
     }
     Serial.println( str );
  }
}

void loop() {
  float temp;  
  ds.requestTemperatures(); // read all connected devices
  Serial.println( "------------------------" );
  for ( uint8_t i=1; i <= device_cnt; i++ ) {
     temp = ds.getTempCByIndex( i-1 );
     if ( temp == 85.0 || temp == (-127.0) ) { // erroneous value
        sprintf( sbuf, "Device #%d read error!!!", i  );
        Serial.println( sbuf );
     } else {
        tempC = dtostrf( temp, 4, 1, sbuf );
        tempC += " deg.C";
        sprintf( sbuf, "Device #%d: ", i );
        Serial.print( sbuf );
        Serial.println( tempC );
     }
  }
  delay(500);
}
//////////////////////////////////////////////////////////////////////////


