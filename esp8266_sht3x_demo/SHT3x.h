////////////////////////////////////////////////////////////////////////////////
// Author: RSP
// IoT Engineering Education, KMUTNB
// Date: 2017-03-23
////////////////////////////////////////////////////////////////////////////////

#ifndef __SHT3X_H
#define __SHT3X_H

#include <Wire.h>

#define SHT3X_I2C_ADDR   (0x45)

class SHT3x {
  
  public:
    SHT3x( uint8_t sda, uint8_t scl, uint8_t addr = SHT3X_I2C_ADDR ) {
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

    void startMeasurement() {
       // Start I2C Transmission
        Wire.beginTransmission( _addr );
        // Send measurement command
        Wire.write( 0x2C );
        Wire.write( 0x06 );
        // Stop I2C transmission
        Wire.endTransmission();
    }

    int readMeasurement() {
        _data_valid = false;
        // Request 6 bytes of data
        Wire.requestFrom( (int) _addr, (int) 6 ); // request 6 bytes
        if ( Wire.available() == 6 ) {
            _data[0] = Wire.read(); // temperature Celsius (MSB)
            _data[1] = Wire.read(); // temperature Celsius (LSB)
            _data[2] = Wire.read(); // temperature Celsius CRC
            _data[3] = Wire.read(); // relative humidty (%RH) (MSB)
            _data[4] = Wire.read(); // relative humidty (%RH) (LSB)
            _data[5] = Wire.read(); // relative humidty CRC
        } else {
            Serial.println( "I2C read: 6 bytes expected!!! ");
            return -1;
        }
 
        // check CRC for both values
        if ((CRC8(_data[0],_data[1],_data[2])) && (CRC8(_data[3],_data[4],_data[5]))) {
            uint16_t value;
            value = (_data[0] << 8) + _data[1];
            _temp  = ((value * 175.0) / 65535) - 45; // temperature in Celsius.
            value = (_data[3] << 8) + _data[4];
            _humid = ((value * 100.0) / 65535); // relative humidity
            _data_valid = true;
            //Serial.println( "CRC OK!!!" );
            return 0;
        }
        else {
            Serial.println( "CRC failed!!!" );
            return -2;
        }
    }

    boolean isValid() { return _data_valid; }
    float getTemperature() { return _temp; }
    float getHumidity() { return _humid; }

  protected:
    bool CRC8(uint8_t MSB, uint8_t LSB, uint8_t CRC) {
        uint8_t crc = 0xFF;
        crc ^= MSB;
        for (uint8_t i=0; i < 8; i++) {
           crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
        }
        crc ^= LSB;
        for (uint8_t i=0; i < 8; i++) {
           crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
        }
        return (crc==CRC) ? true : false;
    }

  private:
     uint8_t _sda, _scl, _addr;
     uint8_t _data[6];
     float   _temp, _humid;
     boolean _data_valid;
};

#endif // __SHT3X_H
////////////////////////////////////////////////////////////////////////////////

