//////////////////////////////////////////////////////////////////////////
#ifndef __TM1638_H__
#define __TM1638_H__

#include "Arduino.h"

#define TM1638_DATA_SET_CMD 0x40  // data instruction set 0x01xx_xxxx
#define TM1638_DATA_WR      0x00  // write data to register
#define TM1638_KEY_RD       0x02  // read key-scan data
#define TM1638_ADDR_INC     0x00  // auto address increment
#define TM1638_ADDR_FIXED   0x04  // fixed address
#define TM1638_MODE_NORM    0x00  // normal mode
#define TM1638_MODE_TEST    0x08  // test mode

#define TM1638_DSP_CTRL_CMD 0x80 // display instruction set 0x10xx_xxxx
#define TM1638_BRT_MSK      0x07
#define TM1638_BRT0         0x00 //Pulsewidth 1/16
#define TM1638_BRT1         0x01 //Pulsewidth 2/16
#define TM1638_BRT2         0x02 //Pulsewidth 4/16
#define TM1638_BRT3         0x03 //Pulsewidth 10/16
#define TM1638_BRT4         0x04 //Pulsewidth 11/16
#define TM1638_BRT5         0x05 //Pulsewidth 12/16
#define TM1638_BRT6         0x06 //Pulsewidth 13/16
#define TM1638_BRT7         0x07 //Pulsewidth 14/16
#define TM1638_DSP_OFF      0x00 //Display OFF
#define TM1638_DSP_ON       0x08 //Display ON

#define TM1638_ADDR_SET_CMD 0xC0 // address instruction set 0x11xx_xxxx
#define TM1638_ADDR_MSK     0x0F

extern const uint8_t DIGIT_FONTS[];

class TM1638 {
  public:
    TM1638( uint8_t dataPin, uint8_t clkPin, uint8_t strbPin );
    void init();
    void setBrightness( uint8_t value, boolean onoff ); 
    void setLED( int value, uint8_t pos );
    void setLEDs( uint8_t leds_value );
    void set7segment( uint8_t value, uint8_t pos );
    void setDigit( uint8_t digit, uint8_t pos, const uint8_t digits_font[]=DIGIT_FONTS );
    void setDigits( uint8_t digits[], uint8_t len=8, uint8_t start=0, const uint8_t digits_font[]=DIGIT_FONTS );
    void setNumber( uint32_t value, uint8_t len=8 );
    uint8_t readButtons( );
    
    uint8_t recv();
    void send( uint8_t data );
    void sendCmd( uint8_t cmd );
    void sendData( uint8_t addr, uint8_t  data);

  protected:
    uint8_t data_pin, clk_pin, strb_pin;
    
};

#endif // __TM1638_H__
//////////////////////////////////////////////////////////////////////////


