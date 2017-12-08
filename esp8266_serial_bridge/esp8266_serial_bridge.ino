////////////////////////////////////////////////////////////////////////////////////////
// Author: Rawat S., Department of Electrical & Computer Engineering, KMUTNB, Thailand
// Date: 2017-12-07
// Software: Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Hardware: Boards with the ESP-12 module
////////////////////////////////////////////////////////////////////////////////////////

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

#define USB_SERIAL_BAUDRATE     (9600)
#define SOFT_SERIAL_BAUDRATE    (9600)

#define CHAR_CR  0x0d   // '\r'
#define CHAR_LF  0x0a   // '\n'

#define SOFT_SERIAL_TX_PIN      (D1)  // GPIO-5 = D1
#define SOFT_SERIAL_RX_PIN      (D2)  // GPIO-4 = D2

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
  delay(100);
  
  Serial.begin( USB_SERIAL_BAUDRATE );
  Serial.flush();
  softSerial.begin( SOFT_SERIAL_BAUDRATE );
  softSerial.flush();
  delay(100);
}

void loop() {
  static char ch;

  while ( Serial.available() > 0 ) { // read from usb serial and send to soft serial
    ch = Serial.read(); 
    softSerial.write( ch ) ;
  }
  
  while ( softSerial.available() > 0 ) { // read from soft serial and send to usb serial
    ch = softSerial.read(); 
    Serial.write( ch ) ;
  }
}
////////////////////////////////////////////////////////////////////////////////////////


