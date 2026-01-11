#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <SPI.h>

#include <Adafruit_CC3000.h>
#include <Adafruit_CC3000_Server.h>
#include <ccspi.h>

//#include <SPI.h>
//#include <string.h>
//#include "utility/debug.h"
//#include <WiFi.h>


// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                         SPI_CLOCK_DIVIDER); // you can change this clock speed but DI



//SSID of your network
#define WLAN_SSID       "WIFI_SSID2"
char ssid[] = "WIFI_SSID2";
//password of your WPA Network
#define WLAN_PASS       "WIFI_PASSWORDGahsh4479#$"   
char pass[] = "WIFI_PASSWORDGahsh4479#$";
#define WLAN_SECURITY  WLAN_SEC_WPA2

void setup()
{
  Serial.begin(115200);
 WiFi.begin(ssid, pass);

  if (WiFi.status() != WL_CONNECTED) { 
    Serial.println("Couldn't get a wifi connection");
    while(true);
  } 
  // if you are connected, print out info about the connection:
  else {
   // print the received signal strength:
  long rssi = WiFi.RSSI(ssid);
  Serial.print("RSSI:");
  Serial.println(rssi);
  }
}


void loop () {
  delay(1000);

}
