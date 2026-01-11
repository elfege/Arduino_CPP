#include <Adafruit_CC3000.h>
#include <Adafruit_CC3000_Server.h>
#include <SPI.h>

/*
Web Server REST controller web page
by Barry Rutkin 

This software is in the public domain

This controller was writen for a Micro and Adafruit CC3000 Breakout Board.
It serves a simple web page with links to control a companion REST server based
on Adafruit's guide https://learn.adafruit.com/a-rest-api-for-arduino-and-the-cc3000-wifi-chip
This interface is configured to control LEDs connected to the REST server's pins 8 and 9.  The REST
server code was modified to make an LED on pin 7 blink and is controlled by the exposed REST function.
Links are provided to initialize the mode of each pin to output, plus links to control the LEDs and retrieve
exposed variables. 
This interface listens on port 888.

** Note **
Address resolution of the REST server depends on DHCP host names.
You will have to configure your router's DHCP server to set the REST server's hostname, or you can assign 
a static IP to the REST server and use that in the links instead.
*/

//#define DEBUG  //  uncomment for serial debug output

// These are the pins for the CC3000 breakout board, plus the hardware SPI pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// CC3000 Status LEDs
#define redLED                 2  //  CC3000 not intialized
#define yellowLED             12  //  Attempting to connect to WLAN
#define greenLED              11  //  WiFi Connected
#define glowLED                6  //  Proof-of-life  Must be a PWM pin

// Create CC3000 instance
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2);
// Wireless network configuration
#define WLAN_SSID       "WIFI_SSID"        // cannot be longer than 32 characters!
#define WLAN_PASS       "WIFI_PASSWORDGahsh4479#$"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2
#define     WLAN_ATTEMPTS   4

// The port to listen for incoming TCP connections 
#define LISTEN_PORT         888

// Server instance
Adafruit_CC3000_Server www(LISTEN_PORT);

String readString;

void setup(){
  pinMode(8, OUTPUT);
  //enable serial data print 
  
    Serial.begin(115200); 
    delay(500); // time to start serial monitor
    Serial.println("v1.3");

}

void loop(){

  Serial.print("LOOP");
  // the following "gaurd code" does the initial connect to the WLAN and then
  // checks the connect state each time throught loop(). 
  // If the CC3000 disconnects from the WLAN it will automatically attempt to
  // reconnect, waiting for the WLAN to recover or come back into range if necessary.
  if (!cc3000.checkConnected())      // make sure we are connected to wireless network
  {
    Serial.println("No connection");
    if (!init_network())    // try a cold start connect to WLAN
    
    {
      delay(2000);     // if we couldn't connect, try again later
      return;
    }
    // put any code that needs to be run after a sucessful WLAN connect/reconnecrt here
    www.begin(); // start the REST interface web server
    
      Serial.println("Listening for connections ...");
    
  }
   // Create a client connection
  Adafruit_CC3000_ClientRef client = www.available();
  if (client) {
    while (client.connected()) 
    {
      if (client.available())  //  <-- gets stuck between here ...
      {
        char c = client.read();
        //read char by char HTTP request
        if (readString.length() < 100) 
        {
          //copy characters to a string 
          readString += c; 
          //Serial.print(c);
        } 
        //if HTTP request has ended
        if (c == '\n') 
        {
          #ifdef DEBUG
            Serial.println(readString); //print to serial monitor for debugging
          #endif
          readString="";          //now output HTML data header
          client.fastrprintln(F("HTTP/1.1 200 OK"));
          client.fastrprintln(F("Content-Type: text/html"));
          client.fastrprintln(F("connection: close"));
          client.fastrprintln("");
          
          client.fastrprintln(F("<html>"));
          client.fastrprintln(F("<head>"));
          client.fastrprintln(F("<title>Arduino REST Controller</title>"));
          client.fastrprintln("</head>");
          client.fastrprintln("<body>");
          client.fastrprintln(F("<h1 style='text-align: center;'><span style='color: #3333ff;'>Arduino REST Controller</span></h1>"));                
          client.fastrprintln(F("<h2 style='text-align: center;'>Controls</h2>"));
          client.fastrprintln(F("<p style='text-align: center;'><span style='font-size:180%'</span>"));
          // If necessary, modify the links below to use the IP or a router-assigned name
          client.fastrprintln(F("Blink&nbsp&nbsp"));
          client.fastrprintln(F("<a href='http://CC3000-3:887/led?params=1' target='dispFrame'>ON</a>"));
          client.fastrprintln(F("&nbsp&nbsp"));
          client.fastrprintln(F("<a href='http://CC3000-3:887/led?params=0' target='dispFrame'>OFF</a>")); 
          client.fastrprintln(F("&nbsp&nbsp"));
          client.fastrprintln(F("<a href='http://CC3000-3:887/mode/7/o' target='dispFrame'>INIT</a>"));
           
          client.fastrprintln(F("<br><br>LED1&nbsp"));
          client.fastrprintln(F("<a href='http://CC3000-3:887/digital/8/1' target='dispFrame'>ON</a>")); 
          client.fastrprintln(F("&nbsp&nbsp"));
          client.fastrprintln(F("<a href='http://CC3000-3:887/digital/8/0' target='dispFrame'>OFF</a>")); 
          client.fastrprintln(F("&nbsp&nbsp"));
          client.fastrprintln(F("<a href='http://CC3000-3:887/mode/8/o' target='dispFrame'>INIT</a>"));
          
          client.fastrprintln(F("<br><br>LED2&nbsp"));
          client.fastrprintln(F("<a href='http://CC3000-3:887/digital/9/1' target='dispFrame'>ON</a>")); 
          client.fastrprintln(F("&nbsp&nbsp"));
          client.fastrprintln(F("<a href='http://CC3000-3:887/digital/9/0' target='dispFrame'>OFF</a>")); 
          client.fastrprintln(F("&nbsp&nbsp"));
          client.fastrprintln(F("<a href='http://CC3000-3:887/mode/9/o' target='dispFrame'>INIT</a>"));
          client.fastrprintln(F("</p>"));
          
          client.fastrprintln(F("<h2 style='text-align: center;'>Queries</h2>"));
          client.fastrprintln(F("<p style='text-align: center;'><span style='font-size:180%'</span>"));
          client.fastrprintln(F("<a href='http://CC3000-3:887/id' target='dispFrame'>ID</a>"));
          client.fastrprintln(F("&nbsp&nbsp"));
          client.fastrprintln(F("<a href='http://CC3000-3:887/' target='dispFrame'>Info</a>"));
          client.fastrprintln(F("&nbsp&nbsp"));
          client.fastrprintln(F("<a href='http://CC3000-3:887/Connects' target='dispFrame'>Connects</a>"));
          client.fastrprintln(F("</p>"));
          
          //  Display the json data returned by the REST server here
          //client.fastrprintln(F("<iframe name='dispFrame'  style='display: none'>"));        
          client.fastrprintln(F("<br>"));
          client.fastrprintln(F("<h2 style='text-align: center;'>REST Server Return Data</h2>"));                
          client.fastrprintln(F("<p style='text-align: center'>"));
          client.fastrprintln(F("<iframe name='dispFrame' width='600' height='30'>"));          
          client.fastrprintln(F("</iframe>"));
          
          client.fastrprintln(F("</p>"));
          client.fastrprintln(F("</body>"));
          client.fastrprintln(F("</html>"));
          delay(10);  //  <-- ... and here
          //close connection
          client.close();
        }
      }
    }
  }
  glow_led();  // Proof of life indicator
} 

int8_t init_network(void)
{
  cc3000.reboot();
  // Set up the CC3000, connect to the access point, and get an IP address.
  #ifdef DEBUG
    Serial.println(F("Initializing CC3000..."));
  #endif
  digitalWrite(greenLED, LOW);  // green off
  digitalWrite(redLED, HIGH);   // red on
  digitalWrite(yellowLED, LOW); // yellow off
  //digitalWrite(ledPin, LOW);    // on-board LED off

  if (!cc3000.begin())  // fatal
  {                     // the code below will never execute since the call hangs when it fails
    #ifdef DEBUG
      Serial.println(F("Couldn't begin()"));
    #endif
    while(1);
  }
  digitalWrite(redLED, LOW);      // red off
  digitalWrite(yellowLED, HIGH);  // yellow on

  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY, WLAN_ATTEMPTS)) 
  {
    #ifdef DEBUG
      //Serial.println(F("Failed to connect to AP!"));
    #endif
    return false; // fail. Leaves yellow LED on
  }

  digitalWrite(yellowLED, LOW);  // successful connect so turn off yellow LED

  #ifdef DEBUG
    Serial.println(F("Connected to Wireless Network!"));
    Serial.println(F("Request DHCP..."));
  #endif
  while (!cc3000.checkDHCP())
  {
    delay(100);
  }
  #ifdef DEBUG
    Serial.println(F("Got IP"));
  #endif
  digitalWrite(greenLED, HIGH);  //  and turn on green LED
  return true;  // success
}

// Glow status LED
void glow_led() 
{
  unsigned long i = millis();
  int j = i % 2000;
  if (j > 1000) 
  { 
      j = 2000 - j;
  }
  analogWrite(glowLED, j/4);
}

