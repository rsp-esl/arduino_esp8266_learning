//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-05
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
//////////////////////////////////////////////////////////////////////////

#include <memory>

// global variable
char sbuf[64];  // used for sprintf();

const char *FLASH_MODES[] = { "QIO", "QOUT", "DIO", "DOUT" };

void setup() {
  Serial.begin( 115200 );
  Serial.println( "\n\n\n" );
  delay(100);

  // see: https://github.com/esp8266/Arduino/blob/master/cores/esp8266/Esp.cpp

  Serial.println("==============================================");
  Serial.println( "ESP8266 Info" );
  Serial.println("----------------------------------------------");
  
  String str;
  str = "ESP ChipID: ";
  str += ESP.getChipId();
  Serial.println( str );

  str = "ESP core version: ";
  str += ESP.getCoreVersion();
  Serial.println( str );

  str = "ESP CPU freq: ";
  str += ESP.getCpuFreqMHz();
  str += " MHz";
  Serial.println( str );

  str = "ESP flash chip speed: ";
  str += ESP.getFlashChipSpeed()/1000000;
  str += " MHz";
  Serial.println( str );

  str = "ESP flash chip mode: ";
  str += FLASH_MODES[ ESP.getFlashChipMode() ];
  Serial.println( str );

  str = "ESP flash chip size (bytes): ";
  str += ESP.getFlashChipSize();
  Serial.println( str );

  str = "ESP sketch size (bytes): ";
  str += ESP.getSketchSize();
  Serial.println( str );

  str = "ESP free heap (bytes): ";
  str += ESP.getFreeHeap();
  Serial.println( str );
  Serial.println("----------------------------------------------");
}

#define ALLOC_MEM_SIZE  1024
size_t len = ALLOC_MEM_SIZE;

void loop() {
  if ( ESP.getFreeHeap() > (len+ALLOC_MEM_SIZE) ) {
     len += ALLOC_MEM_SIZE;
  } else {
     delay(1000);
     return;
  }

  // Memory allocation test
  // use std::unique_ptr to manage an array of chars
  std::unique_ptr<char[]> buf( new char[len] );
  sprintf( sbuf, "create a buffer of %u bytes", len );
  Serial.println( sbuf );

  sprintf( sbuf, "Free Heap: %u bytes", ESP.getFreeHeap() );
  Serial.println( sbuf );
  
  buf.reset();  // release buffer
  
  Serial.println( "Clear buffer..." );
  sprintf( sbuf, "Free Heap: %u bytes", ESP.getFreeHeap() );
  Serial.println( sbuf );
  Serial.println("----------------------------------------------");
  delay(5000);
}
//////////////////////////////////////////////////////////////////////////


