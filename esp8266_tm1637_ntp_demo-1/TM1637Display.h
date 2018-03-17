////////////////////////////////////////////////////////////////////////////////
// Author: RSP
// IoT Engineering Education, KMUTNB
// Date: 2017-03-17
////////////////////////////////////////////////////////////////////////////////

#ifndef __TM1637_DISPLAY_H
#define __TM1637_DISPLAY_H

const uint8_t TABLE_7SEGMENTS[] = { // binary to 7-segment 
    0x3f,0x06,0x5b,0x4f,
    0x66,0x6d,0x7d,0x07,
    0x7f,0x6f,0x77,0x7c,
    0x39,0x5e,0x79,0x71
}; //0~9,A,b,C,d,E,F

class TM1637Display {
  
  public:
     TM1637Display() { 
        _clk = -1;
        _dio = -1;
     }

     void begin( int8_t clk, int8_t dio ) {
        _clk = clk;
        _dio = dio;
        pinMode( _clk, OUTPUT );
        pinMode( _dio, OUTPUT );
        setBrightness( 7 );  
     }
     
     void setBrightness( uint8_t level ) {
        uint8_t cmd_disp_ctrl = 0x88 + (level & 0b111);
        start(); 
        write_byte( cmd_disp_ctrl ); 
        stop(); 
     }

     void turnOff( ) {
        uint8_t cmd_disp_ctrl = 0x80;
        start(); 
        write_byte( cmd_disp_ctrl ); 
        stop(); 
     }  

     void turnOn( uint8_t level=7 ) {
        setBrightness( level );
     }

     void update( uint8_t data[], bool colon=true ) {
        uint8_t cmd;

        start();
        cmd = 0x40;    // automatic address incrementing
        write_byte( cmd );
        stop(); 

        start();
        cmd = 0xC0;    // set the display address to 0xC0
        write_byte( cmd );
        
        for( uint8_t i=0; i < 4; i++ ) { // write 4-byte data, byte-by-byte
            if (i==1) {
               if (colon) {  // show the colon LEDs ?
                  data[i] |=  0x80;  // colon on
               } else {
                  data[i] &= ~0x80;  // colon off
               }
            }
            write_byte( data[i] );
         } //end-for
         stop();
     }

     void showDigits( uint8_t digits[], bool colon=true ) {
         uint8_t data[4];
         for ( uint8_t i=0; i < 4; i++ ) { 
            data[i] = TABLE_7SEGMENTS[ digits[i] ];
         }
         update( data, colon );
     }

  protected:

     void start(void) { // send start signal to TM1637
        digitalWrite( _clk, HIGH ); 
        digitalWrite( _dio, HIGH );
        digitalWrite( _dio, LOW );
        digitalWrite( _clk, LOW );
     }

     void stop(void) { // send end signal to TM1637
        digitalWrite( _clk, LOW ); 
        digitalWrite( _dio, LOW );
        digitalWrite( _clk, HIGH );
        digitalWrite( _dio, HIGH );
     }

     bool write_byte( uint8_t wr_data ) {
        bool ok = true;
        for ( uint8_t i=0; i<8; i++ ) {  // LSB first
           digitalWrite( _clk, LOW );
           digitalWrite( _dio, ( wr_data & 0x01) ? HIGH : LOW );
           wr_data >>= 1;
           digitalWrite( _clk, HIGH );
        }
        digitalWrite( _clk, LOW ); 
        pinMode( _dio, INPUT );
        digitalWrite( _dio, HIGH );
        digitalWrite( _clk, HIGH );
        
        // wait for the ACK
        uint8_t cnt = 0;
        while( digitalRead( _dio ) == HIGH ) {
           cnt++;
           if ( cnt == 100 ) { // timeout
               pinMode( _dio, OUTPUT );
               digitalWrite( _dio, LOW );
               cnt = 0;
               ok = false;
            }
            pinMode( _dio, INPUT );
            delayMicroseconds(20);
        }  // end-of-while
        
        pinMode( _dio, OUTPUT );
        
        if (!ok) { 
           Serial.println( "TM1637 write byte error" );
        }
        return ok;
     }

  private:
     int8_t _clk, _dio;
};

#endif
////////////////////////////////////////////////////////////////////////////////

