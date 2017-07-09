//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-01
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
//////////////////////////////////////////////////////////////////////////

#include <string.h>
// for Windows 
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
// for Linux
//#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#include <ESP8266WiFi.h>

#define VALUE_MAX 255      // max. value (range: 0..VALUE_MAX) for PWM period

// constants
const int QA_PIN = 14;     // D5 = GPIO14 (QA or CLK pin of the rotary encoder switch)
const int QB_PIN = 12;     // D6 = GPIO12 (QB or DT  pin of the rotary encoder switch)
const int PWM_LED_PIN = 4; // D2 = GPIO4 (connected to LED, active-high)

// global variables
char sbuf[64];
volatile int last_a = 1, last_b = 1;
volatile int16_t encoder_cnt = 0;
volatile bool encoder_update = false;

// forward declaration
void encode_inputs();

void encode_inputs() {
  detachInterrupt( QA_PIN );
  int a = digitalRead(QA_PIN);
  int b = digitalRead(QB_PIN);
  if (( a != last_a ) || ( b != last_b ) ) {
    if ( a == b ) {
       encoder_cnt++;
    } else { 
       encoder_cnt--;
    }
    last_a = a;
    last_b = b;
    encoder_update = true;
    if ( encoder_cnt < 0 ) { encoder_cnt = 0; }
    if ( encoder_cnt > VALUE_MAX ) { encoder_cnt = VALUE_MAX; }
  }
  attachInterrupt( QA_PIN, encode_inputs, CHANGE );
}

void setup() {
  Serial.begin( 115200 );
  Serial.println( F("\n\n\n") );
  delay(100);
    
  pinMode( QA_PIN, INPUT );
  pinMode( QB_PIN, INPUT );
  attachInterrupt( QA_PIN, encode_inputs, CHANGE );
  pinMode( PWM_LED_PIN, OUTPUT );

  WiFi.disconnect();
  WiFi.mode(WIFI_OFF); // disable WiFi

  Serial.println( F("ESP8266 Rotary Encoder Demo") );
  sprintf_P( sbuf, PSTR("Sketch: '%s'"), __FILENAME__ );
  Serial.println( sbuf );
  Serial.println( "Build: " __DATE__ ", " __TIME__ );
  
  analogWriteFreq(500);          // set PWM frequency: 500 Hz
  analogWriteRange(VALUE_MAX+1); // set PWM duty cycle range 
}

void loop() {
  if ( encoder_update ) {
     encoder_update = false;
     if ( encoder_cnt >= 0 && encoder_cnt <= 255 ) {
        analogWrite( PWM_LED_PIN, encoder_cnt );
        sprintf_P( sbuf, PSTR("PWM duty cycle: %d"), encoder_cnt );
        Serial.println( sbuf );
     }
  }
  delay(1);
}
//////////////////////////////////////////////////////////////////////////

