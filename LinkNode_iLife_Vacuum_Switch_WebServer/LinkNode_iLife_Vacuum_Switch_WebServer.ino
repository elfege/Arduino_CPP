/******************************************
   The unmodified version of this code is originally
   by kakopappa and can be found at http://bit.ly/2kKQiRg.

   This version of the code has been modified by Charles Gantt
   and requires five additional files which can be found at http://bit.ly/2lRDwAJ

   Find out more about this project on Charles' website
   http://www.themakersworkbench.com

   Follow Charles and TheMakersWorkbench on the following sites:
   YouTube: bit.ly/TMWB-on-YouTube
   TMWB on Facebook: bit.ly/TMWB-on-Facebook
   CJGanttMakes on Facebook: bit.ly/CJGanttMakes
   TMWB on Twitter: bit.ly/TMWB-on-Twitter
   Charles Gantt on Twitter: bit.ly/CJGanttOnTwitter
   Instructables: bit.ly/CJGanttOnInstructables
   TMWB Website: bit.ly/TheMakersWorkbench
   Charles Gantt on Element14: bit.ly/CJGantt-On-E14
   Charles Gantt on GitHub: bit.ly/CJGantt-on-Github
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include <Wire.h>

// prototypes
boolean connectWifi();

//on/off callbacks
void lightOneOn();
void lightOneOff();


// Change this before you flash
const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORDGahsh4479#$";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *lightOne = NULL;
Switch *lightTwo = NULL;
Switch *outletOne = NULL;
Switch *outletTwo = NULL;

// Set Relay Pins
int relayOne = 14;
int relayTwo = 15;
int relayThree = 03;
int relayFour = 01;



void setup()
{
 
  //Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();
  //Serial.print("WiFi Connected");

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

   

    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port no, on callback, off callback
    lightOne = new Switch("Light One", 80, lightOneOn, lightOneOff);
    lightTwo = new Switch("Light Two", 81, lightTwoOn, lightTwoOff);
    
    //Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*lightOne);
 
    upnpBroadcastResponder.addDevice(*outletOne);
   

    //Set relay pins to outputs
    pinMode(14, OUTPUT);
    pinMode(15, OUTPUT);
    pinMode(03, OUTPUT);
    pinMode(01, OUTPUT);

  

    //Set each relay pin to HIGH this display status messagefor each relay
    digitalWrite(relayOne, HIGH);   // sets relayOne on
    


  }
}

void loop()
{
  if (wifiConnected) {
    upnpBroadcastResponder.serverLoop();
    lightOne->serverLoop();
    lightTwo->serverLoop();
    outletOne->serverLoop();
    outletTwo->serverLoop();
  }
}

void lightOneOn() {
  // Serial.print("Switch 1 turn on ...");
  digitalWrite(relayOne, LOW);   // sets relayOne on
 
}

void lightOneOff() {
  // Serial.print("Switch 1 turn off ...");
  digitalWrite(relayOne, HIGH);   // sets relayOne off

}


// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  // Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }

  if (state) {
    //  Serial.println("");
    //  Serial.print("Connected to ");
    //  Serial.println(ssid);
    // Serial.print("IP address: ");
    //  Serial.println(WiFi.localIP());
  }
  else {
    // Serial.println("");
    //Serial.println("Connection failed.");
  }

  return state;
}
