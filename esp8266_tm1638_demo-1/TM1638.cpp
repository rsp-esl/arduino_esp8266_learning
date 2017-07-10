//////////////////////////////////////////////////////////////////////////
#include "TM1638.h"

const uint8_t DIGIT_FONTS[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111, // 9
  0b01110111, // A
  0b01111100, // B
  0b00111001, // C
  0b01011110, // D
  0b01111001, // E
  0b01110001, // F
  0b00000000  // OFF
};


TM1638::TM1638( uint8_t dataPin, uint8_t clkPin, uint8_t strbPin ):
  data_pin(dataPin), clk_pin(clkPin), strb_pin(strbPin)
{
  //...
}

void TM1638::setBrightness( uint8_t value, boolean onoff ) {
  sendCmd( TM1638_DSP_CTRL_CMD 
         | ((onoff) ? TM1638_DSP_ON : TM1638_DSP_OFF) 
         | (value & TM1638_BRT_MSK) );

}
void TM1638::init() {
  pinMode( clk_pin, OUTPUT );
  pinMode( data_pin, OUTPUT );
  pinMode( strb_pin, OUTPUT );
  digitalWrite( strb_pin, HIGH );
  digitalWrite( clk_pin, HIGH );
  
  sendCmd( TM1638_DATA_SET_CMD | TM1638_DATA_WR 
         | TM1638_ADDR_INC | TM1638_MODE_NORM ); 

  setBrightness( TM1638_BRT7, true );

  digitalWrite( strb_pin, LOW );
  send( TM1638_ADDR_SET_CMD ); // 0xC0 set address to 00h
  for ( int i=0; i < 16; i++ ) { // up to 16 addresses
     send(0x00); // turn all 7-segments and LEDs off
  }
  digitalWrite( strb_pin, HIGH );
}

void TM1638::set7segment( uint8_t value, uint8_t pos ) {
  digitalWrite( strb_pin, LOW );
  send( TM1638_ADDR_SET_CMD | ((pos << 1) & TM1638_ADDR_MSK) ); 
  send( value ); // 7-segments + DP (1=on/0=off)
  digitalWrite( strb_pin, HIGH );  
}

void TM1638::setDigit( uint8_t digit, uint8_t pos, const uint8_t digits_font[] ) {
  set7segment( digits_font[digit], pos );
}

void TM1638::setDigits( uint8_t digits[], uint8_t len, uint8_t start, const uint8_t digits_font[] ) {
  if (len > 8) { len = 8; }
  for ( uint8_t i=0; i < len; i++ ) {
    if ( (i+start) < 8 ) {
      setDigit( digits[i], i+start, digits_font );
    } 
  }
}
    
void TM1638::setNumber( uint32_t value, uint8_t len ) {
  if (len > 8) { len = 8; }
  for ( uint8_t i=0; i < 8; i++ ) {
     if ( i < len ) {
       setDigit( value % 10, 7-i );
       value /= 10;
     } else {
       set7segment( 0x00, 7-i );
     }
  }
}

void TM1638::setLED( int value, uint8_t pos ) {
  digitalWrite( strb_pin, LOW );
  send( TM1638_ADDR_SET_CMD | (((pos << 1)+1) & TM1638_ADDR_MSK) ); 
  send( value ? 1 : 0 );  // LED (1=on/0=off)
  digitalWrite( strb_pin, HIGH );  
}

void TM1638::setLEDs( uint8_t leds_value ) {
  for ( int i=0; i < 8; i++ ) {
    setLED( (leds_value >> (7-i)) & 1, i );
  }
}

uint8_t TM1638::readButtons( ) {
  uint8_t value;
  digitalWrite( strb_pin, LOW );
  send( TM1638_DATA_SET_CMD | TM1638_KEY_RD );
  value = recv();
  digitalWrite( strb_pin, HIGH );
  return value;
}

uint8_t TM1638::recv() {
  uint8_t value = 0, temp;
  
#ifdef ESP8266
  pinMode( data_pin, INPUT_PULLUP );
#else
  pinMode( data_pin, INPUT );
  digitalWrite( data_pin, HIGH );
#endif

  for ( int j=0; j < 8; j++ ) {
    temp = 0;    
    for ( int i=0; i < 4; i++ ) {
      temp >>= 1;
      digitalWrite( clk_pin, LOW );
      if ( digitalRead(data_pin) ) {
        temp |= 0x8;
      }
      digitalWrite( clk_pin, HIGH );
    }
    if ( temp ) {
      if (j%2 == 0) {
        value |= (1 << j/2);
      } else {
        value |= (1 << (j/2+4));
      }
    }
  }
  pinMode( data_pin, OUTPUT );
  digitalWrite( data_pin, LOW );
  return value;
}

void TM1638::send( uint8_t data ) {
  for ( int i=0; i < 8; i++ ) {
    digitalWrite( clk_pin, LOW );
    digitalWrite( data_pin, (data & 1) ? HIGH : LOW );
    data >>= 1;
    digitalWrite( clk_pin, HIGH );
  }
}

void TM1638::sendCmd( uint8_t cmd ) {
  digitalWrite( strb_pin, LOW );
  send(cmd);
  digitalWrite( strb_pin, HIGH );
}

void TM1638::sendData( uint8_t addr, uint8_t  data) {
  sendCmd( TM1638_DATA_SET_CMD | TM1638_ADDR_FIXED );
  digitalWrite( strb_pin, LOW );
  send( TM1638_ADDR_SET_CMD | addr );
  send( data );
  digitalWrite( strb_pin, HIGH );
}

//////////////////////////////////////////////////////////////////////////




