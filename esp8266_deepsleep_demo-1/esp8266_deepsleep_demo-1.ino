// Target board: WeMos D1 mini
// see: https://wiki.wemos.cc/products:d1:d1_mini

#include <ESP8266WiFi.h>

#define DONE_PIN   D8           // with on-board pull-down
#define BTN_PIN    D3 
#define LED_PIN    BUILTIN_LED  // with on-board pull-up

// On WeMos D1 mini:
//  - On-board LED (active-low) is the GPIO2 / D4 pin.
//  - To enter boot mode, pull down the D3 pin and press the RESET button.
//  - Do not forget to connect the D0 pin (GPIO16) to the /RESET pin

int state = 0;

#define ONE_SEC              (1000000UL)
#define ONE_MINUTE           (60 * 1000000UL)

#define DEEP_SLEEP_10_SEC    (10 * ONE_SEC)
#define DEEP_SLEEP_15_SEC    (15 * ONE_SEC)
#define DEEP_SLEEP_30_SEC    (30 * ONE_SEC)
#define DEEP_SLEEP_60_SEC    (60 * ONE_SEC)

#define DEEP_SLEEP_1_SEC     ( 1 * ONE_MINUTE)
#define DEEP_SLEEP_5_MIN     ( 5 * ONE_MINUTE)
#define DEEP_SLEEP_10_MIN    (10 * ONE_MINUTE)
#define DEEP_SLEEP_15_MIN    (15 * ONE_MINUTE)
#define DEEP_SLEEP_30_MIN    (30 * ONE_MINUTE)
#define DEEP_SLEEP_60_MIN    (60 * ONE_MINUTE)

#define DEEP_SLEEP_FOREVER   (0)

void wifi_off() {
  WiFi.mode( WIFI_STA );
  WiFi.disconnect(); 
  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  delay(1);
}

void setup() {
  pinMode( BTN_PIN, INPUT_PULLUP );
  
  pinMode( DONE_PIN, OUTPUT );
  digitalWrite( DONE_PIN, LOW );
  
  pinMode( LED_PIN, OUTPUT );
  digitalWrite( LED_PIN, LOW ); // turn on LED

  wifi_off();

  for ( int i=0; i < 50; i++ ) { // blink the LED 
     digitalWrite( LED_PIN, state );
     state ^= 1;
     delay(50);
  }
  
  if ( digitalRead( BTN_PIN ) == HIGH ) { // if the button is not pressed.
     digitalWrite( LED_PIN, HIGH ); // turn off LED
     digitalWrite( DONE_PIN, HIGH );
     ESP.deepSleep( DEEP_SLEEP_5_MIN );
     delay(1);
  }
}

void loop() {
  digitalWrite( LED_PIN, state );
  state ^= 1;
  delay(100);
}
