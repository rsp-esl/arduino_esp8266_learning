////////////////////////////////////////////////////////////////////////////////////////
// Author: Rawat S., Department of Electrical & Computer Engineering, KMUTNB, Thailand
// Date: 2017-11-29
// Software: Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Hardware: Boards with the ESP-12 module
////////////////////////////////////////////////////////////////////////////////////////

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

#define USB_SERIAL_BAUDRATE     (9600)
#define SOFT_SERIAL_BAUDRATE    (9600)

#define CHAR_CR  0x0d   // '\r'
#define CHAR_LF  0x0a   // '\n'

#define SOFT_SERIAL_TX_PIN      (5)  // GPIO-5 = D1
#define SOFT_SERIAL_RX_PIN      (4)  // GPIO-4 = D2

// see: https://www.arduino.cc/en/Reference/SoftwareSerial
SoftwareSerial softSerial( SOFT_SERIAL_RX_PIN, SOFT_SERIAL_TX_PIN );

////////////////////////////////////////////////////////////////////////////////////////

void wifi_off() {
  WiFi.mode( WIFI_STA );
  WiFi.disconnect(); 
  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  delay(1);
}

void setup() {  
  wifi_off(); // turn off WiFi to reduce power consumption
  
  Serial.begin( USB_SERIAL_BAUDRATE );
  softSerial.begin( SOFT_SERIAL_BAUDRATE );
  softSerial.flush();
  Serial.flush();
}

void loop() {
  static char ch;
  
  while ( softSerial.available() > 0 ) { // read from soft serial and send to usb serial
    ch = softSerial.read(); 
    Serial.write( ch ) ;
  }
  
  while ( Serial.available() > 0 ) { // read from usb serial and send to soft serial
    ch = Serial.read(); 
    softSerial.write( ch ) ;
  }
}
////////////////////////////////////////////////////////////////////////////////////////


