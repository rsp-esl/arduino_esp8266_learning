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

// #define USE_ACTIVE_LOW

#ifdef USE_ACTIVE_LOW 
 #define ON  LOW
 #define OFF HIGH
#else
 #define ON  HIGH
 #define OFF LOW
#endif

void setup() {
   pinMode( RED_PIN,   OUTPUT );
   pinMode( GREEN_PIN, OUTPUT );
   pinMode( BLUE_PIN,  OUTPUT );
}

void loop() {
  // all LEDs (Red, Green, Blue) OFF
  digitalWrite( RED_PIN,   OFF );
  digitalWrite( GREEN_PIN, OFF );
  digitalWrite( BLUE_PIN,  OFF );
  delay(1000);
  // only Red ON
  digitalWrite( RED_PIN,   ON  );
  digitalWrite( GREEN_PIN, OFF );
  digitalWrite( BLUE_PIN,  OFF );
  delay(1000);
  // only Green ON
  digitalWrite( RED_PIN,   OFF );
  digitalWrite( GREEN_PIN, ON  );
  digitalWrite( BLUE_PIN,  OFF );
  delay(1000);
  // only Blue ON
  digitalWrite( RED_PIN,   OFF );
  digitalWrite( GREEN_PIN, OFF );
  digitalWrite( BLUE_PIN,  ON  );
  delay(1000);
  // all LEDs ON
  digitalWrite( RED_PIN,   ON  );
  digitalWrite( GREEN_PIN, ON  );
  digitalWrite( BLUE_PIN,  ON  );
  delay(1000);
  // Red and Green ON
  digitalWrite( RED_PIN,   ON  );
  digitalWrite( GREEN_PIN, ON  );
  digitalWrite( BLUE_PIN,  OFF );
  delay(1000);
  // Red and Blue ON
  digitalWrite( RED_PIN,   ON  );
  digitalWrite( GREEN_PIN, OFF );
  digitalWrite( BLUE_PIN,  ON  );
  delay(1000);
  // Green and Blue ON
  digitalWrite( RED_PIN,   OFF );
  digitalWrite( GREEN_PIN, ON  );
  digitalWrite( BLUE_PIN,  ON  );
  delay(1000);
}
////////////////////////////////////////////////////////////////////////////////


