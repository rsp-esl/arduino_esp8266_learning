//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-01
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
//////////////////////////////////////////////////////////////////////////

// User-defined pin definitions for ESP8266 Board: WeMos D1 mini (with ESP12)
// see: https://www.wemos.cc/product/d1-mini.html
// schematic: https://www.wemos.cc/sites/default/files/2016-09/mini_new_V2.pdf

#define LED_PIN   2     // GPIO-2 or D9 pin (built-in LED on ESP-12E module)

// global variable
int state = 0;

void setup() {
   Serial.begin( 115200 );
   Serial.println( "\n\n\n\n" );
   pinMode( LED_PIN, OUTPUT );
}

void loop() {
   Serial.println( state );  // show the current value of the state variable
   digitalWrite( LED_PIN, state ); // update the output using the state variable
   state ^= 1;   // toggle the state variable
   delay(500);   
}

//////////////////////////////////////////////////////////////////////////
