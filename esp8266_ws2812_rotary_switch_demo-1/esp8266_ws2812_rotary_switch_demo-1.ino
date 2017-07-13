//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-13
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
//////////////////////////////////////////////////////////////////////////

#include "Adafruit_NeoPixel.h"  // use the Adafruit NeoPixel library
// see: https://github.com/adafruit/Adafruit_NeoPixel
// Note: use Vcc = 5V for the WS2812 strip

// User-defined pin definitions for ESP8266 Board
#define  QA_PIN     D5     // D5 = GPIO-14 pin
#define  QB_PIN     D6     // D6 = GPIO-12 pin
#define  DATA_PIN   D1     // D1 = GPIO-5 pin

#define NUM_PIXELS  8     // Number of WS2812 RGB LEDs
#define CNT_MIN     0
#define CNT_MAX     (2*NUM_PIXELS)
 
// create Adafruit_NeoPixel object and specify the WS2812 type and clock frequency
Adafruit_NeoPixel pixels = Adafruit_NeoPixel( NUM_PIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800 );

const uint32_t COLOR_OFF = pixels.Color(0,0,0);
const uint32_t COLOR_RED_HALF = pixels.Color(127,0,0);

// global variables
int active_index=0;
char sbuf[64];

volatile int     last_a = 1, last_b = 1;
volatile int16_t encoder_cnt = 0;
volatile bool    encoder_update = false;

// forward declaration
void encode_inputs();

void encode_inputs() {
  detachInterrupt( QA_PIN );
  int a = digitalRead(QA_PIN);
  int b = digitalRead(QB_PIN);
  if (( a != last_a ) || ( b != last_b ) ) {
     if ( a == b ) {
       if ( encoder_cnt < CNT_MAX ) {
          encoder_cnt++;
          encoder_update = true;
       }
    } else { 
       if ( encoder_cnt > CNT_MIN ) {
         encoder_cnt--;
         encoder_update = true;
       }
    }
    last_a = a;
    last_b = b;
  }
  attachInterrupt( QA_PIN, encode_inputs, CHANGE );
}

void setup() {
   Serial.begin(115200);
   Serial.println("\n\n\n\n");
   
   pinMode( DATA_PIN, OUTPUT );
   pinMode( QA_PIN, INPUT );
   pinMode( QB_PIN, INPUT );
   attachInterrupt( QA_PIN, encode_inputs, CHANGE ); // use QA_PIN for external interrupt
   
   Serial.println( "ESP8266 WS2812 + Rotary Switch Demo..." );
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
  if ( encoder_update ) {
     encoder_update = false;
     sprintf( sbuf, "Encoder Counter: %d", encoder_cnt );
     Serial.println( sbuf );
     active_index = (encoder_cnt/2)-1;
     for ( int i=0; i < NUM_PIXELS; i++ ) {
        pixels.setPixelColor( i, (i <= active_index) ? COLOR_RED_HALF : COLOR_OFF ); 
     }
     pixels.show();  // update the color of the WS2812 strip
  }
  delay( 5 );
}
////////////////////////////////////////////////////////////////////////////////

