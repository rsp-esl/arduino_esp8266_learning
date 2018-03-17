////////////////////////////////////////////////////////////////////////////////
// Author: RSP
// IoT Engineering Education, KMUTNB
// Date: 2017-03-17
////////////////////////////////////////////////////////////////////////////////

#ifndef __NTP_DATETIME_H
#define __NTP_DATETIME_H

#include <ESP8266WiFi.h>
#include <time.h>

class NTPDateTime {
  
  public:
     NTPDateTime() {
        _ts = time(NULL);
     }

     void connect() {
        configTime( 
            7*3600,                /* timezone offset in seconds*/
            0,                     /* daylightOffset */
            "th.pool.ntp.org",     /* NTP server 1 */
            "time.nist.gov",       /* NTP server 2 */
            "time.microsoft.com"   /* NTP server 3 */ );
     }

     bool update( uint8_t num_retries=10 ) {
        uint8_t cnt = 0;
        _ts = time(NULL);
        while ( !_ts ) {
            time( &_ts );
            cnt++;
            if ( cnt >= num_retries ) { // too many retries
               return false;
            }
            delay(100);
        }
        return true;
     }

     bool getDate( uint16_t &year, uint16_t &month, uint16_t &mday, uint16_t &wday ) {
        if ( !update() ) {
            return false;
        }
        struct tm *tm_now = localtime( &_ts ); 
        wday  = tm_now->tm_wday;         // 0=Sun, ..., 6=Sat
        mday  = tm_now->tm_mday;         // 1..31
        month = tm_now->tm_mon;          // 0..11
        year  = tm_now->tm_year + 1900; 
        return true;      
     }
     
     bool getTime( uint16_t &hours, uint16_t &minutes, uint16_t &seconds ) {
        if ( !update() ) {
            return false;
        }
        struct tm *tm_now = localtime( &_ts ); 
        hours   = tm_now->tm_hour;
        minutes = tm_now->tm_min;
        seconds = tm_now->tm_sec;
        return true;      
     }
     
     bool getDateTime( struct tm *tm_ptr ) {
        if ( tm_ptr == NULL || !update() ) {
            return false;
        }
        struct tm *tm_now = localtime( &_ts ); 
        tm_ptr->tm_wday  = tm_now->tm_wday;       // 0=Sun, ..., 6=Sat
        tm_ptr->tm_mday  = tm_now->tm_mday;       // 1..31
        tm_ptr->tm_mon   = tm_now->tm_mon;        // 0..11
        tm_ptr->tm_year  = tm_now->tm_year;       // +1900
        tm_ptr->tm_hour  = tm_now->tm_hour;
        tm_ptr->tm_min   = tm_now->tm_min;
        tm_ptr->tm_sec   = tm_now->tm_sec;
        return true;
     }

     bool getDateTimeString( String &str ) {
        struct tm tm_now;
        if ( getDateTime( &tm_now ) ) {
           str = asctime( &tm_now );  
           return true;
        } 
        return false;
     }
     
  private:
     time_t _ts;  // timestamps in seconds
};

#endif
////////////////////////////////////////////////////////////////////////////////

