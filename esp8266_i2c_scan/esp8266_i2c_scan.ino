#include <Wire.h> // use the Wire library

char sbuf[32];

// GPIO0/D3 = SCL
// GPIO4/D2 = SDA

#define SCL_PIN  0
#define SDA_PIN  4

void i2c_scan() {
  int count = 0;
  Serial.println( "Scanning I2C slave devices..." );
  for( byte addr=0x01; addr <= 0x7f; addr++ ) {
     Wire.beginTransmission( addr ); 
     if ( Wire.endTransmission() == 0 ) {
       sprintf( sbuf, "I2C device found at 0x%02X.", addr );
       Serial.println( sbuf );
       count++;
    }
  }
  if (count > 0 ) {
    sprintf( sbuf, "Found %d I2C devices.", count );
  } else {
    sprintf( sbuf, "No I2C device found." );
  }
  Serial.println( sbuf );
}

// Uno or 328P:  A4 for SDA and A5 for the SCL line of the I2C bus.
void setup() {
  Serial.begin( 115200 ); // set baudrate
  delay(1000);
}

void loop() {
  Wire.begin( SDA_PIN, SCL_PIN );
  Wire.setClock(100000);
  i2c_scan();
  delay(3000);
}

