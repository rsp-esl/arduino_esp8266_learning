/////////////////////////////////////////////////////////////////////////////////
// Embedded Systems Lab (ESL) @ KMUTNB
// Date: 2017-05-30
// Target board: WeMos D1 Pro
// Ethernet module: Funduino W5100 Ethernet module
/////////////////////////////////////////////////////////////////////////////////

#include <SPI.h>
#include <Ethernet.h>

#include <EthernetClient.h>
#include "PubSubClient.h"

/* 2x5 pin header of the Funduino W5100 Ethernet module
- V+    5V pin (there is a 3.3V LDO regulator onboard)
- GND   Ground pin
- NSS   Chip Select or Slave Select (active-low) pin
- RST   Reset pin (Active-low), with 10k pull-up resistor onboard
- MO    MOSI
- SOK   SCK
- MI    MISO
- POE-  not used
- POE+: not used
*/

// Note:
//  SPI CLK  = GPIO14 / D5
//  SPI MOSI = GPIO13 / D7
//  SPI MISO = GPIO12 / D6
//  SPI #CS  = GPIO15 / D8

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:

#define BT_PIN  4 //  GPIO-4 / D2 pin

uint8_t client_mac[] = { 0xDE, 0xAD, 0xBE, 0x00, 0x00, 0x99 };
IPAddress mqtt_server( 192, 168, 1, 9 );  // set the IP address of the MQTT server

#define MAX_COUNT       200         // the number of messages to be sent
#define MQTT_PORT       1883        // the default port number of the MQTT broker
#define MQTT_CLIENT_ID  "arduinoClient"
#define MQTT_TOPIC      "esl/arduino/test"

void callback( char* topic, byte* payload, unsigned int len );

EthernetClient ethClient;
PubSubClient client(ethClient);

enum states  { ST_NONE=0, ST_READY, ST_RUNNING, ST_FINISHED };
typedef enum states state_t;
state_t state = ST_READY;

boolean running = false;
boolean wait_for_response = false;
uint32_t ts0, ts1, ts_saved;
uint16_t cnt = 0;
char sbuf[100];

void setup() {
  delay( 100 );
  Serial.begin( 115200 );
  Serial.println( "\n\n\n\n" );
  delay( 100 );
  pinMode( BT_PIN, INPUT_PULLUP );
 
  client.setServer( mqtt_server, MQTT_PORT );
  client.setCallback( callback );

  while ( Ethernet.begin( client_mac ) == 0 ) {
     Serial.println( "Cannot get a valid IP address from DHCP server..." );
     Serial.println( "Reconnect to DHCP server..." );
     delay(1000);
  }
  Serial.print( "IP address: " );
  Serial.println( Ethernet.localIP() );
}

boolean reconnect() { // non-blocking reconnect
  if ( client.connect( MQTT_CLIENT_ID ) ) {
     client.publish( MQTT_TOPIC, "connect" );
     client.subscribe( MQTT_TOPIC );
  }
  return client.connected(); // returns true if connected, otherwise false.
}

void loop() {
   switch (state) {
     case ST_READY:
       if ( !client.connected() ) { // not connected
          Serial.println( "Reconnect to MQTT broker.. " );
          reconnect();
       }
       if ( digitalRead( BT_PIN ) == LOW ) { // buttion pressed
          delay(5);
          while ( digitalRead( BT_PIN ) == LOW ) { delay(5); } // wait until the button is released
          state = ST_RUNNING;   // go to the ST_RUNNING state
          Serial.println( "Start sending messages..." );
       }
       break;
       
     case ST_RUNNING:
       ts0 = micros(); // read and save the current timestamp to ts0
       if ( !wait_for_response ) { // send next message to the MQTT broker
           sprintf( sbuf, "{id:'1234',count:%d,extra:'00000000000000000000'}", (cnt+1) );    
           client.publish( MQTT_TOPIC, sbuf );
           ts1 = micros(); // read and save the current timestamp to ts1
           cnt++;
           wait_for_response = true;
           ts_saved = ts1;
           Serial.print( "publish() done in (usec): " );
           Serial.println( (ts1 - ts0) );
       } 
       else { // wait for response (incoming message from MQTT broker)
           if ( micros() - ts_saved >= 1000000UL ) { // timeout
              state = ST_FINISHED;
              wait_for_response = false;
              cnt = 0;
              Serial.println( "Timeout...\n\n" );
           }
       }
       if ( cnt >= MAX_COUNT ) {
          state = ST_FINISHED;
          cnt = 0;
       }
       client.loop(); 
       break;
       
     case ST_FINISHED:
       if ( wait_for_response == false ) {
          Serial.println( "Stopped...\n\n" );
          Serial.println( "Press the button to start the next test run..." );
          state = ST_READY;
       }
       client.loop(); 
       break;
       
     default:
       state = ST_READY;
       break;
   } // end-of-switch    
}

// handles message arrived on subscribed topic(s)
void callback( char* topic, byte* payload, unsigned int len ) {
  wait_for_response = false;
  uint32_t t = micros() - ts_saved;
  memcpy( sbuf, payload, len );
  sbuf[len] = '\0';
  Serial.print( "recv: '" + String(sbuf) );
  Serial.print( "' (" + String(strlen(sbuf)) );
  Serial.print( ") after (usec): " );
  Serial.println( t );
}
//////////////////////////////////////////////////////////////////////////////////////

