///////////////////////////////////////////////////////////////////////////////////////////
// Author: Rawat S.
//    Department of Electrical & Computer Engineering, 
//    Faculty of Engineering, KMUTNB, Bangkok/Thailand
// Date: 2017-12-06
///////////////////////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>

#define BAUDRATE (115200)

const char* WIFI_SSID = "XXXXXXXXXXXXX"; // specify the SSID of the target AP
const char* WIFI_PASS = "XXXXXXXXXXXXX"; // specify the password for that SSID

IPAddress server_ip(192, 168, 1, 9); // specify the IP address of the remote machine (UDP server)
int server_port = 8888;   // UDP port number of the remote machine
int local_port  = 22222;  // local UDP port

WiFiUDP udp;

void setup() {
  Serial.begin( BAUDRATE );
  Serial.println( F("\n\n\n\n") );
  Serial.flush();
   
  WiFi.begin( WIFI_SSID, WIFI_PASS );
  WiFi.config( 
     IPAddress(192, 168, 1, 20),  // we use a static IP address 
     IPAddress(192, 168, 1, 1), 
     IPAddress(255, 255, 255, 0) );
  
  Serial.println( F("Connecting to WiFi...") );
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }

  Serial.println( F("\n\nWiFi connected") );  
  Serial.print( F("IP address: ") );
  Serial.println( WiFi.localIP() );
  
  udp.begin( local_port );
}

#define MAX_LEN (255)
char data[ MAX_LEN+1 ];
int count = 1;
uint32_t ts;
uint32_t t_min = 1000000, t_max = 0, t_sum = 0; 

void loop() {
  ts = micros();
  sprintf( data, "%d.%03d: This is a text message number #%04d sent from ESP8266 over 2.4GHz WiFi...\n",
           (ts/1000000), (ts%1000000)/1000, count );
           
  udp.beginPacket( server_ip, server_port );
  udp.write( (const uint8_t *)data, strlen(data) );
  udp.endPacket();
  ts = micros() - ts;
  
  Serial.printf( "packet sent: #%05d, proc time: %d usec\n", count, ts );

  // update statistics about execution times
  t_sum += ts;
  if ( ts < t_min ) { t_min = ts; }
  if ( ts > t_max ) { t_max = ts; }
  if ( count % 100 == 0 ) {
     Serial.println( F("===============================================") );
     Serial.printf( "proc time (min/max/avg) = (%d,%d,%d.%d) usec\n\n",
                    t_min, t_max, t_sum/100, (10*(t_sum%100))/10 );
     t_min = 1000000;
     t_max = 0;
     t_sum = 0;
     delay(10000);
  }
  count = (count+1) % 10000;

  int packetSize = udp.parsePacket();
  if( packetSize > 0 ) { // check whether we have an incoming message from the server
    udp.read( data, MAX_LEN );
    String str(data);
    str.trim();
    const char *p = str.c_str();
    Serial.printf( "recv: '%s' (%d bytes)\n", p, strlen(p) );
  }
  
  delay(100);
}

///////////////////////////////////////////////////////////////////////////////////////////

