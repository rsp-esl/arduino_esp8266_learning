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

// Note:
// 1) We use the HC-SR04+ ultrasonic module that can operate at Vcc=3.3V.
// 2) The HC-SR04 ultrasonic module works only with Vcc=5V,
//    whereas the HC-SR04+ module works with both 3.3V and 5V.

const uint32_t timeout_usec = 40000;     // timeout in microseconds
const uint32_t sound_speed  = 34300;     // in centimeters/second

void setup() {
   Serial.begin( 115200 );
   Serial.println( F("\n\n\n") );
   delay(100);
   Serial.println( F("ESP8266 Ultrasonic SR04+ Demo...") );
   pinMode( ECHO_PIN, INPUT  );
   pinMode( TRIG_PIN, OUTPUT );
}

String str;

void loop() {
   // send a PING signal (a short-pulse signal on TRIG pin)
   digitalWrite( TRIG_PIN, HIGH );
   delayMicroseconds( 20 );
   digitalWrite( TRIG_PIN, LOW );
   
   // see: https://www.arduino.cc/en/Reference/pulseIn
   uint32_t duration = pulseIn( ECHO_PIN, HIGH, timeout_usec );
   str = F("Duration: ");
   str += duration;
   str += F(" usec, ");
   str += F("Distance: ");
   str += (sound_speed * duration / 1000000) / 2;
   str += F(" cm");
   Serial.println( str );
   
   delay(500);
}
//////////////////////////////////////////////////////////////////////////
// t = (2*d)/v ; t = time duration, d = distance, v = sound speed
// => d = v*t/2 
//      = ( 343 [m/s] * t [s] )/ 2
//      = ( 34300 [cm/s] * t [s] )/ 2
//      = (34300 [cm/s] * t [usec]/10^6)/ / 2
//////////////////////////////////////////////////////////////////////////

