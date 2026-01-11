// Import required libraries
#include "ESP8266WiFi.h"

int gpioA = 0;
int gpioB = 2;
int gpioC = 4;
int gpioD = 5;
int gpioE = 12;
int gpioF = 13;
int gpioG = 14;
int gpioH = 15;
int gpioI = 16;

//int TX = 1;
//int RX = 3;


// WiFi parameters
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORDGahsh4479#$";

WiFiServer server(80);


void setup(void)
{
  // Start Serial
  Serial.begin(9600);
  delay(10);
  pinMode(gpioA, OUTPUT);// Initialize gpioA as an output
  pinMode(gpioB, OUTPUT);
  pinMode(gpioC, OUTPUT);
  //pinMode(gpioD, OUTPUT);
  //pinMode(gpioE, OUTPUT);
  // pinMode(gpioF, OUTPUT);
  //pinMode(gpioG, OUTPUT);
  //pinMode(gpioH, OUTPUT);
  //pinMode(gpioI, OUTPUT);
  pinMode(TX, OUTPUT);

  digitalWrite(gpioA, 0);
  digitalWrite(gpioB, 0);
  digitalWrite(gpioC, 0);
  /*digitalWrite(gpioD, 0);
  digitalWrite(gpioE, 0);
  digitalWrite(gpioF, 0);
  digitalWrite(gpioG, 0);
  digitalWrite(gpioH, 0);
  digitalWrite(gpioI, 0);
  */
  digitalWrite(TX, 0);


  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Print the IP address
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Server started");

}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> turn on<br>");
            client.print("Click <a href=\"/L\">here</a> turn off<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {

          digitalWrite(gpioA, 1);
          digitalWrite(gpioB, 1);
          digitalWrite(gpioC, 1);
          /*digitalWrite(gpioD, 1);
          digitalWrite(gpioE, 1);
          digitalWrite(gpioF, 1);
          digitalWrite(gpioG, 1);
          digitalWrite(gpioH, 1);
          digitalWrite(gpioI, 1);
          */
          digitalWrite(TX, 1);
          Serial.println("");
          Serial.println("HIGH Signal sent");
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(gpioA, 0);
          digitalWrite(gpioB, 0);
          digitalWrite(gpioC, 0);
          /*digitalWrite(gpioD, 0);
          digitalWrite(gpioE, 0);
          digitalWrite(gpioF, 0);
          digitalWrite(gpioG, 0);
          digitalWrite(gpioH, 0);
          digitalWrite(gpioI, 0);
          */
          digitalWrite(TX, 0);
          Serial.println("");
          Serial.println("LOW Signal sent");
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

/*
  void ON() {
  digitalWrite(gpioA, LOW);   // Turn the LED on (Note that LOW is the voltage level
                               // but actually the LED is on; this is because
                               // it is acive low on the ESP-01)
  delay(1000);                 // Wait for a second
  digitalWrite(gpioA, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(2000);                 // Wait for two seconds (to demonstrate the active low LED)
  }
*/
