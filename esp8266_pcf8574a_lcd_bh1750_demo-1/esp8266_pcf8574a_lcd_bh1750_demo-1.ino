//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-01
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
// Note: the 16x2 LCD module + PCF8574(A) adapter use Vcc=3.3V.
//////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>

#include <Wire.h> // use the Wire library
#include "LiquidCrystal_I2C.h" // -> https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
#include "BH1750.h"            // -> https://github.com/claws/BH1750

#define I2C_SCL_PIN       (4)       // D2 pin (SCL / GPIO-4)
#define I2C_SDA_PIN       (0)       // D3 pin (SDA / GPIO-0)

#define I2C_LCD_ADDR      (0x3F)    // set the I2C address for PCF8574(A) chip (0x27 or 0x3F)
#define I2C_BH1750_ADDR   (0x23)    // set the I2C address for BH1750 (default)

LiquidCrystal_I2C lcd( I2C_LCD_ADDR, 16, 2 ); // 16x2 LCD display, set I2C address

BH1750 bh( I2C_BH1750_ADDR );

// global variables
char sbuf[64];  // used for sprintf()
uint32_t ts;    // used to keep timestamp value

#define INTERVAL_MSEC  (2000)

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

//-------------------------------------------------------------------
void setup() {
  Serial.begin( 115200 );
  Serial.println( "\n\n\n" );

  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);

  Wire.begin( I2C_SDA_PIN, I2C_SCL_PIN );
  delay(1000);
  i2c_scan();
  delay(1000);

  // initialize the BH1750 sensor
  bh.begin( BH1750_CONTINUOUS_HIGH_RES_MODE, I2C_SDA_PIN, I2C_SCL_PIN, 400000 );
  
  // initialize the LCD module
  lcd.begin( I2C_SDA_PIN, I2C_SCL_PIN, 400000 );
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0); // set cursor at top-level position on the first row
  lcd.print( F("ESP8266 BH1750") );
  lcd.setCursor(0,1); // set cursor at the start position on the second row
  lcd.print( F("ESL/KMUTNB") );
  delay(4000);
  lcd.clear();
  lcd.setCursor(0,0); // set cursor at top-level position on the first row  
  lcd.print( F("Light Intensity") );
  ts = millis();
}

void loop() {
  if ( millis() - ts >= INTERVAL_MSEC ) {
     char val_str[8];
     ts += INTERVAL_MSEC;

     uint16_t lux = bh.readLightLevel(); 
     dtostrf( lux, 6, 1, val_str );

     sprintf( sbuf, "    %s Lux", val_str );
     lcd.setCursor(0 /*col*/, 1 /*row*/); 
     lcd.print( sbuf );
     Serial.println( sbuf );
  }
  delay(1);
}
//////////////////////////////////////////////////////////////////////////

