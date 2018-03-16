#define CLK   4   // GPIO-4 = D2
#define DIO   5   // GPIO-5 = D1

void tm1637_writeByte( uint8_t wr_data ) {
    uint8_t i, count1;
    for ( i=0; i<8; i++ ) {  // LSB first
        digitalWrite( CLK, LOW );
        digitalWrite( DIO, ( wr_data & 0x01) ? HIGH : LOW );
        wr_data >>= 1;
        digitalWrite( CLK, HIGH );
    }
 
    digitalWrite( CLK, LOW );   // wait for the ACK
    pinMode( DIO, INPUT );
    digitalWrite( DIO, HIGH );
    digitalWrite( CLK, HIGH );

    while( digitalRead( DIO ) == HIGH ) {
        count1 += 1;
        if (count1 == 200) { // timeout
            pinMode( DIO, OUTPUT );
            digitalWrite( DIO, LOW );
            count1 = 0;
            Serial.println("error\r\n");
        }
        pinMode( DIO, INPUT );
    }
    pinMode( DIO, OUTPUT );
}

// send start signal to TM1637
void tm1637_start(void) {
    // send start signal to TM1637
    digitalWrite( CLK, HIGH ); 
    digitalWrite( DIO, HIGH );
    digitalWrite( DIO, LOW );
    digitalWrite( CLK, LOW );
}

// End of transmission
void tm1637_stop(void) {
    // send start signal to TM1637
    digitalWrite( CLK, LOW ); 
    digitalWrite( DIO, LOW );
    digitalWrite( CLK, HIGH );
    digitalWrite( DIO, HIGH );
}


void tm1637_updateDisplay( uint8_t data[], bool dot=true ) {
  uint8_t i;
  uint8_t cmd;

  tm1637_start();
  cmd = 0x40;    // automatic address incrementing
  tm1637_writeByte( cmd );
  tm1637_stop();

  tm1637_start();
  cmd = 0xC0;   // set the display address to 0xC0
  tm1637_writeByte( cmd );
  for( i=0; i < 4; i++ ) {
      if (i==1) {
        if (dot) {
           data[i] |= 0x80; 
        } else {
           data[i] &= ~0x80; 
        }
      }
      tm1637_writeByte( data[i] );
  }
  tm1637_stop();

}

void tm1637_setBrightness( uint8_t level ) {
  uint8_t cmd_disp_ctrl = 0x88 + (level & 0b111);
  tm1637_start(); 
  tm1637_writeByte( cmd_disp_ctrl ); 
  tm1637_stop(); 
}

void tm1637_displayOff( ) {
  uint8_t cmd_disp_ctrl = 0x80;
  tm1637_start(); 
  tm1637_writeByte( cmd_disp_ctrl ); 
  tm1637_stop(); 
}

void tm1637_displayOn( uint8_t level=7) {
  tm1637_setBrightness( level );
}

const uint8_t DATA_7SEGMENTS[] = {
    0x3f,0x06,0x5b,0x4f,
    0x66,0x6d,0x7d,0x07,
    0x7f,0x6f,0x77,0x7c,
    0x39,0x5e,0x79,0x71
}; //0~9,A,b,C,d,E,F

uint8_t data[4];


void setup() {
  Serial.begin( 115200 );
  pinMode( CLK, OUTPUT );
  pinMode( DIO, OUTPUT );
  tm1637_setBrightness( 7 );

  data[0] = DATA_7SEGMENTS[1];
  data[1] = DATA_7SEGMENTS[2] | 0x80;
  data[2] = DATA_7SEGMENTS[4];
  data[3] = DATA_7SEGMENTS[9];
  tm1637_updateDisplay( data, true );
  delay(500);
  tm1637_displayOff();
  delay(500);
  tm1637_displayOn(7);
}

void loop() {
  tm1637_updateDisplay( data, true );
  delay(500);
  tm1637_updateDisplay( data, false );
  delay(500);
}


