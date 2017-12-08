/////////////////////////////////////////////////////////////////////////////////
// Embedded Systems Lab (ESL) @ KMUTNB
// Date: 2017-05-30
// Target board: WeMos D1 Pro
/////////////////////////////////////////////////////////////////////////////////

#include <ESP8266WiFi.h>
#include "PubSubClient.h"

const char ssid[] = "XXXXXXXXXXXXX"; 
const char pass[] = "XXXXXXXXXXXXX"; 

#define BT_PIN          4            //  GPIO-4 / D2 pin

#define MAX_COUNT       200         // the number of messages to be sent
#define MQTT_PORT       1883        // the default port number of the MQTT broker
#define MQTT_CLIENT_ID  "espClient"
#define MQTT_TOPIC      "esl/esp/test"
#define WIFI_TX_POWER   (20.5)      // in dBm: 0 .. +20.5

IPAddress mqtt_server( 192, 168, 1, 9 );  // set the IP address of the MQTT server

void callback( char* topic, byte* payload, unsigned int len );

WiFiClient    espClient;
PubSubClient  client(espClient);

enum states  { ST_NONE=0, ST_READY, ST_RUNNING, ST_FINISHED };
typedef enum states state_t;
state_t state = ST_READY;

boolean running = false;
boolean wait_for_response = false;
uint32_t ts0, ts1, ts_saved;
uint16_t cnt = 0;
char sbuf[64];

void setup() {
  Serial.begin( 115200 );
  Serial.println( "\n\n\n\n" );
  delay(1000);
  WiFi.setOutputPower( WIFI_TX_POWER );
  WiFi.mode( WIFI_STA );
  WiFi.begin( ssid, pass );
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print( "." );
  }
  Serial.println( "" );
  Serial.println( "WiFi connected" );  
  Serial.println( "IP address: " );
  Serial.println( WiFi.localIP() );

  client.setServer(mqtt_server, MQTT_PORT );
  client.setCallback(callback);
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
          cnt = 0;
          wait_for_response = false;
          Serial.println( "Start sending messages..." );
       }
       break;
       
     case ST_RUNNING:
       ts0 = micros(); // read and save the current timestamp to ts0
       if ( !wait_for_response ) { // send next message to the MQTT broker
           sprintf( sbuf, "{id:'#123456789',count:%d}", (cnt+1) );
           client.publish( MQTT_TOPIC /* topic */, sbuf /* message */ );
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
              Serial.println( "Timeout...\n\n" );
           }
       }
       if ( cnt >= MAX_COUNT ) {
          state = ST_FINISHED;
       }
       client.loop(); 
       break;
       
     case ST_FINISHED:
       if ( wait_for_response == false ) {
          Serial.println( "Stopped...\n\n" );
          Serial.println( "Press the button to start the next test run..." );
          state = ST_READY;
       }
       if ( micros() - ts_saved >= 1000000UL ) { // timeout
          Serial.println( "Timeout...\n\n" );
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
  Serial.print( "' after (usec): " );
  Serial.println( t );
}
//////////////////////////////////////////////////////////////////////////////////////


