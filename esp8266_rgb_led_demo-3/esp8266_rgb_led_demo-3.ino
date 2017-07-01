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

#define USE_ACTIVE_LOW

const int dly = 10; // in msec

// r,g,b must be integers between 0 and 1023
void update_color( int r, int g, int b, int delay_ms ) {
  
#ifdef USE_ACTIVE_LOW
  analogWrite( RED_PIN,   1023-r );
  analogWrite( GREEN_PIN, 1023-g );
  analogWrite( BLUE_PIN,  1023-b );
#else
  analogWrite( RED_PIN,   r );
  analogWrite( GREEN_PIN, g );
  analogWrite( BLUE_PIN,  b );
#endif
  delay( delay_ms );
}

void setup() {
   pinMode( RED_PIN,   OUTPUT );
   pinMode( GREEN_PIN, OUTPUT );
   pinMode( BLUE_PIN,  OUTPUT );
   analogWriteFreq( 500 ); // set PWM frequency to 500Hz
}

void loop() {
  for ( int r=0; r < 1024; r+=4 ) {
    update_color( r, 0, 0, dly );
  }
  for ( int g=0; g < 1024; g+=4 ) {
    update_color( 0, g, 0, dly );
  }
  for ( int b=0; b < 1024; b+=4 ) {
    update_color( 0, 0, b, dly );
  }
}
//////////////////////////////////////////////////////////////////////////


