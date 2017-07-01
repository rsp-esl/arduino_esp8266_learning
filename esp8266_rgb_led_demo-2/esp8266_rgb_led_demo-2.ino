//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-01
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
//////////////////////////////////////////////////////////////////////////

// User-defined pin definitions for ESP8266 Board: WeMos D1 mini
#define RED_PIN    14   // GPIO-14 / D5 pin
#define BLUE_PIN   12   // GPIO-12 / D6 pin
#define GREEN_PIN  13   // GPIO-13 / D7 pin

//#define USE_ACTIVE_LOW

#ifdef USE_ACTIVE_LOW 
 #define ON  LOW
 #define OFF HIGH
#else
 #define ON  HIGH
 #define OFF LOW
#endif

const int dly = 1000; // in msec

void update_color( int r, int g, int b, int delay_ms ) {
  digitalWrite( RED_PIN,   r );
  digitalWrite( GREEN_PIN, g );
  digitalWrite( BLUE_PIN,  b );
  delay( delay_ms );
}

void setup() {
   pinMode( RED_PIN,   OUTPUT );
   pinMode( GREEN_PIN, OUTPUT );
   pinMode( BLUE_PIN,  OUTPUT );
}

void loop() {
  update_color( OFF, OFF, OFF, dly );   // all LEDs OFF
  update_color( ON,  OFF, OFF, dly );   // only Red ON
  update_color( OFF,  ON, OFF, dly );   // only Green ON
  update_color( OFF, OFF,  ON, dly );   // only Blue ON
  update_color(  ON,  ON,  ON, dly );   // all LEDs ON
  update_color(  ON,  ON, OFF, dly );   // Red and Green ON
  update_color(  ON, OFF,  ON, dly );   // Red and Blue ON
  update_color( OFF,  ON,  ON, dly );   // Green and Blue ON
}
//////////////////////////////////////////////////////////////////////////


