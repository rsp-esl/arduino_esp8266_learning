////////////////////////////////////////////////////////////////////////////////
// ESP8266 Hands-on Workshop
// by Embedded Systems Lab (ESL) @ KMUTNB, Bangkok / Thailand
// Date: 2017-05-22
////////////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include "AM2320.h" // see: https://github.com/hibikiledo/AM2320

// AM2320 Sensor 
// Front view (left-to-right): VCC (3.3V), SDA, GND, SCL

#define SDA_PIN 5   // GPIO-5
#define SCL_PIN 4   // GPIO-4

AM2320 sensor;

void setup() {
  // enable serial communication
  Serial.begin(115200);
  // call sensor.begin() to initialize the library
  sensor.begin( SDA_PIN, SCL_PIN );
}

void loop() {


  if ( sensor.measure() ) { // ok
    Serial.print("Temperature: ");
    Serial.println(sensor.getTemperature());
    Serial.print("Humidity: ");
    Serial.println(sensor.getHumidity());
  }
  else { // error has occured
    int err_code = sensor.getErrorCode();
    switch (err_code) {
      case 1: Serial.println("ERR: Sensor is offline"); break;
      case 2: Serial.println("ERR: CRC validation failed."); break;
    }    
  }
  delay(500);
}
/////////////////////////////////////////////////////////////////////////////////////////

