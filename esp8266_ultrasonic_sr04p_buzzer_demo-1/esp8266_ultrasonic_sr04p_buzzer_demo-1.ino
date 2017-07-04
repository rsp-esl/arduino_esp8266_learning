//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-04
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
// Note: use the HC SR04+ Ultrasonic module, Vcc=3.3V 
//////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>

// User-defined pin definitions for ESP8266 Board: WeMos D1 mini
#define TRIG_PIN    5     // GPIO-5 / D1 pin
#define ECHO_PIN    4     // GPIO-4 / D2 pin
#define BUZZER_PIN  14    // GPIO-14 / D5 pin

// Note:
// 1) We use the HC-SR04+ ultrasonic module that can operate at Vcc=3.3V.
// 2) The HC-SR04 ultrasonic module works only with Vcc=5V,
//    whereas HC-SR04+ module works with both 3.3V and 5V.
// 3) We also use an active buzzer for sound alarm, Vcc=3.3V.

const uint32_t timeout_usec = 40000;     // timeout in microseconds
const uint32_t sound_speed  = 34300;     // centimeters/second

#define BUZER_ACTIVE_LOW

#define MAX_PWM_PERIOD   100

#define ON_DUTY_CYCLE    (MAX_PWM_PERIOD/2)
#ifdef BUZZER_ACTIVE_LOW
 #define OFF_DUTY_CYCLE  (0)
#else
 #define OFF_DUTY_CYCLE  (MAX_PWM_PERIOD)
#endif

void setup() {
   Serial.begin( 115200 );
   Serial.println( "\n\n\n" );
   delay(100);
   Serial.println( "ESP8266 Ultrasonic HC-SR04+ and Buzzer Demo..." );
   pinMode( ECHO_PIN,   INPUT  );
   pinMode( TRIG_PIN,   OUTPUT );
   pinMode( BUZZER_PIN, OUTPUT );

   WiFi.disconnect();
   WiFi.mode(WIFI_OFF); // disable WiFi 
   
   analogWriteRange( MAX_PWM_PERIOD );
   analogWriteFreq(1); 
   analogWrite( BUZZER_PIN, ON_DUTY_CYCLE );
   delay(500);
   analogWrite( BUZZER_PIN, OFF_DUTY_CYCLE );
}

uint32_t read_distance() {
   // send a PING signal (a short-pulse signal on TRIG pin)
   digitalWrite( TRIG_PIN, HIGH );
   delayMicroseconds( 20 );
   digitalWrite( TRIG_PIN, LOW );
   // see: https://www.arduino.cc/en/Reference/pulseIn
   // measure pulse width of the ECHO signal
   uint32_t duration = pulseIn( ECHO_PIN, HIGH, timeout_usec );
   uint32_t distance_cm = (sound_speed * duration / 1000000) / 2;
   return distance_cm;
}

#define NUM_READINGS   4
#define DISTANCE_L0   10
#define DISTANCE_L1   40

void loop() {
   uint32_t distance = 500, value;

   for (int i=0; i < NUM_READINGS; i++ ) {
      value = read_distance();
      if ( value < distance ) { distance = value; }
   }
    
   String str = "Distance: ";
   str += distance;
   str += " cm";
   Serial.println( str );
   if ( distance > DISTANCE_L1 ) {
     analogWriteFreq(1);   
     analogWrite( BUZZER_PIN, OFF_DUTY_CYCLE );
   } else if ( distance < DISTANCE_L0 ) {
     analogWriteFreq(5); 
     analogWrite( BUZZER_PIN, ON_DUTY_CYCLE );
   } else {
     analogWriteFreq( map( (DISTANCE_L1-distance), 0, (DISTANCE_L1-DISTANCE_L0), 1, 5) );   
     analogWrite( BUZZER_PIN, ON_DUTY_CYCLE );
   }
   delay(200);
}

//////////////////////////////////////////////////////////////////////////
// t = (2*d)/v ; t = time duration, d = distance, v = sound speed
// => d = v*t/2 
//      = ( 343 [m/s] * t [s] )/ 2
//      = ( 34300 [cm/s] * t [s] )/ 2
//      = (34300 [cm/s] * t [usec]/10^6)/ / 2
//////////////////////////////////////////////////////////////////////////


