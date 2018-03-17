////////////////////////////////////////////////////////////////////////////////
// Author: RSP
// IoT Engineering Education, KMUTNB
// Date: 2017-03-17
////////////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include "TM1637Display.h"
#include "NTPDateTime.h"

// You need to connect the ESP8266 to the Internet via a Wireless Access Point.
const char WIFI_SSID[] = "XXXXXXXXXXX";     // <======
const char WIFI_PASS[] = "XXXXXXXXXXX";           // <======

#if defined(ESP8266) 
  #define TM1637_CLK   4     // GPIO-4 = D2
  #define TM1637_DIO   5     // GPIO-5 = D1
#else
  #error "This sketch was written for ESP8266 (tested with WeMoS D1 min board.."
#endif

NTPDateTime    datetime;
TM1637Display  disp;

// global variables
uint32_t ts;
uint8_t digits[] = { 0,0,0,0 };
uint16_t hh, mm, ss, wday, mday, month, year;
String str;

void connect_wifi() {
   WiFi.mode( WIFI_STA );
   WiFi.begin(  WIFI_SSID, WIFI_PASS );
   Serial.print( F("Connecting WiFi") );
   while ( WiFi.status() != WL_CONNECTED ) {
      delay(500);
      Serial.print( '.' );
   }
   Serial.println( ' ' );  
   Serial.println( F("WiFi connected") );
   Serial.print( F("IP address: ") );
   Serial.println( WiFi.localIP() );
}

void setup() {
   Serial.begin( 115200 );
   Serial.println( F("\n\n\n\n") );
  
   connect_wifi();  
   disp.begin( TM1637_CLK, TM1637_DIO ); 
   disp.showDigits( digits );

   datetime.connect();
   Serial.print( F("Update localtime from NTP server") );
   year = 1970;
   while ( year == 1970 ) {
       datetime.getDate( year, month, mday, wday );
       Serial.print('.');
       disp.turnOff();
       delay(500);
       disp.turnOn();
       delay(500);
   }
   Serial.println( '.' );
   ts = millis();
}

void loop() {
   static uint8_t cnt = 0;
   if ( millis() - ts >= 500 ) {
      ts = millis();
      if (cnt==0) {
         datetime.getDateTimeString( str );
         Serial.println( str) ;
         datetime.getTime( hh, mm, ss );
         digits[0] = hh/10;
         digits[1] = hh%10;
         digits[2] = mm/10;
         digits[3] = mm%10;
      }
      disp.showDigits( digits, (cnt==0) ? true : false );
      cnt = (cnt+1) % 2;
   }
}
////////////////////////////////////////////////////////////////////////////////

