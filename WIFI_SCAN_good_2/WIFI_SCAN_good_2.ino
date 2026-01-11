
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
#include <WiFi.h>



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



void setup(void)
{
  Serial.begin(115200);
  Serial.println(F("Hello, CC3000!\n"));

  /* Initialise the module */
  Serial.println(F("\nInitialising the CC3000 ..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Unable to initialise the CC3000! Check your wiring?"));
    while (1);
  }

  /* Delete any old connection data on the module */
  /*Serial.println(F("\nDeleting old connection profiles"));
    if (!cc3000.deleteProfiles()) {
    Serial.println(F("Failed!"));
    while (1);
    }*/



  /* Attempt to connect to an access point */


  //int rssi = WiFi.RSSI(ssid);

}

void loop()
{

  int result_count = WiFi.scan(wifi_scan_callback);
  Serial.print(result_count);
  Serial.println(" APs scanned.");
}



void wifi_scan_callback(WiFiAccessPoint* wap, void* data)
{
  WiFiAccessPoint& ap = *wap;
  Serial.print("SSID: ");
  Serial.println(ap.ssid);
  Serial.print("Security: ");
  Serial.println(ap.security);
  Serial.print("Channel: ");
  Serial.println(ap.channel);
  Serial.print("RSSI: ");
  Serial.println(ap.rssi);
}
