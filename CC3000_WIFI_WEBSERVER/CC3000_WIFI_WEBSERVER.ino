

#include <Adafruit_CC3000.h>
#include <SPI.h>
#include "utility/debug.h"
#include "utility/socket.h"
#include <Adafruit_CC3000.h>
#include <Adafruit_CC3000_Server.h>
#include <SPI.h>

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
#define WLAN_SSID       "WIFI_SSID"           // cannot be longer than 32 characters!
#define WLAN_PASS       "WIFI_PASSWORDGahsh4479#$"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2
#define LISTEN_PORT           80
Adafruit_CC3000_Server www(LISTEN_PORT); //(LISTEN_PORT);

/* --- Webpage variables --- */
String req;     // Will handle the request of the client.
String reqsub;  // Request substring to for keyword "on" or "off".
char webpage[404] = "<!DOCTYPE HTML><html><body><center><h1>Wireless Controller via Wi-Fi</h1><p>by e-Gizmo</p><br><form method=\"get\"><input type=\"button\"style=\"height:100px;width:100px\" value=\"ON\"onclick=\"window.location='/?on'\">&nbsp;<input type=\"button\"style=\"height:100px;width:100px\"value=\"OFF\"onclick=\"window.location='/?off'\"></form></center></body></html>";


void(* SoftReset) (void) = 0;
long count = 0;
int on;

int Start()
{
  digitalWrite(8, LOW); //
delay(3000);
digitalWrite(8, HIGH); //
}

int off()
{
  digitalWrite(8, LOW); //
  delay(1000);
  //digitalWrite(8, HIGH); //
  on = 0;
}

void setup(void)
{

  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  Serial.begin(115200);
  //Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);
  /* Initialise the module */
  Serial.println("Initializing...");
  if (!cc3000.begin())
  {
    Serial.println("Couldn't begin()! Check your wiring?");
    while (1);
  }
  //Delete any old connection data on the module
  /* Serial.println(F("\nDeleting old connection profiles"));
     if (!cc3000.deleteProfiles()) {
      Serial.println(F("Failed!"));
      while (1);
     }
  */
  Serial.print("Connecting to "); Serial.println(WLAN_SSID);
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println("Failed!");
    while (1);
  }
  Serial.println("Connected!");

  //Serial.println("Request DHCP...");
  //cc3000.checkDHCP();

  //delay(3000);
  //  Display the IP address DNS, Gateway, etc.
  while (! displayConnectionDetails()) {
    delay(1000);
  }
  //displayConnectionDetails();
  www.begin();  // Start listening for connections
  Serial.println("Waiting for Client...");
}

void loop(void) {
  count++;
  Serial.println(count);

  if(count > 5000){
    SoftReset();
  }
  
  if (!cc3000.checkConnected()) {
    Serial.println("CONNECTION LOST!");
    SoftReset();
    //while (1) does this: the code turns on the watchdog timer.
    //It then goes into an infinite loop.  The watchdog timer
    //times out after about 15ms and resets the processor
  }
  else {
    // Try to get a client which is connected.

    Adafruit_CC3000_ClientRef client = www.available();
    if (client) {
      Serial.println("client connected!");
      boolean currentLineIsBlank = true;
      String buffer = "";
      while (client.connected()) {
        if (client.available()) {
          char c = client.read();
          //Serial.print(c);
          buffer += c;
          if (c == '\n' && currentLineIsBlank) {

            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
            client.print("<form action=\"http://192.168.10.23/\" >");
            client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"10\">START</p>");
            client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"11\">STOP</p>");
            client.print("<p align=\"center\"><input type=\"submit\" value=\"Submit\"> </p></form>");
            break;
          }
          if (c == '\n') {
            currentLineIsBlank = true;
            buffer = "";
          }
          else if (c == '\r') {

            if (buffer.indexOf("GET /?status=10") >= 0)
            {
              Start();
            }
            if (buffer.indexOf("GET /?status=11") >= 0)
            {
              on = 0;
              off();
            }
          }
          else {

            currentLineIsBlank = false;
          }
        }
      }

      {
        delay(500);
        client.stop();
        //www.begin();
      }
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

  if (!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}
