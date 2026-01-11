
/************************************** NTP CONNECTIVITY *****************************************/

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const long utcOffsetInSeconds = -14400;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

/************************************** ****************** *****************************************/


//******************************************************************************************
/// Wifi (for ajax webpage)
//******************************************************************************************

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "index.h"  //Web page header file
#include <ESP8266Ping.h>


//#include <SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB.h>
//#include <SmartThingsESP8266WiFi_DUAL_SSID.h>
#include <SmartThingsESP8266WiFi.h>

// call out function forward decalaration
SmartThingsCallout_t messageCallout;

//******************************************************************************************
/// Arduino OTA (redundent but necessary for custom name for now)
//******************************************************************************************
#include <ArduinoOTA.h>


//*****************************************************************************
/// @file
/// @brief
///   Arduino SmartThings Ethernet ESP8266 WiFi PIR motion sensor
///
///   Revised by Dan Ogorchock on 2017-02-11 to work with new "SmartThings v2.0" Library
///
///   Notes: The NodeMCU ESP communicates via WiFi to your home network router,
///          then to the ST Hub, and eventually to the ST cloud servers.
///
///
//*****************************************************************************
