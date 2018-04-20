////////////////////////////////////////////////////////////////////////////////
// Author: RSP
// IoT Engineering Education, KMUTNB
// Date: 2017-04-20
////////////////////////////////////////////////////////////////////////////////

#ifndef __SI7021_H
#define __SI7021_H

#include <Wire.h>

#define SI7021_MEASTEMP_NOHOLD_CMD     (0xF3)
#define SI7021_MEASRH_NOHOLD_CMD       (0xF5)
#define SI7021_RESET_CMD               (0xFE)
#define SI7021_READ_USER_REG           (0xE7)

#define SI7021_I2C_ADDR                (0x40)

class SI7021 {
  
  public:
    SI7021( uint8_t sda, uint8_t scl, uint8_t addr = SI7021_I2C_ADDR ) {
        _addr = addr;
        _sda  = sda;
        _scl  = scl;
        _temp  = 0.0f;
        _humid = 0.0f;
        _data_valid = false;
    }

    void begin() {
        Wire.begin( _sda, _scl );
        Wire.setClock( 400000 );
    }

    uint16_t readRaw( uint8_t cmd ) {
        uint16_t value = 0;

        _data_valid = false;
        // Start I2C Transmission
        Wire.beginTransmission( _addr );
        Wire.write( cmd );
        // Stop I2C transmission
        Wire.endTransmission();
        delay(25);

        // Request 3 bytes of data
        Wire.requestFrom( (int) _addr, (int) 3 ); // request 3 bytes

        if ( Wire.available() == 3 ) {
            _data[0] = Wire.read(); // (MSB)
            _data[1] = Wire.read(); // (LSB)
            _data[2] = Wire.read(); // (CRC8)
            Wire.endTransmission();
            if ( CRC8(_data, 2) == _data[2] ) {
               _data_valid = true;
               value = _data[0];
               value = (value << 8) | _data[1];
            }
        } 
        return value;
    }

    bool readMeasurement( ) {
        uint16_t value;

        value = readRaw( SI7021_MEASTEMP_NOHOLD_CMD );
        if ( value != 0 && _data_valid ) {
           _temp = value;
           _temp *= 175.72f;
           _temp /= 65536;
           _temp -= 46.85f;
        } else {
            return false;
        }

        value = readRaw( SI7021_MEASRH_NOHOLD_CMD );
        if ( value != 0 && _data_valid ) {
           _humid  = value;
           _humid *= 125;
           _humid /= 65536;
           _humid -= 6;
        } else {
           return false;
        }
        return true;
    }

    boolean isValid() { return _data_valid; }
    float getTemperature() { return _temp; }
    float getHumidity() { return _humid; }

  protected:

    uint8_t CRC8( const uint8_t *data, uint8_t len ) {
       uint8_t crc = 0x00;
       int i, j;
       for ( i=0; i < len; ++i ) {
            crc ^= data[i];
            for ( j=8; j > 0; --j) {
                if ( crc & 0x80 ) {
                    crc = (crc << 1) ^ 0x131;
                } else {
                   crc = (crc << 1);
               }
            }
       }
       return crc;
   }

  private:
     uint8_t _sda, _scl, _addr;
     uint8_t _data[3];
     float _temp, _humid;
     boolean _data_valid;
};

#endif 
////////////////////////////////////////////////////////////////////////////////

