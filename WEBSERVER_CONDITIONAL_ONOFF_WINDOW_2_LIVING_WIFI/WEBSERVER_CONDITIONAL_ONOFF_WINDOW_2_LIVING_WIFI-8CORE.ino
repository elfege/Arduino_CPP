
/***************************************************
  Adafruit CC3000 Breakout/Shield Simple HTTP Server

  This is a simple implementation of a bare bones
  HTTP server that can respond to very simple requests.
  Note that this server is not meant to handle high
  load, concurrent connections, SSL, etc.  A 16mhz Arduino
  with 2K of memory can only handle so much complexity!
  This server example is best for very simple status messages
  or REST APIs.

  See the CC3000 tutorial on Adafruit's learning system
  for more information on setting up and using the
  CC3000:
  http://learn.adafruit.com/adafruit-cc3000-wifi

  Requirements:

  This sketch requires the Adafruit CC3000 library.  You can
  download the library from:
  https://github.com/adafruit/Adafruit_CC3000_Library

  For information on installing libraries in the Arduino IDE
  see this page:
  http://arduino.cc/en/Guide/Libraries

  Usage:

  Update the SSID and, if necessary, the CC3000 hardware pin
  information below, then run the sketch and check the
  output of the serial port.  After connecting to the
  wireless network successfully the sketch will output
  the IP address of the server and start listening for
  connections.  Once listening for connections, connect
  to the server IP from a web browser.  For example if your
  server is listening on IP 192.168.1.130 you would access
  http://192.168.1.130/ from your web browser.

  Created by Tony DiCola and adapted from HTTP server code created by Eric Friedrich.

  This code was adapted from Adafruit CC3000 library example
  code which has the following license:

  Designed specifically to work with the Adafruit WiFi products:
  ----> https://www.adafruit.com/products/1469

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
****************************************************/
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include "utility/debug.h"
#include "utility/socket.h"

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                         SPI_CLOCK_DIVIDER); // you can change this clock speed

#define WLAN_SSID       "WIFI_SSID"   // cannot be longer than 32 characters!
#define WLAN_PASS       "WIFI_PASSWORDGahsh4479#$"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define LISTEN_PORT           80      // What TCP port to listen on for connections.  
// The HTTP protocol uses port 80 by default.

#define MAX_ACTION            10      // Maximum length of the HTTP action that can be parsed.

#define MAX_PATH              64      // Maximum length of the HTTP request path that can be parsed.
// There isn't much memory available so keep this short!

#define BUFFER_SIZE           MAX_ACTION + MAX_PATH + 20  // Size of buffer for incoming request data.
// Since only the first line is parsed this
// needs to be as large as the maximum action
// and path plus a little for whitespace and
// HTTP version.

#define TIMEOUT_MS            500    // Amount of time in milliseconds to wait for
// an incoming request to finish.  Don't set this
// too high or your server could be slow to respond.

Adafruit_CC3000_Server httpServer(LISTEN_PORT);
uint8_t buffer[BUFFER_SIZE + 1];
int bufindex = 0;
char action[MAX_ACTION + 1];
char path[MAX_PATH + 1];

void setup(void)
{

  digitalWrite(2, HIGH);
  digitalWrite(8, HIGH);

  pinMode(2, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.begin(9600);
  Serial.println(F("Hello, CC3000!\n"));

  Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);

  // Initialise the module
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while (1);
  }

  Serial.print(F("\nAttempting to connect to ")); Serial.println(WLAN_SSID);
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while (1);
  }

  Serial.println(F("Connected!"));

  /*Serial.println(F("Request DHCP"));
    while (!cc3000.checkDHCP())
    {
    delay(100); // ToDo: Insert a DHCP timeout!
    }  */

  // Display the IP address DNS, Gateway, etc.
  while (! displayConnectionDetails()) {
    delay(1000);
  }

  // ******************************************************
  // You can safely remove this to save some flash memory!
  // ******************************************************
  // Serial.println(F("\r\nNOTE: This sketch may cause problems with other sketches"));
  // Serial.println(F("since the .disconnect() function is never called, so the"));
  // Serial.println(F("AP may refuse connection requests from the CC3000 until a"));
  // Serial.println(F("timeout period passes.  This is normal behaviour since"));
  //  Serial.println(F("there isn't an obvious moment to disconnect with a server.\r\n"));

  // Start listening for connections
  httpServer.begin();

  //Serial.println(F("Listening for connections..."));
}

int Open()
{
  digitalWrite(2, LOW); //
  digitalWrite(8, LOW); //

}

int Close()
{
  digitalWrite(2, HIGH); //
  digitalWrite(8, HIGH); //
}

int Stop()
{
  digitalWrite(2, LOW); //
  digitalWrite(8, HIGH); //
}

void loop(void)
{


  // reconnect wifi if disconnected
  if (!cc3000.checkConnected()) {
    Serial.println("CONNECTION LOST!");
    //digitalWrite(wifiLED, LOW);
    setup();
    //while(1);
  }
  else {
    // Try to get a client which is connected.
    Adafruit_CC3000_ClientRef client = httpServer.available();
    if (client) {
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


            client.print("<form action=\"http://192.168.10.204/\" >");
            if (digitalRead(8) == HIGH) {
              client.print("<p align=\"center\">Window LIVING is <font color='green'>OPEN</font></p>");
            }
            else {
              client.print("<p align=\"center\">Window LIVING is <font color='red'>CLOSED</font></p>");
            }
            client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"12\">STOP</p>");
            client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"11\">CLOSE</p>");
            client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"10\">OPEN</p>");

            client.print("<p align=\"center\"><input type=\"submit\" value=\"Submit\"> </p></form>");

            break;
          }
          if (c == '\n') {

            currentLineIsBlank = true;
            buffer = "";
          }
          else if (c == '\r') {
            if (buffer.indexOf("GET /?status=11") >= 0)
            {
              Close();
            }
            if (buffer.indexOf("GET /?status=10") >= 0)
            {
              Open();
            }
            if (buffer.indexOf("GET /?status=12") >= 0)
            {
              Stop();
            }

          }
          else {

            currentLineIsBlank = false;
          }
        }
      }

      {
        delay(500);
        Serial.println(F("Client disconnected"));
        client.close();
        //httpServer.begin();
      }
    }
  }
}



// Tries to read the IP address and other connection details
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
    /*Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
      Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
      Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
      Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
      Serial.println();*/
    return true;
  }
}
