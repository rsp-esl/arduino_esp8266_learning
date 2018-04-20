////////////////////////////////////////////////////////////////////////////////
// Author: RSP
// IoT Engineering Education, KMUTNB
// Date: 2017-04-18
////////////////////////////////////////////////////////////////////////////////

#include "SHT3x.h"

SHT3x      sht3x( 2 /* gpio2=d4 sda */, 0 /* gpio0=d3 scl */, 0x44 );
  
// global variables
uint32_t ts;
char sbuf[64];

void setup() {
   Serial.begin( 115200 );
   Serial.println( F("\n\n\n\n") );

   Serial.println( F("ESP-01 SHT31 Reading...\r\n") );
   sht3x.begin();
   sht3x.startMeasurement(); 
   delay(500);
   ts = millis();
}

int32_t temp, humid;
uint8_t cnt = 0;

void loop() {
   if ( millis() - ts >= 1000 ) { 
       ts = millis();
       if ( cnt == 0 ) {
           // start the SHT3x measurement process
           sht3x.startMeasurement();
       } else {
           sht3x.readMeasurement();
           if ( !sht3x.isValid() ) {
              Serial.println( "Sensor reading error!!!\r\n" );
           }
           else {
              temp = (int32_t)(100*sht3x.getTemperature());
              humid = (int32_t)(100*sht3x.getHumidity());
              sprintf( sbuf, "Temperature=%d.%02d deg.C, ", temp/100, temp%100 );
              Serial.print( sbuf );
              sprintf( sbuf, "Rel.Humidity=%d.%02d %%RH", humid/100, humid%100 );
              Serial.println( sbuf );
           }
       }
       cnt = (cnt+1) % 2;
   }
}

////////////////////////////////////////////////////////////////////////////////

