//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-01
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
// Note: use the SR04+ Ultrasonic module, Vcc=3.3V 
//////////////////////////////////////////////////////////////////////////

// User-defined pin definitions for ESP8266 Board: WeMos D1 mini
#define TRIG_PIN   5    // GPIO-5 / D1 pin
#define ECHO_PIN   4    // GPIO-4 / D2 pin

#define RED_PIN    14   // GPIO-14 / D5 pin
#define BLUE_PIN   12   // GPIO-12 / D6 pin
#define GREEN_PIN  13   // GPIO-13 / D7 pin

#define  USE_ACTIVE_LOW // Common-Anode LEDs

#ifdef USE_ACTIVE_LOW
 #define ON  LOW
 #define OFF HIGH
#else
 #define ON  HIGH
 #define OFF LOW
#endif

// Note:
// 1) We use the HC-SR04+ ultrasonic module that can operate at Vcc=3.3V.
// 2) The HC-SR04 ultrasonic module works only with Vcc=5V,
//    whereas HC-SR04+ module works with both 3.3V and 5V.

const uint32_t timeout_usec = 40000;     // timeout in microseconds
const uint32_t sound_speed  = 34300;      // centimeters/second

void setup() {
   Serial.begin( 115200 );
   pinMode( ECHO_PIN,  INPUT  );
   pinMode( TRIG_PIN,  OUTPUT );
   pinMode( RED_PIN,   OUTPUT );
   pinMode( GREEN_PIN, OUTPUT );
   pinMode( BLUE_PIN,  OUTPUT );
}

void set_color( int r, int g, int b ) {
  digitalWrite( RED_PIN,   r );
  digitalWrite( GREEN_PIN, g );
  digitalWrite( BLUE_PIN,  b );
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

#define NUM_READINGS  (4)

void loop() {
   uint32_t distance = read_distance();
   if ( distance >= 50 || distance == 0 ) { 
      set_color( OFF, ON, OFF );
   } else if ( distance >= 30 ) {
      set_color( ON, ON, OFF );
   } else {
      set_color( ON, OFF, OFF );
   }
   Serial.println( distance );
   delay(250);
}

//////////////////////////////////////////////////////////////////////////
// t = (2*d)/v ; t = time duration, d = distance, v = sound speed
// => d = v*t/2 
//      = ( 343 [m/s] * t [s] )/ 2
//      = ( 34300 [cm/s] * t [s] )/ 2
//      = (34300 [cm/s] * t [usec]/10^6)/ / 2
//////////////////////////////////////////////////////////////////////////


