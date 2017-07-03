//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-03
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
// Note: use Vcc=3.3V for PCF8574-based 8-bit I/O expander module
//      + 4x4 membrane keypad
//////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>    // Use the ESP8266WiFi library
#include <Wire.h>           // Use the Wire library

// Use VCC=3.3V for PCF8574(A)
// PCF8574A and 4x4 Keypad (with 10K pull-up for each column line)
//   P7..P4 as inputs,  connected to rows (R3..R0) of the 4x4 keypad 
//   P3..P0 as outputs, connected to columns (C3..C0) of the 4x4 keypad
// see: http://www.ti.com/lit/ds/symlink/pcf8574.pdf
//  Vcc 2.5V to 6V, f_SCL 100kHz max., Sink Current (I_OL) 25mA max.

#define I2C_SCL_PIN      (4)      // D2 pin (SCL / GPIO-4)
#define I2C_SDA_PIN      (0)      // D3 pin (SDA / GPIO-0)
#define I2C_ADDR        0x38      // <-- set the I2C address of the PCF8574(A) chip 

#define DEBUG

// constants
const char keys[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};

// global variables
char sbuf[64];   // used for sprintf()

void i2c_scan() {
  int count = 0;
  Serial.println( "Scanning I2C slave devices..." );
  for( uint8_t addr=0x01; addr <= 0x7f; addr++ ) {
     Wire.beginTransmission( addr ); 
     if ( Wire.endTransmission() == 0 ) {
       sprintf( sbuf, "I2C device found at 0x%02X.", addr );
       Serial.println( sbuf );
       count++;
    }
  }
  if ( count > 0 ) {
    sprintf( sbuf, "Found %d I2C device(s).", count );
  } else {
    sprintf( sbuf, "No I2C device found." );
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
  while ( Wire.available() == 0 ) ; // waiting 
  uint8_t data = Wire.read();
  Wire.endTransmission();  
  return data;
}

// PCF8574: P0..P3 output, P4..P7 input (with 4x 10k pull-up resistors)
char getKey() { // blocking function call
  byte value;
  char key = '\0';
  for ( uint8_t row=0; row < 4; row++ ) { 
     value = (~(1 << row)) << 4 | (0x0F); 
     i2c_write_byte( I2C_ADDR, value ); // send one data byte to PCF8754A
     value = i2c_read_byte( I2C_ADDR ); // read one data byte from PCF8574A
     for ( uint8_t col=0; col < 4; col++ ) {
        if ( (~value >> col) & 1 ) {
          key = keys[3-row][3-col];
#ifdef DEBUG
          sprintf( sbuf, "R%dC%d", row, col );
          Serial.println( sbuf );
#endif
          break;
        }
     }
     if ( key != '\0' ) {
        while ( value == i2c_read_byte( I2C_ADDR ) ) ; // blocking
        return key; 
     } 
  }
  return key; // no key press
}

void setup() {
  Serial.begin( 115200 );
  Serial.println( "\n\n\n" );
  delay(100);
  
  WiFi.disconnect();
  WiFi.mode( WIFI_OFF ); // disable WiFi

  Wire.begin( I2C_SDA_PIN, I2C_SCL_PIN ); 
  Wire.setClock( 100000 ); // set clock speed
  i2c_scan(); // scan I2C devices
}

void loop() {
  char key = getKey();
  if ( key != '\0' ) {
    sprintf( sbuf, "key pressed: %c", key ); 
    Serial.println( sbuf );
  }
  delay(50);
}
//////////////////////////////////////////////////////////////////////////

