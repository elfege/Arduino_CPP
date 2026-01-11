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

#include <ArduinoOTA.h>
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
#define D3  0 //must not be pulled low during power on/reset, toggles value during boot // must not be used as input
#define D4  2 //must not be pulled low during power on/reset, toggles value during boot  // must not be used as input
#define D5 14
#define D6 12
#define D7 13
#define D8 15 //must not be pulled high during power on/reset //PIO15 is always pulled low, so you can’t use the internal pull-up resistor
#define D9  3
#define D10 1

//Defining Pins
// GPIO 0-15 all have a built-in pull-up resistor, just like in an Arduino. GPIO16 has a built-in pull-down resistor.
#define LED D0
#define Run D6 // isRunning input (GPIO16) // yellow wire 
#define ASensor A0 // awake input // white wire 
#define switchPin D7 // switch Pin output // blue wire

#define RESET_PIN D2 // pin to connect to RST 


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
bool websent = false;
bool isDebugEnabled;    // enable or disable debug in this example
bool activateWeb = true;
unsigned long previousMillis = 0; //needed for new timing mechanism - DanO
unsigned long previousMillisBlink = 0;
const int DELAY_SEC = 60; //in seconds (to facilitate debugging!) - DanO


int ON = 0;
int OFF = 1;

int AwakeVolt = 50; // anlog value (0-1024) converted to voltage by ST device handler



//******************************************************************************************
//ESP8266 WiFi Information    CHANGE THIS INFORMATION ACCORDINGLY FOR YOUR NETWORK!
//******************************************************************************************
// mac address is : 5CCF7FACB458
String str_ssid     = "WIFI_SSID";                           //  <---You must edit this line!
String str_password = "WIFI_PASSWORDGahsh4479#$";                   //  <---You must edit this line!
IPAddress iptoping (192, 168, 10, 2);   // The remote ip to ping
IPAddress ip(192, 168, 10, 206);       // Device IP Address      //  <---You must edit this line!
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



//*****************************************************************************
// API Functions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void setup()
{
  // create OTA name
  const char *NameOTA = "NEATO";
  ArduinoOTA.setHostname(NameOTA);

  // setup default state of global variables
  isDebugEnabled = true;

  if (isDebugEnabled)
  { // setup debug serial port
    Serial.begin(115200);         // setup serial with a baud rate of 9600
    Serial.println("");
    Serial.println("setup..");  // print out 'setup..' on start
  }

  // setup hardware pins
  //HC-SR04 Setup
  pinMode(switchPin, OUTPUT);     //
  digitalWrite(switchPin, ON); // off // wires on relay are connected to Naturally Open

pinMode(ASensor, INPUT);
  pinMode(Run, INPUT_PULLUP); // isRunning input
  pinMode(LED, OUTPUT);
  pinMode(RESET_PIN, OUTPUT); //pulled down by digitalWrite to RST board
  digitalWrite(RESET_PIN, HIGH); //  HIGH by default 
  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();
  digitalWrite(LED, HIGH); // turn off the LED



  //synch up the ST cloud

  // led signals successful boot


  Blink(10, 50);

}

//*****************************************************************************
void loop()
{

  smartthing.run();

  if (millis() - previousMillis > DELAY_SEC * 1000)
  {
    //PingTest(); // let the library take care of this
    Refresh();
    //Blink(3, 600);
    previousMillis = millis();
  }
  if (millis() - previousMillisBlink > 1000)
  {
    Blink(2, 200);
    previousMillisBlink = millis();
  }

  if (activateWeb) {
    server.begin();
    delay(100);
    websent = false; // allow refresh for webserver value in st
    webserver();
  }
  if (millis() - previousMillis > DELAY_SEC * 120000 && activateWeb)
  {
    Serial.println("Stopping webserver");
    activateWeb = false; // stop webserver after
    websent = false; // allow refresh for webserver value in st
    server.stop();
  }


}

