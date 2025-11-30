

/*SCAN SSID AND RETURNS RSSI*/

#include <Adafruit_CC3000.h>
#include <Adafruit_CC3000_Server.h>
#include <ccspi.h>
#include <SPI.h>

// may need to re-insert libraries... THIS IS THE REAL GOOD ONE NO DOUBT 


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
#define WLAN_SSID       "WIFI_SSID_0"
char ssid[] = "WIFI_SSID_0";
//password of your WPA Network
#define WLAN_PASS       "DFjuivb2013"
char pass[] = "DFjuivb2013";
#define WLAN_SECURITY  WLAN_SEC_WPA2

int CharN = 7;

void setup(void)
{
  Serial.begin(9600);
  Serial.println(F("Hello, CC3000!\n"));

  /* Initialise the module */
  Serial.println(F("\nInitialising the CC3000 ..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Unable to initialise the CC3000! Check your wiring?"));
    while (1);
  }
  //Delete any old connection data on the module
  /*Serial.println(F("\nDeleting old connection profiles"));
  if (!cc3000.deleteProfiles()) {
    Serial.println(F("Failed!"));
    while (1);
  }*/

}

void loop(void)
{
  uint32_t index;
  uint8_t valid, rssi, sec;
  char ssidname[33];
  char ssid[33] = WLAN_SSID;

  int dataSent;
  int rssiToSend;

  char correspondingSSID[33];

  if (!cc3000.startSSIDscan(&index)) {
    Serial.println(F("SSID scan failed!"));
    return;
  }

 while (index) {
    index--;
    valid = cc3000.getNextSSID(&rssi, &sec, ssidname);
    
    int ssidInt = ssid[CharN] + 0;
    int ssidnameInt = ssidname[CharN] + 0;

    if (ssidnameInt == ssidInt) {
      cc3000.stopSSIDscan();

      int ssidInt = ssid[CharN] + 0;
      int ssidnameInt = ssidname[CharN] + 0;

      // double check
      if (ssidnameInt == ssidInt) {
        rssiToSend = rssi;
        Serial.write(rssiToSend);
        dataSent = 1;

        break;
      }
    }
    cc3000.stopSSIDscan();
  }
  //cc3000.stopSSIDscan();
  delay(100);
}


void reset() {
  //reset Serial to clear it
  Serial.end();
  delay(200);
  Serial.begin(9600);
}




/*

    Serial.println();
        Serial.print("Data Sent to WIFI_SSID2: ");
        Serial.println();
        Serial.print("rssi value : "); Serial.print(rssi);
        Serial.println();
        Serial.print("for SSID : "); Serial.print(ssidname);
        Serial.println();
      Serial.println(F("================================================"));
       Serial.print(" ssid is: "); Serial.print(ssid);
       Serial.print(" ssidname is: "); Serial.print(ssidname);
       Serial.print(" ssid rssi value is: "); Serial.print(rssi);
       Serial.println(" ");
       Serial.println(F("================================================"));
       continueloop = 0;


       Serial.print("ssidnameInt is : "); Serial.print(ssidnameInt);
       Serial.print("SSID is : "); Serial.print(ssid);
       Serial.println();
       Serial.print("rssi value is : "); Serial.print(rssi);
       Serial.println();
       Serial.print("rssiToSend BEFORE serial reset : "); Serial.print(rssiToSend);
       Serial.println();
       Serial.println(F("================================================"));



       // reset Serial to clear it
       Serial.end();
       delay(200);
       Serial.begin(9600);


     // waiting to receive data request
     while (Serial.available() == 0) {
       Serial.print("waiting for data request");
       Serial.println();
       delay(10);
     }
     // read the incoming data from the ezb
     int incomingByte = Serial.read();

     // command 'a' means reset Serial port and transmit the current rssi value
     if (incomingByte == 'a') {

*/

