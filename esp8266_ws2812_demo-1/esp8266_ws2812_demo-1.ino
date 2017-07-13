//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-03
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
//////////////////////////////////////////////////////////////////////////

#include "Adafruit_NeoPixel.h"  // use the Adafruit NeoPixel library
// see: https://github.com/adafruit/Adafruit_NeoPixel

// User-defined pin definitions for ESP8266 Board
#define DATA_PIN   D1     // GPIO-5 / D1 pin
// Note: use Vcc = 5V (or 3.3V, but this works in some cases) for the WS2812

#define NUM_PIXELS  1     // Number of WS2812 RGB LEDs: We use only one WS2812 LED. 
#define DELAY_MSEC  2000

// create Adafruit_NeoPixel object and specify the WS2812 type and clock frequency
Adafruit_NeoPixel pixels = Adafruit_NeoPixel( NUM_PIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800 );

// constants
const uint32_t TEST_COLORS [ ] = {
   pixels.Color(0,0,0),   // off
   pixels.Color(255,0,0), // red
   pixels.Color(0,255,0), // green
   pixels.Color(0,0,255), // blue
   pixels.Color(255,255,0),
   pixels.Color(0,255,255),
   pixels.Color(255,0,255),
};

const int NUM_TEST_COLORS = sizeof(TEST_COLORS) / sizeof(uint32);

// global variables
uint32_t ts;         // used to save timestamp
int color_index = 0; // used to select the test color
String str;
char sbuf[32];

void setup() {
   Serial.begin(115200);
   Serial.println("\n\n\n\n");
   pinMode( DATA_PIN, OUTPUT );
   
   Serial.println( "ESP8266 WS2812 Demo..." );
   pixels.begin();  // initialize the WS2812 module
   ts = millis();
   pixels.setPixelColor( 0 /*index of WS2812 */, 255 /*R*/, 255 /*G*/, 255 /*B*/ ); 
   pixels.show(); 
   delay(100);
   pixels.setPixelColor( 0 /*index of WS2812 */, 0 ); 
   delay(1000);
}

void loop() {   
  if ( millis() - ts >= DELAY_MSEC ) {
      ts += DELAY_MSEC;      
      pixels.setPixelColor( 0 /*index of WS2812 */, TEST_COLORS[color_index] /* color value */ ); 
      pixels.show(); 

      str = "Color index: ";
      str += color_index;
      str += ", color value (uint32_t): ";
      sprintf( sbuf, "%06X", pixels.getPixelColor( 0 ) );
      str += sbuf;
      Serial.println( str );
  
      color_index = (color_index+1) % NUM_TEST_COLORS;
  }
}
////////////////////////////////////////////////////////////////////////////////

