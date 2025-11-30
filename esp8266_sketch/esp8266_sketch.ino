// Import required libraries
#include "ESP8266WiFi.h"
int ledPin = 2; //On the ESP-12, the onboard LED is connected to pin 2
 
// WiFi parameters
const char* ssid = "<redacted>";
const char* password = "<redacted>";
 
void setup(void)
{ 
// Start Serial
Serial.begin(115200);
 
// Connect to WiFi
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
 
// Print the IP address
Serial.println(WiFi.localIP());
 
pinMode(ledPin, OUTPUT);       // Initialize the BUILTIN_LED pin as an output
}
 
void loop() {
  digitalWrite(ledPin, LOW);   // Turn the LED on (Note that LOW is the voltage level
                               // but actually the LED is on; this is because 
                               // it is acive low on the ESP-01)
  delay(1000);                 // Wait for a second
  digitalWrite(ledPin, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(2000);                 // Wait for two seconds (to demonstrate the active low LED)
}
