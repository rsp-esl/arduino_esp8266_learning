//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-05
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
//////////////////////////////////////////////////////////////////////////

#include <MD5Builder.h>

MD5Builder md5;

void setup() {
   Serial.begin( 115200 );
   Serial.println( "\n\n\n" );
   delay(100);
   Serial.println( "ESP8266 MD5 calculation..." );
}

#define NUM_MESSAGES (4)

const char *TEST_MESSAGES[] = {
  "The quick brown fox jumps over the lazy dog",
  "0123456789",
  "apple",
  "",
};

const char *MD5_HEX_STRINGS[] = {
  "9e107d9d372bb6826bd81d3542a419d6",
  "781e5e245d69b566979b86e28d23f2c7",
  "1f3870be274f6c49b3e31a0c6728957f",
  "d41d8cd98f00b204e9800998ecf8427e",
};

int msg_index = 0;
char sbuf[64];

void loop() {  
   String str = TEST_MESSAGES[ msg_index ]; 
   
   md5.begin();
   md5.add( str );
   md5.calculate();
   
   sprintf( sbuf, "md5(\"%s\") = ", str.c_str() );
   Serial.print( sbuf );

   String md5_str = md5.toString();
   Serial.print( md5_str );
   if ( md5_str.equals(MD5_HEX_STRINGS[ msg_index ]) ) {
     Serial.println( " OK" );
   } else {
     Serial.println( " MISMATCH" );
   }
   msg_index = (msg_index+1) % NUM_MESSAGES;
   delay(3000);
}
//////////////////////////////////////////////////////////////////////////

