#include "AM2315.h"

AM2315::AM2315() {
}

boolean AM2315::begin(void) {
  return readData();
}

boolean AM2315::readSensor() {
  if (!readData()) return false;
  return true;
}

float AM2315::getHumidity(void) {
   return humidity;
}

float AM2315::getTemperature(void) {
   return temperature;
}

boolean AM2315::readData(void) {
  uint8_t reply[10];
  
  // wakeup the sensor
  Wire.beginTransmission(AM2315_I2CADDR);
  delay(2);
  Wire.endTransmission();

  // Start the request with the sensor
  Wire.beginTransmission( AM2315_I2CADDR );
  Wire.write( AM2315_READREG );
  Wire.write(0x00);  // start at address 0x0
  Wire.write(4);  // request 4 bytes data
  Wire.endTransmission();
  
  delay(10); // add delay between request and read

  Wire.requestFrom(AM2315_I2CADDR, 8);
  for (uint8_t i=0; i<8; i++) {
    reply[i] = Wire.read();
  }
  
  if (reply[0] != AM2315_READREG) return false;
  if (reply[1] != 4) return false; // not enough data bytes supplied

  humidity = reply[2];
  humidity *= 256;
  humidity += reply[3];
  humidity /= 10;
    
  temperature = reply[4] & 0x7F;
  temperature *= 256;
  temperature += reply[5];
  temperature /= 10;
    
  // change sign
  if (reply[4] >> 7) temperature = -temperature;

  return true;
}
