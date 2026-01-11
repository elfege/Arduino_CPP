/*
WirelessControllerWifi is one of the applicatioin of CC3000 Wifi Module
                  - This sample application code simply turns on and off
                   an LED remotely via Wifi.
                  - It is suggested to use Atmega644 to avoid further
                    RAM issues.
                  - The IP address given to CC3000 by the Access Point
                    is static. Therefore, it is advisable to make sure
                    that the IP address entered to the browser is correct.
                   
Execution:
  1. Once uploaded to the microcontroller, open the serial.
  2. Wait for successful connection.
  3. When DHCP request is successful, Copy the IPaddress given to your device.
  4. Enter the IPaddress to your browser.
  5. The small Wireless Controller webpage will then be loaded.
  6. Click ON button to turn on the LED and OFF button to turn it off.

Wiring Connection:
  1. Connect +pin of LED(series with current limiting resistor if needed) to pin 14(A0).
             -pin to GND. This will be the LED to be controlled.
  2. Connect +pin of LED(series with current limiting resistor if needed) to pin 9.
             -pin to GND. This will be the wifi connection indicator.           
             
Notes:
  Reserved pins for CC3000 and SD Card communication are as follows.
    CC3000 Breakout Board              Gizduino
          SCK   <------------------> Digital pin 13
          MISO  <------------------> Digital pin 12
          MOSI  <------------------> Digital pin 11
          CS    <------------------> Digital pin 10
          V_EN  <------------------> Digital pin 5
          uSDCS <------------------> Digital pin 4
          IRQ   <------------------> Digital pin 3
References:
         - Adafruit_CC3000 ChatServer.ino
         - Ethernet WebServer.ino
         - Important Libraries from Adafruit: https://learn.adafruit.com/adafruit-cc3000-wifi/cc3000-library-software

Created On:  28 APR 14
        By:  e-Gizmo Mechatronix Central
*/

#include <Adafruit_CC3000.h>
#include <SPI.h>
#include "utility/debug.h"
#include "utility/socket.h"

/* Optional: Set a static IP address instead of using DHCP.
     Note that the setStaticIPAddress function will save its state
     in the CC3000's internal non-volatile memory and the details
     will be used the next time the CC3000 connects to a network.
     This means you only need to call the function once and the
     CC3000 will remember the connection details.  To switch back
     to using DHCP, call the setDHCP() function (again only needs
     to be called once).
  */



/* --- CC3000 constants --- */
// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIVIDER); // you can change this clock speed
#define WLAN_SSID       "WIFI_SSID2"           // cannot be longer than 32 characters!
#define WLAN_PASS       "WIFI_PASSWORDGahsh4479#$"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2
#define LISTEN_PORT           80 
Adafruit_CC3000_Server gizduinoServer(LISTEN_PORT);  //(LISTEN_PORT);

/* --- Webpage variables --- */
String req;     // Will handle the request of the client.
String reqsub;  // Request substring to for keyword "on" or "off".
char webpage[404] = "<!DOCTYPE HTML><html><body><center><h1>Wireless Controller via Wi-Fi</h1><p>by e-Gizmo</p><br><form method=\"get\"><input type=\"button\"style=\"height:100px;width:100px\" value=\"ON\"onclick=\"window.location='/?on'\">&nbsp;<input type=\"button\"style=\"height:100px;width:100px\"value=\"OFF\"onclick=\"window.location='/?off'\"></form></center></body></html>";

/* --- LED Indicators --- */
#define controlLED 14
#define wifiLED 9

  
  

void setup(void)
{
  pinMode(2, OUTPUT);
   pinMode(8, OUTPUT);
   digitalWrite(2, HIGH);
  digitalWrite(8, LOW);
  Serial.begin(9600);
  Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC); 
  /* Initialise the module */
  Serial.println("Initializing...");
  if (!cc3000.begin())
  {
    Serial.println("Couldn't begin()! Check your wiring?");
    while(1);
  } 
  Serial.print("Attempting to connect to "); Serial.println(WLAN_SSID);
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println("Failed!");
    while(1);
  }  
  Serial.println("Connected!");  
  /*Serial.println("Request DHCP...");
  //cc3000.checkDHCP();
  
    //delay(3000);
  // Display the IP address DNS, Gateway, etc.
  while (! displayConnectionDetails()) {  
    delay(1000);
  }  */
  displayConnectionDetails();
  gizduinoServer.begin();  // Start listening for connections
  Serial.println("Waiting for Client...");
  
  pinMode(wifiLED,OUTPUT);  // LED pin for Wifi connection indicator.
  digitalWrite(wifiLED,HIGH);
  pinMode(controlLED,OUTPUT);  // LED pin to be controlled.
  
}

void loop(void){
  
  if(!cc3000.checkConnected()){
    Serial.println("CONNECTION LOST!");
    digitalWrite(wifiLED,LOW);
    setup();
    while(1);
  }else{  
    // Try to get a client which is connected.
    Adafruit_CC3000_ClientRef client = gizduinoServer.available();
    if (client) {
      Serial.println("New client");
      // an http request ends with a blank line
      boolean currentLineIsBlank = true;
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          Serial.write(c);
          req += c;
          reqsub = req.substring(6,8);
          if (reqsub == "off"){
            digitalWrite(2, LOW);
            digitalWrite(8, HIGH);
          }
          if (reqsub == "on"){
            digitalWrite(2, HIGH);
            digitalWrite(8, LOW);
             
          }
          // Send a reply to the client
          if (c == '\n' && currentLineIsBlank) {
            // send a standard http response header
            client.println("HTTP/1.1 200 OK");
            //client.println("Content-Type: text/html");
            //client.println("Connection: close");  // the connection will be closed after completion of the response
            //client.println();
            // Give the web page to the client
            //client.println(webpage);
            //client.println("");       
            break;
          }
          if (c == '\n') {
            // you're starting a new line
            currentLineIsBlank = true;
          } 
          else if (c != '\r') {
            // you've gotten a character on the current line
            currentLineIsBlank = false;
          }
        }
      }    
      // give the web browser time to receive the data
      delay(100);
      // close the connection
      client.close();
      Serial.println("Client disconnected");
      req="";
    }    
  }
}

/**************************************************************************/
/*!
    @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    //Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    //Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    //Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    //Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    //Serial.println();
    return true;
  }
}
