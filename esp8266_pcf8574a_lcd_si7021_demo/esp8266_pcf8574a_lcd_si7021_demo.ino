/////////////////////////////////////////////////////////////////////////////////////////////
// Author: RSP
// IoT Engineering Education, KMUTNB
// Date: 2017-04-20
/////////////////////////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>

#include <Wire.h> // use the Wire library
#include "LiquidCrystal_I2C.h" // https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
#include "SI7021.h" 

#define I2C_SCL_PIN       (0)       //  GPIO-0 (D4)
#define I2C_SDA_PIN       (2)       //  GPIO-2 (D4)
#define I2C_LCD_ADDR          (0x3F)    // use 0x27 or 0x3F
#define I2C_BH1750_ADDR       (0x23)

//-------------------------------------------------------------------
LiquidCrystal_I2C lcd( I2C_LCD_ADDR, 16, 2 ); // 16x2 LCD display, set I2C address
SI7021  si7021( I2C_SDA_PIN, I2C_SCL_PIN, 0x40 );

char sbuf[64];
uint32_t ts;

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

int32_t temp, humid;

//-------------------------------------------------------------------
void setup() {
  Serial.begin( 115200 );
  Serial.println( "\n\n\n" );

  WiFi.disconnect();
  WiFi.mode( WIFI_OFF );

  Wire.begin( I2C_SDA_PIN, I2C_SCL_PIN );
  delay(100);
  i2c_scan();
  delay(100);

  lcd.begin( I2C_SDA_PIN, I2C_SCL_PIN, 400000 );
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0); // set cursor at top-level position on the first row
  lcd.print( "IoT Engineering" );
  lcd.setCursor(0,1); // set cursor at the start position on the second row
  lcd.print( "Education" );
  delay(4000);
  lcd.clear();
  lcd.setCursor(0,0); // set cursor at top-level position on the first row  
  lcd.print( "Si7021 Reading..." );
  ts = millis();
}

void loop() {
  if ( millis() - ts >= INTERVAL_MSEC ) {
     ts = millis();
     
     if ( si7021.readMeasurement() ) {
         temp = (int32_t)(10*si7021.getTemperature());
         humid = (int32_t)(10*si7021.getHumidity());
         sprintf( sbuf, "Temp : %d.%d degC, ", temp/10, temp%10 );
         Serial.print( sbuf );
         lcd.setCursor( 0 /*col*/, 0 /*row*/ ); 
         lcd.print( sbuf );
         sprintf( sbuf, "Humid: %d.%d %%RH", humid/10, humid%10 );
         Serial.println( sbuf );
         lcd.setCursor( 0 /*col*/, 1 /*row*/ ); 
         lcd.print( sbuf );
     } else {
         Serial.println( "Sensor reading failed!!!\r\n" );
         lcd.setCursor( 0 /*col*/, 0 /*row*/ ); 
         lcd.print( "Sensor Reading" );
         lcd.setCursor( 0 /*col*/, 1 /*row*/ ); 
         lcd.print( "Status: FAILED " );
     }
  }
  delay(1);
}
/////////////////////////////////////////////////////////////////////////////////////////////

