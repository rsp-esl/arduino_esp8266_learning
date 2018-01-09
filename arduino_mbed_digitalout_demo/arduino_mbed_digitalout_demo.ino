//////////////////////////////////////////////////////////////////////////////////////////
// Author: Rawat S. (KMUTNB, Bangkok / Thailand
// Date: 2018-01-09
//////////////////////////////////////////////////////////////////////////////////////////

#include "Arduino.h"
#include "DigitalOut.h"

#define wait_ms(dly)   delay(dly)

DigitalOut redLed( D5 );
DigitalOut blueLed( D6 );

void setup() {
    redLed  = 0;  // turn off the Red LED
    blueLed = 1;  // turn on the Blue LED
    wait_ms( 100 );
    Serial.begin( 115200 );
    Serial.printf( "Toggle two LEDs\n" );
    Serial.printf( "Initial states: Red=%d, Blue=%d\n", redLed.read(), blueLed.read() );
}

void loop() {
    redLed  = !redLed;  // toggle the Red LED pin
    blueLed = !blueLed; // toggle the Blue LED pin
    wait_ms( 100 ); // delay in msec
}

//////////////////////////////////////////////////////////////////////////////////////////

