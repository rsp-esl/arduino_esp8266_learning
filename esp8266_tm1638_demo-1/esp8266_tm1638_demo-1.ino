//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-10
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
//////////////////////////////////////////////////////////////////////////

#include "TM1638.h"

const int CLK_PIN  = 5;  // GPIO-5 / D1
const int DATA_PIN = 4;  // GPIO-4 / D2
const int STRB_PIN = 0;  // GPIO-0 / D3

char sbuf[64];
uint32_t ts;

TM1638 tm1638( DATA_PIN, CLK_PIN, STRB_PIN );

void test_leds() {
  uint8_t leds;
  // test 1
  leds = 0xff;
  tm1638.setLEDs( leds );
  delay(2000);

  // test 2
  leds = 0x55;
  for ( int i=0; i < 10; i++ ) {
    tm1638.setLEDs( leds );
    leds = leds ^ 0xff;
    delay(100);
  }
  delay(1000);

  // test 3
  leds = 0x01;
  for ( int i=0; i < 16; i++ ) {
    tm1638.setLEDs( leds );
    leds = (leds << 1) | (leds >> 7);
    delay(100);
  }
  tm1638.setLEDs( 0x00 );
  delay(1000);

  // test 4
  leds = 0x80;
  for ( int i=0; i < 16; i++ ) {
    tm1638.setLEDs( leds );
    leds = (leds >> 1) | ((leds << 7) & 0x80);
    delay(100);
  }
  tm1638.setLEDs( 0x00 );
  delay(1000);  
}

void test_display() {
  uint8_t digits[] = {1,2,3,4,5,6,7,8};
  tm1638.setDigits( digits );
  delay(2000);
  tm1638.setDigits( &digits[0], 4 /*len*/, 4 /*start*/ );
  delay(2000);
  tm1638.setDigits( &digits[4], 4 /*len*/, 0 /*start*/ );
  delay(2000);
  tm1638.setNumber( 55555555 );  
  delay(2000);  
}

void setup() {
  Serial.begin( 115200 );
  Serial.println( "\n\n\n" );
  delay(100);
  Serial.println( "TM1638-based board with 8x 7-segment display, LEDs and push buttons" );
  tm1638.init();
  test_leds();
  test_display();
  ts = millis();
}

void loop() {
  static uint32_t cnt = 0;
  if ( millis() - ts >= 100 ) { // increment the counter every 100 msec (0.1 sec)
     ts += 100;
     tm1638.setNumber( cnt++, 8 );  
     uint8_t value = tm1638.readButtons();
     if ( value != 0x00 ) {
       sprintf( sbuf, "Buttons: 0x%02x", value );
       Serial.println( sbuf );
     }
  }
}
//////////////////////////////////////////////////////////////////////////


