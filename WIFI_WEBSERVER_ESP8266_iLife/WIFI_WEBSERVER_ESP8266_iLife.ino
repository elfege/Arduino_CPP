// variable into chars library
#include <stdio.h>
// definition for var into chars
#define MAX_OUT_CHARS 16  //max nbr of characters to be sent on any one serial command

// ping library
#include <ESP8266Ping.h>

//*****************************************************************************
/// @file
/// @brief
///   Arduino SmartThings Ethernet ESP8266 WiFi On/Off with LED Example
///
///   Revised by Dan Ogorchock on 2017-02-11 to work with new "SmartThings v2.0" Library
///
///   Notes: The NodeMCU ESP communicates via WiFi to your home network router,
///          then to the ST Hub, and eventually to the ST cloud servers.
///
///
//*****************************************************************************

#include <SmartThingsESP8266WiFi.h>
//#include <ESP8266WiFi.h>  //should not be needed - DanO
//#include <SoftwareSerial.h> //should not be needed here - DanO

//*****************************************************************************
// Pin Definitions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                    V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
//******************************************************************************************
//NodeMCU ESP8266 Pin Definitions (makes it much easier as these match the board markings)
//******************************************************************************************

#define D0 16 //no internal pullup resistor
#define D1  5
#define D2  4
#define D3  0 //must not be pulled low during power on/reset, toggles value during boot
#define D4  2 //must not be pulled low during power on/reset, toggles value during boot
#define D5 14
#define D6 12
#define D7 13
#define D8 15 //must not be pulled high during power on/reset
#define D9  3
#define D10 1

//Defining Pins
#define LED D4
#define ASensor A0 // running input
#define switchPin D5 // switch Pin output

// call out function forward decalaration
SmartThingsCallout_t messageCallout;
//*****************************************************************************
// Global Variables   | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                    V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
bool wakingup = false;
bool turnOffRequested = false;
bool Alreadyrestarted = false;
bool reseting = false;
int resetcount = 0;

int count;
int AWAKE;
int AwakeVolt = 600;
int RUNNING;
int blinks;
int blinksCount;
int lapse = 200;


//******************************************************************************************
//ESP8266 WiFi Information    CHANGE THIS INFORMATION ACCORDINGLY FOR YOUR NETWORK!
//******************************************************************************************
// mac address is : 5CCF7FA34F16
String str_ssid     = "WIFI_SSID";                           //  <---You must edit this line!
String str_password = "WIFI_PASSWORDGahsh4479#$";                   //  <---You must edit this line!
IPAddress iptoping (192, 168, 10, 2);   // The remote ip to ping
IPAddress ip(192, 168, 10, 207);       // Device IP Address      //  <---You must edit this line!
IPAddress gateway(192, 168, 10, 1);    //router gateway          //  <---You must edit this line!
IPAddress subnet(255, 255, 255, 0);   //LAN subnet mask         //  <---You must edit this line!
IPAddress dnsserver(192, 168, 10, 1);  //DNS server              //  <---You must edit this line!
const unsigned int serverPort = 8090; // port to run the http server on

// Smartthings Hub Information
IPAddress hubIp(192, 168, 10, 145);    // smartthings hub ip     //  <---You must edit this line!
const unsigned int hubPort = 39500;   // smartthings hub port

WiFiServer server(80);

//Create a SmartThings Ethernet ESP8266WiFi object
st::SmartThingsESP8266WiFi smartthing(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);

bool isDebugEnabled;    // enable or disable debug in this example
unsigned long previousMillis = 0; //needed for new timing mechanism - DanO
unsigned long previousMillisBlink = 0;
const int DELAY_SEC = 60; //in seconds (to facilitate debugging!) - DanO

//*****************************************************************************
// API Functions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void setup()
{
  // setup default state of global variables
  isDebugEnabled = true;

  if (isDebugEnabled)
  { // setup debug serial port
    Serial.begin(9600);         // setup serial with a baud rate of 9600
    Serial.println("");
    Serial.println("setup..");  // print out 'setup..' on start
  }

  // setup hardware pins
  //HC-SR04 Setup
  pinMode(switchPin, OUTPUT);     // define LED as an output
  digitalWrite(switchPin, 0); // off
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW); // turn on the LED

  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();
  

  
  server.begin();
  //synch up the ST cloud

  // led signals successful boot

 
  Blink(10, 50);
  Serial.println("Setup Successful");

}

//*****************************************************************************
void loop()
{
  
  smartthing.run();

  if (millis() - previousMillis > DELAY_SEC * 1000)
  {
    PingTest();
    Refresh();
    Blink(3, 600);
    previousMillis = millis();
  }
  if (millis() - previousMillisBlink > 1000)
  {
    Blink(2, 200);
    previousMillisBlink = millis();
  }
  
  webserver();
  delay(100);
  yield();
}

