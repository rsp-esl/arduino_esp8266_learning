//////////////////////////////////////////////////////////////////////////
// Author: Rawat S., Department of Electrical & Computer Engineering, KMUTNB, Thailand
// Date: 2017-07-05
// Arduino IDE: v1.8.2 + esp8266 v2.3.0
// Hardware: Boards with the ESP-12 module
////////////////////////////////////////////////////////////////////////////////////////

#include <SD.h>         // see: https://github.com/esp8266/Arduino/blob/master/libraries/SD/src/SD.h
#include <MD5Builder.h> // see: https://github.com/esp8266/Arduino/blob/master/cores/esp8266/MD5Builder.h

const char     *design = "DATA.TXT"; // This is the first file which is stored in the microSD.
const char *design_md5 = "DATA.MD5"; // This is the second file which contains the MD5 hex string of the first file.

MD5Builder md5;

// ESP8266 Pins for microSD: 
//   D8 = #SS  / GPIO-15
//   D7 = MOSI / GPIO-13
//   D6 = MISO / GPIO-12
//   D5 = SCLK / GPIO-14

const int SD_CS_PIN = 15; 

const int BTN_PIN = 4; // GPIO-4 / D2

#define MAX_BUF_SIZE   (512)

char buf[MAX_BUF_SIZE];

const char *getSdTypeString( int type ){
  if (type == SD_CARD_TYPE_SD1){
    return "SD1";
  } else if(type == SD_CARD_TYPE_SD2){
    return "SD2";
  } else if(type == SD_CARD_TYPE_SDHC){
    return "SDHC";
  } else {
    return "UNKNOWN";
  }
}

void SD_info() {
  String str;
  
  str = "SD FAT type: ";
  Serial.println( str + SD.fatType() );
  
  str = "SD Card type: ";
  Serial.println( str + getSdTypeString( SD.type()) );
  
  str = "SD cluster size: ";
  Serial.println( str + SD.clusterSize() ); // =  #blocks per cluster * block size
  
  str = "SD #clusters: ";
  Serial.println( str + SD.totalClusters() );
  
  str = "SD block size: ";
  Serial.println( str + SD.blockSize() );
  
  str = "SD #blocks per cluster: ";
  Serial.println( str + SD.blocksPerCluster() );
  
  size_t size_kb = (SD.clusterSize()/1024) * SD.totalClusters();
  str = "SD capacity = ";
  str += (size_kb / 1024);
  str += " MB";
  Serial.println( str );
}
       
void setup() {
  pinMode( BTN_PIN, INPUT_PULLUP );
  Serial.begin( 115200 );
  Serial.println("\n\n\n");
  delay(100);

  Serial.println( "ESP8266 microSD and MD5 demo.. \n" );
  
  while ( !SD.begin(SD_CS_PIN, 32000000 /* SPI speed*/) ) {
    Serial.println( "Card failed or not present" );
    delay(1000);
  }

  Serial.println( "Card initialized....\n" );
  SD_info();
  
  Serial.println( "Press the button to continue..." );
  while ( digitalRead( BTN_PIN ) == HIGH ) { delay( 10 ); }

  String str;
  String md5_hex_str = "";
  if ( SD.exists( (char *)design ) ) {
     File f = SD.open( design, O_READ );
     if ( f ) {
       size_t bytes_read_total = 0, bytes_read;
       str = "File size: ";
       str += f.size();
       Serial.println( str );
       md5.begin();
       while ( f.available() ) {
         bytes_read = f.read( buf, MAX_BUF_SIZE-1 );
         bytes_read_total += bytes_read;
         md5.add( (uint8_t *)buf, (uint16_t)bytes_read );
       }
       f.close();

       str = "Number of bytes read: ";
       str += bytes_read_total;
       Serial.println( str );
       
       md5.calculate(); // calculate the MD5 checksum 
       str = "MD5 Hex String (calculated): ";
       md5_hex_str = md5.toString();  // get the MD5 hex string
       str += md5_hex_str;
       Serial.println( str );
     }
  } else {
    str = "Cannot open file (no existing): ";
    str += design;
    Serial.println( str );
  }
  
  if ( SD.exists( (char *)design_md5 ) ) {
     File f = SD.open( design_md5, FILE_READ );
     str = "MD5 Hex string   (expected): ";
     while ( f.available() ) {
        char ch = f.read();
        if ( ch == '\n')  break;
        str += ch;
     }
     Serial.println( str );
  }
  else {
    str = "Cannot open file (no existing): ";
    str += design_md5;
    Serial.println( str );
    
    File f = SD.open( design_md5, O_WRITE | O_CREAT ); // create new file for write
    f.println( md5_hex_str );
    f.close();
  }
}

void loop() {
  delay(1000);
}

//////////////////////////////////////////////////////////////////////////

