// see: https://github.com/ControlEverythingCommunity/SHT30/blob/master/Arduino/SHT30.ino

#include <Wire.h>
#include <ESP8266WiFi.h>

// SHT3x-DIS (I2C)
#define SHT30_I2C_ADDR 0x44

#define SCL_PIN        (D1)
#define SDA_PIN        (D2)  

char sbuf[64];
uint32_t ts;
float temp, humid;
boolean data_valid;

void wifi_off() {
  WiFi.mode( WIFI_STA );
  WiFi.disconnect(); 
  WiFi.mode( WIFI_OFF );
  WiFi.forceSleepBegin();
  delay(1);
}

uint8_t i2c_scan( ) {
  uint8_t error;
  uint8_t start_addr = 0x01;
  uint8_t end_addr   = 0x7f;
  int founds;
  
  founds = 0;
  for ( uint8_t addr = start_addr; addr < end_addr; addr++ ) {
    Wire.beginTransmission(addr);
    error = Wire.endTransmission();
    if ( error == 0 ) {
      sprintf( sbuf, "Found a device at 0x%02X", addr );
      Serial.println( sbuf );
      founds++;
    }
    yield();
  }
  
  if ( founds == 0 ) {
    Serial.println( F("No I2C devices found") );
  } 
  else {
    Serial.printf( "Scan done, %d device(s) found\r\n", founds );
  }
  return founds;
}

void setup() {
  wifi_off();
  Serial.begin(115200);
  Serial.println( F("\n\n\n\n") );
  Serial.flush();
  delay(100);

  Wire.begin( SDA_PIN, SCL_PIN );
  Wire.setClock(100000);
  i2c_scan();
  ts = millis();
}

// CRC-8, Polynomial: 0x31  x^8 + x^5 + x^4 + 1, Initial value = 0xff

bool CRC8(uint8_t MSB, uint8_t LSB, uint8_t CRC) {
  uint8_t crc = 0xFF;

  crc ^= MSB;
  for (uint8_t i=0; i < 8; i++) {
     crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
  }
  
  crc ^= LSB;
  for (uint8_t i=0; i < 8; i++) {
     crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
  }
  return (crc==CRC) ? true : false;
}

void updateSensorReading() {
  unsigned int data[6];
  
  // Start I2C Transmission
  Wire.beginTransmission( SHT30_I2C_ADDR );
  // Send measurement command
  Wire.write(0x2C);
  Wire.write(0x06);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(500);
  // Request 6 bytes of data
  Wire.requestFrom(SHT30_I2C_ADDR, 6); // request 6 bytes
  if ( Wire.available() == 6 ) {
    data[0] = Wire.read(); // temperature Celsius (MSB)
    data[1] = Wire.read(); // temperature Celsius (LSB)
    data[2] = Wire.read(); // temperature Celsius CRC
    data[3] = Wire.read(); // relative humidty (%RH) (MSB)
    data[4] = Wire.read(); // relative humidty (%RH) (LSB)
    data[5] = Wire.read(); // relative humidty CRC
  }

  // check CRC for both values
  if ((CRC8(data[0],data[1],data[2])) && (CRC8(data[3],data[4],data[5]))) {
    uint16_t value;
    value = (data[0] << 8) + data[1];
    temp  = ((value * 175.0) / 65535) - 45; // temperature in Celsius.
    value = (data[3] << 8) + data[4];
    humid = ((value * 100.0) / 65535); // relative humidity
    data_valid = true;
  } else {
    data_valid = false;
  }  
}

void loop() {

  if ( millis() - ts >= 1000 ) {
    ts += 1000;
    updateSensorReading();
    if ( data_valid ) {
      Serial.print("Relative Humidity: ");
      Serial.print(humid);
      Serial.println(" %RH");
      Serial.print("Temperature [Celsius]: ");
      Serial.print(temp);
      Serial.println(" C");
    } 
    else {
      Serial.println("Sensor reading error: CRC failed...");
    }
  }
}
///////////////////////////////////////////////////////////////////////////


