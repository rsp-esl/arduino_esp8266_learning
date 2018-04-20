////////////////////////////////////////////////////////////////////////////////
// Author: RSP
// IoT Engineering Education, KMUTNB
// Date: 2017-04-20
////////////////////////////////////////////////////////////////////////////////

#include "SI7021.h"

SI7021  si7021( 2 /* gpio2=d4 sda */, 0 /* gpio0=d3 scl */, 0x40 );
  
// global variables
uint32_t ts;
char sbuf[64];

void setup() {
   Serial.begin( 115200 );
   Serial.println( F("\n\n\n\n") );

   Serial.println( F("ESP8266 si7021 Reading...\r\n") );
   si7021.begin();
   delay(500);
   ts = millis();
}

int32_t temp, humid;

void loop() {
   if ( millis() - ts >= 1000 ) { 
       ts = millis();
       if ( si7021.readMeasurement() ) {
           temp = (int32_t)(100*si7021.getTemperature());
           humid = (int32_t)(100*si7021.getHumidity());
           sprintf( sbuf, "Temperature: %d.%02d deg.C, ", temp/100, temp%100 );
           Serial.print( sbuf );
           sprintf( sbuf, "Rel.Humidity: %d.%02d %%RH", humid/100, humid%100 );
           Serial.println( sbuf );
       } else {
          Serial.println( "Sensor reading failed!!!\r\n" );
       }
   }
}

////////////////////////////////////////////////////////////////////////////////

