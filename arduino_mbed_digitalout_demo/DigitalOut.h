///////////////////////////////////////////////////////////////////////
#ifndef __DIGITAL_OUT_H
#define __DIGITAL_OUT_H
///////////////////////////////////////////////////////////////////////

#include "Arduino.h"

class DigitalOut {
public:
  DigitalOut( int pin, int value=0 ) : _pin(pin), _value(value) {
     pinMode( _pin, OUTPUT );
     digitalWrite( _pin, _value );
  }
  
  inline void write( int value ) {
     digitalWrite( _pin, _value = value );
  }
  
  inline int read() { 
     return _value; 
  }
  
  inline operator int() { 
     return this->read(); 
  }
  
  inline DigitalOut& operator=( DigitalOut& rhs ) {
     this->write( rhs.read() );
     return *this;
  }
  
  inline DigitalOut& operator=( int value ) {
     this->write( value );
     return *this;
  }

private:
  int _pin, _value;

};

#endif // __DIGITAL_OUT_H
///////////////////////////////////////////////////////////////////////


