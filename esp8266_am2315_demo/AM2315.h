// based on https://github.com/adafruit/Adafruit_AM2315/

#include <Arduino.h>
#include <Wire.h>

#define AM2315_I2CADDR       0x5C
#define AM2315_READREG       0x03

class AM2315 {
  public:
      AM2315();
      boolean begin(void);
      boolean readSensor();
      float getHumidity(void);
      float getTemperature(void);
  
 private:
      boolean readData(void);
      float humidity;
      float temperature;
};

