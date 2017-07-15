//////////////////////////////////////////////////////////////////////////
// Author: RSP @ Embedded Systems Lab (ESL), KMUTNB, Bangkok / Thailand
// Date: 2017-07-17
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Boards with ESP-12E
//////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include <time.h>

// You need to connect the ESP8266 to the Internet via a Wireless Access Point.
const char WIFI_SSID[] = "<YOUR_WIFI_SSID>";     // <======
const char WIFI_PASS[] = "<YOUR_WIFI_PASSWORD>"; // <======

const char *WEEK_DAY_NAMES[] = {
   "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" 
};
const char *MONTH_NAMES[] = {
   "Jan", "Feb", "Mar", "Apr", "May", "Jun", 
   "Jul", "Aug", "Sep", "Oct", "Nov", "Dem" };

uint32_t ts;

void connectWiFi() {
  WiFi.mode( WIFI_STA );
  WiFi.begin(  WIFI_SSID, WIFI_PASS );
  Serial.print( "Connecting WiFi " );
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }
  Serial.println( "" );  
  Serial.println( "WiFi connected" );
  Serial.print( "IP address: " );
  Serial.println( WiFi.localIP() );
}

void showDateTime( ) {
  time_t now = time(NULL);
  while( !now ) {
     time( &now );
     delay(100);
  } 
  
  struct tm *tm_ptr = localtime(&now); 
  //Serial.print( asctime( tm_ptr ) );  
  //Serial.print( ctime(&now) ); 
  
  int wday   = tm_ptr->tm_wday;       // 0=Sun, ..., 6=Sat
  int mday   = tm_ptr->tm_mday;       // 1..31
  int mon    = tm_ptr->tm_mon;        // 0..11
  int year   = tm_ptr->tm_year+1900;
  int hour   = tm_ptr->tm_hour;
  int minute = tm_ptr->tm_min;
  int second = tm_ptr->tm_sec;

  String str;
  char sbuf[64];
  sprintf( sbuf, "%s %s %d, %4d ", WEEK_DAY_NAMES[ wday ], MONTH_NAMES[ mon], mday, year );
  str += sbuf;
  sprintf( sbuf, "%02d:%02d:%02d ", hour, minute, second );
  str += sbuf;
  Serial.println( str );
}

void setup() {
  Serial.begin( 115200 );
  Serial.println( "\n\n\n" );

  connectWiFi();  
  configTime( 7*3600 /* timezone offset in seconds*/, 0 , "th.pool.ntp.org", "time.nist.gov" ); 
  showDateTime();
  ts = millis();
}

void loop() {
  if ( millis() - ts >= 1000 ) {
     ts = millis();
     showDateTime();
  }
  delay(5);
}
//////////////////////////////////////////////////////////////////////////


