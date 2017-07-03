//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-03
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
// Note: use Vcc=3.3V for PCF8574-based 8-bit I/O expander module
//////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>   // use the WiFi library
#include <Wire.h>          // use the Wire library (Soft I2C for ESP8266)

// see: http://www.ti.com/lit/ds/symlink/pcf8574.pdf
//  Vcc 2.5V to 6V, f_SCL 100kHz max., Sink Current (I_OL) 25mA max.

#define I2C_SCL_PIN      (4)      // D2 pin (SCL / GPIO-4)
#define I2C_SDA_PIN      (0)      // D3 pin (SDA / GPIO-0)
#define I2C_ADDR        0x38      // set the I2C address of the PCF8574(A) chip 

#define DELAY_MSEC      200

#define USE_PATTERN_2             // use USE_PATTERN_1 or USE_PATTERN_2

// global variables
char sbuf[64];                    // used for sprintf()
uint32_t ts;                      // used to save timestamp value

void i2c_scan() {
  int count = 0;
  Serial.println( "\n\nScanning I2C slave devices..." );
  for( uint8_t addr=0x01; addr <= 0x7f; addr++ ) {
     Wire.beginTransmission( addr ); 
     if ( Wire.endTransmission() == 0 ) {
       sprintf( sbuf, "I2C device found at 0x%02X.", addr );
       Serial.println( sbuf );
       count++;
    }
  }
  if ( count > 0 ) {
    sprintf( sbuf, "Found %d I2C device(s).\n\n", count );
  } else {
    sprintf( sbuf, "No I2C device found.\n\n" );
  }
  Serial.println( sbuf );
}

inline void i2c_write_byte( uint8_t i2c_addr, uint8_t data ) {
  Wire.beginTransmission( i2c_addr );
  Wire.write( data );
  Wire.endTransmission();
}

inline uint8_t i2c_read_byte( uint8_t i2c_addr  ) {
  Wire.beginTransmission( i2c_addr );
  Wire.endTransmission();
  Wire.requestFrom( i2c_addr, (uint8_t) 1 ); // request only one byte
  while ( Wire.available() == 0 ) ; // waiting (blocking)
  uint8_t data = Wire.read();
  Wire.endTransmission();  
  return data;
}

void setup() {
  Serial.begin( 115200 );                  // set baudrate for Serial communication (Tx/Rx pins)

  WiFi.disconnect();                       // disconnect WiFi AP
  WiFi.mode( WIFI_OFF );                   // no use of WiFi, turn WiFi off (to save power consumption)
  
  Wire.begin( I2C_SDA_PIN, I2C_SCL_PIN );  // initialize I2C port (as I2C master)
  Wire.setClock( 100000 );                 // set clock speed for I2C
  i2c_scan();                              // scan I2C devices
  ts = millis();                           // read system timestamp in milliseconds
}

#ifdef USE_PATTERN_1
void update_output() {  
  static uint8_t data = 0x01; 
  i2c_write_byte( I2C_ADDR, ~data /* bitwise inverted */ ); // write data to PCF8574(A)
  if ( data & 0x80 ) {
    data = 0x01;
  } else {
    data = (data << 1);
  }
}
#endif

#ifdef USE_PATTERN_2
void update_output() {
  static uint8_t data = 0xFE; 
  static bool right_to_left = true;
  i2c_write_byte( I2C_ADDR, data ); // write data to PCF8574
  if ( right_to_left ) {
     data = (data << 1) | (data >> 7);
     if ( data == 0x7F ) right_to_left = false;
  } else { 
     data = (data >> 1) | (data << 7);
     if ( data == 0xFE ) right_to_left = true;
  }
}
#endif

void loop() {
  if ( millis() - ts >= DELAY_MSEC ) {
     uint8_t value = i2c_read_byte( I2C_ADDR ); // read value from PCF8574(A)
     sprintf( sbuf, "read: 0x%02X", (uint8_t)(~value) /* inverted */ );
     Serial.println( sbuf );
     update_output();
     ts += DELAY_MSEC; 
  }
}

///////////////////////////////////////////////////////////////////////////////////

