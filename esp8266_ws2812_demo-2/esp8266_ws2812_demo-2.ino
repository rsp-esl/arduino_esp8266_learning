//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-17
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
//////////////////////////////////////////////////////////////////////////

#include "Adafruit_NeoPixel.h"  // use the Adafruit NeoPixel library
// see: https://github.com/adafruit/Adafruit_NeoPixel

// User-defined pin definitions for ESP8266 Board: WeMos D1 mini
#define DATA_PIN   D1     // GPIO-5 / D1 pin
// Note: use Vcc = 5V for the WS2812 strip

#define NUM_PIXELS  8     // Number of WS2812 RGB LEDs

// create Adafruit_NeoPixel object and specify the WS2812 type and clock frequency
Adafruit_NeoPixel pixels = Adafruit_NeoPixel( NUM_PIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800 );

const uint32_t COLOR_OFF = pixels.Color(0,0,0);

const uint32_t TEST_COLORS [ ] = {
   pixels.Color(255,0,0), // red
   pixels.Color(0,255,0), // green
   pixels.Color(0,0,255), // blue
   pixels.Color(255,255,0),
   pixels.Color(0,255,255),
   pixels.Color(255,0,255)
};

const int NUM_TEST_COLORS = sizeof(TEST_COLORS) / sizeof(uint32);

// global variables
int active_index=0;
int color_index=0;

void setup() {
   Serial.begin(115200);
   Serial.println("\n\n\n\n");
   pinMode( DATA_PIN, OUTPUT );
   
   Serial.println( "ESP8266 WS2812 Demo..." );
   pixels.begin();  // initialize the WS2812 module
   
   for (int i=0; i < NUM_PIXELS; i++ ) {
     pixels.setPixelColor( i /*index of WS2812 */, 255 /*R*/, 255 /*G*/, 255 /*B*/ ); 
   }
   pixels.show();   // update the color of the WS2812 strip
   delay(100);
   for (int i=0; i < NUM_PIXELS; i++ ) {
     pixels.setPixelColor( i /*index of WS2812 */, COLOR_OFF ); 
   }   
   pixels.show();   // update the color of the WS2812 strip
   delay(1000);
}

void loop() {   
  for ( int i=0; i < NUM_PIXELS; i++ ) {
     pixels.setPixelColor( i, (i <= active_index) ? TEST_COLORS[color_index] : COLOR_OFF ); 
  }
  pixels.show();  // update the color of the WS2812 strip
  
  if ( active_index == NUM_PIXELS-1 ) {
     color_index = (color_index+1) % NUM_TEST_COLORS;
     active_index = 0;
  } else {
     active_index++;
  }
  delay( 200 );
}
////////////////////////////////////////////////////////////////////////////////

