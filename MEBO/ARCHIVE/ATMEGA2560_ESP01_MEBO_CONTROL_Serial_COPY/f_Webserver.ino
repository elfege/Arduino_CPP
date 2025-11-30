#include "WiFiEsp.h"
// use a ring buffer to increase speed and reduce memory allocation
RingBuffer buf(8);

// Emulate Serial2 on pins if not present
#ifndef HAVE_HWSERIAL2
#include "SoftwareSerial.h"
SoftwareSerial Serial2(17, 16); // RX, TX
#endif


char ssida[] = "WIFI_SSID2";            // your network SSID (name)
char passa[] = "WIFI_PASSWORDGahsh4479#$";      // your network password

int ledStatus = LOW;
int status = WL_IDLE_STATUS;



WiFiEspServer server(80);


void webserver() {

  WiFiEspClient client = server.available();  // listen for incoming clients

  if (client) {
    NoSt = true; // pause ST

    if (status != WL_CONNECTED) {
      // initialize second ESP module
      Serial.println("Reinitializing Wifi Serial 2");
      WiFi.init(&Serial2); // done by SmarThingsWiFiEsp.cpp line 62

      while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssida);
        // Connect to WPA/WPA2 network
        status = WiFi.begin(ssida, passa);
      }
    }

    // if you get a client,
    Serial.println("New client");             // print a message out the serial port
    buf.init();                               // initialize the circular buffer
    while (client.connected()) {              // loop while the client's connected
      if (client.available()) {               // if there's bytes to read from the client,
        char c = client.read();               // read a byte, then
        buf.push(c);                          // push it to the ring buffer

        // printing the stream to the serial monitor will slow down
        // the receiving of data from the ESP filling the serial buffer
        //Serial.write(c);

        // you got two newline characters in a row
        // that's the end of the HTTP request, so send a response
        if (buf.endsWith("\r\n\r\n")) {
          // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
          // and a content-type so the client knows what's coming, then a blank line:
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/html");
          client.println();

          // the content of the HTTP response follows the header:
          client.print("The LED is ");
          client.print(ledStatus);
          client.println("<br>");
          client.println("<br>");

          client.println("Click <a href=\"/H\">here</a> turn the LED on<br>");
          client.println("Click <a href=\"/L\">here</a> turn the LED off<br>");

          // The HTTP response ends with another blank line:
          client.println();
          break;
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (buf.endsWith("GET /H")) {
          Serial.println("Turn led ON");
          ledStatus = HIGH;
          digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
        else if (buf.endsWith("GET /L")) {
          Serial.println("Turn led OFF");
          ledStatus = LOW;
          digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        }
      }
    }

    // close the connection
    client.stop();
    Serial.println("Client disconnected");
    // initialize second ESP module

    Serial.println("Reinitializing Wifi Serial 1");
    WiFi.init(&Serial1); // done by SmarThingsWiFiEsp.cpp line 62

    NoSt = false; // resume ST
  }
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}

