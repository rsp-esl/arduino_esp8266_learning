//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-01
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
// Note: use Vcc=3.3V for 16x2 LCD module, PCF8574 LCD adapter, and DHTxx
//////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>

#include <Wire.h> // use the Wire library
#include "LiquidCrystal_I2C.h" // -> https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
#include "DHT.h"               // -> https://github.com/adafruit/DHT-sensor-library

#define I2C_LCD_ADDR          (0x3F)    // set the I2C address for PCF8574(A): 0x27 or 0x3F
#define I2C_LCD_SCL_PIN       (4)       // D2 pin (SCL / GPIO-4)
#define I2C_LCD_SDA_PIN       (0)       // D3 pin (SDA / GPIO-0)

#define DHT_PIN               (2)       // D4 pin (GPIO-2)
#define DHT_TYPE              DHT22     // select the DHT sensor type: DHT11 or DHT22 tesed

DHT  dht( DHT_PIN, DHT_TYPE );
LiquidCrystal_I2C  lcd( I2C_LCD_ADDR, 16, 2 ); // 16x2 LCD display, set I2C address

// global variables
char sbuf[64];   // used for sprintf()
uint32_t ts;     // used to save timestamp value

#define INTERVAL_MSEC  (2000)

void i2c_scan() {
  int count = 0;
  Serial.println( "\n\nScanning I2C slave devices..." );
  for( uint8_t addr=0x01; addr <= 0x7f; addr++ ) {
     Wire.beginTransmission( addr ); 
     if ( Wire.endTransmission() == 0 ) {
       sprintf_P( sbuf, PSTR("I2C device found at 0x%02X."), addr );
       Serial.println( sbuf );
       count++;
    }
  }
  if ( count > 0 ) {
    sprintf_P( sbuf, PSTR("Found %d I2C device(s).\n\n"), count );
  } else {
    sprintf_P( sbuf, PSTR("No I2C device found.\n\n") );
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
  Serial.println( F("\n\n\n") );
  delay(100);

  WiFi.disconnect();
  WiFi.mode(WIFI_OFF); // disable WiFi

  dht.begin();

  Wire.begin( I2C_LCD_SDA_PIN, I2C_LCD_SCL_PIN );
  delay(1000);

  i2c_scan();
  delay(1000);
  
  lcd.begin( I2C_LCD_SDA_PIN, I2C_LCD_SCL_PIN, 400000 );
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0); // set cursor at top-level position on the first row
  lcd.print( F("ESP8266 DHTxx") );
  lcd.setCursor(0,1); // set cursor at the start position on the second row
  lcd.print( F("ESL/KMUTNB") );
  delay(4000);
  
  lcd.clear();
  lcd.setCursor(0,0); // set cursor at top-level position on the first row
  ts = millis();
}

void loop() {
  if ( millis() - ts >= INTERVAL_MSEC ) {
     char val_str[8];
     ts += INTERVAL_MSEC;

     // read relative humidity
     float humid = dht.readHumidity();
     // read temperature as Celsius (the default)
     float temp = dht.readTemperature();

     // check if any reads failed and exit early (to try again).
     if ( isnan(humid) || isnan(temp)  ) {
        Serial.println( F("Failed to read from DHT sensor!") );
        return;
     }

     dtostrf( humid, 3, 1, val_str );
     sprintf_P( sbuf, PSTR("Humid. %s \%RH"), val_str );
     lcd.setCursor(0 /*col*/, 0 /*row*/); 
     lcd.print( sbuf );
     Serial.println( sbuf );

     dtostrf( temp, 3, 1, val_str );
     sprintf_P( sbuf, PSTR("Temp.  %s C"), val_str );
     lcd.setCursor(0 /*col*/, 1 /*row*/); 
     lcd.print( sbuf );
     Serial.println( sbuf );
  }
  delay(1);
}
//////////////////////////////////////////////////////////////////////////

