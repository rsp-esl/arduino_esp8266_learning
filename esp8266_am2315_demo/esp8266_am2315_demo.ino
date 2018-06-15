///////////////////////////////////////////////////////////////////////////////////
// IoT Engineering Education @ KMUTNB, Bangkok/Thailand
// Date: 2018-06-15
///////////////////////////////////////////////////////////////////////////////////
// ESP8266 Demo: I2C AM2315 Digital Temperature & Relative Humdity Sensor
// Vcc: 3.3V to 5.5V,  5V recommended
// I2C address: 0x5C (default)
// Temperature Range : -40 ..+125 deg.C
// Humidity Range    : 0..99.9 %RH
// Temperature
//  - Resolution: 16-bit
//  - Accuracy: +/-0.5 deg.C
//  Relative Humidity
//  - Resolution: 16-bit resolution
//  - Accurancy +/- 2%RH @25 deg.C
// Sampling period: 2sec
// Wire Colors:
//  - Red    : VCC (3.3V to 5.5V)
//  - Yellow : SDA
//  - Black  : GDN
//  - White  : SCL
// Pull-up resistors: 10kOhm on SDA and SCL pins
///////////////////////////////////////////////////////////////////////////////////

#include <Wire.h> // use the Wire library
#include "AM2315.h"

// GPIO0/D3 = SCL
// GPIO4/D2 = SDA
#define SCL_PIN  0
#define SDA_PIN  4

AM2315 am2315;
char sbuf[80];
uint32_t cnt = 1;

void setup() {
  Serial.begin( 115200 ); // set baudrate
  Wire.begin( SDA_PIN, SCL_PIN );
  Wire.setClock( 100000 );  // set I2C clock frequency to 100kHz
  delay(100);

  Serial.println( "\n\nReading AM2315 sensor ...\n\n" );

  if ( !am2315.begin() ) {
     Serial.println( "Sensor not found!" );
     while (1) delay(1);
  }
  delay(2000);
}

void loop() {
  if ( am2315.readSensor() ) {
     int16_t h = 10*am2315.getHumidity();
     int16_t t = 10*am2315.getTemperature();
     sprintf( sbuf, "%d) Humid: %d.%1d, Temp: %d.%1d deg*C\n",
              cnt++, (h/10), (h%10), (t/10), (t%10) ); 
     Serial.println( sbuf );
  } else {
     Serial.println( "Sensor reading error!" );
  }
  delay(3000);
}

///////////////////////////////////////////////////////////////////////////////////

