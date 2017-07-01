//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-01
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
//////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>

const int QA_PIN = 14;  // D5 = GPIO14
const int QB_PIN = 12;  // D6 = GPIO12

char sbuf[64];
volatile int last_a = 1, last_b = 1;
volatile int16_t encoder_cnt = 0;
volatile bool encoder_update = false;

void encode_inputs();

void encode_inputs() {
  detachInterrupt( QA_PIN );
  int a = digitalRead(QA_PIN);
  int b = digitalRead(QB_PIN);
  if (( a != last_a ) || ( b != last_b ) ) {
     if ( a == b ) {
       encoder_cnt++;
       encoder_update = true;
    } else { 
       encoder_cnt--;
       encoder_update = true;
    }
    last_a = a;
    last_b = b;
  }
  attachInterrupt( QA_PIN, encode_inputs, CHANGE );
}

void setup() {
  Serial.begin( 115200 );
  Serial.println( F("\n\n\n") );
  delay(100);
  Serial.println( F("ESP8266 Rotary Encoder Demo") );

  pinMode( QA_PIN, INPUT );
  pinMode( QB_PIN, INPUT );
  attachInterrupt( QA_PIN, encode_inputs, CHANGE ); // use QA_PIN for external interrupt
  
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF); // disable WiFi
}

void loop() {
  if ( encoder_update ) {
     encoder_update = false;
     sprintf_P( sbuf, PSTR("Encoder Counter: %d"), encoder_cnt );
     Serial.println( sbuf );
  }
  delay(1);
}
//////////////////////////////////////////////////////////////////////////

