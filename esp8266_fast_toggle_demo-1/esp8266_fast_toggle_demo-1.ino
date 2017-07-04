//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-04
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// MCU Boards with ESP-12E
//////////////////////////////////////////////////////////////////////////

const int TEST_PIN = 5;   // D1 or GPIO-5 (or any GPIO pin between 0..15)
const int TEST_PIN_BITS = (1<< TEST_PIN);

void setup() {
  pinMode( TEST_PIN, OUTPUT );
}

// CPU freq: 80MHz
// loop execution time (approx.): 6.5 usec
// pulse width (approx.) = 88 ns

void loop() {
  noInterrupts();
  for ( int i=0; i < 5; i++ ) { // create five pulses per loop
    // see: https://github.com/esp8266/Arduino/blob/master/cores/esp8266/esp8266_peri.h
    GPOS = TEST_PIN_BITS;  // set bit  
    GPOC = TEST_PIN_BITS;  // clear bit
  }
  interrupts();
}
//////////////////////////////////////////////////////////////////////////

