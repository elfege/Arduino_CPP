// DO NOT EVER ATTEMPT AGAIN TO USE ESP32 WITH IRREMOTE IT SIMPLY DOESN'T WORK WITH SEND() !!! YOU WASTED TWICE AN ENTIRE DAY
// BECAUSE YOU FUCKING ALZHEIMER'S IDIOT FORGOT IT HAD ALREADY BEEN ESTABLISHED INSIDE YOUR FUCKING STUPID LITTLE BRAIN !!!

//*****************************************************************************
/// @file
/// @brief
///   Arduino SmartThings Ethernet ESP8266 WiFi AC REMOTE CONTROL
///
//*****************************************************************************

//******************************************************************************************
/// Libraries
//******************************************************************************************
#include <avr/pgmspace.h> // allows storage of variable in PROGMEM instead of SRAM
#include <IRremoteESP8266.h>
#define DELAY_CMD 400
#include <NTPClient_Generic.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SmartThingsESP8266WiFi_DUAL_SSID_3_HUBS.h>
//#include <SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB.h>
//#include <SmartThingsESP8266WiFi_DUAL_SSID.h>
//#include <SmartThingsESP8266WiFi.h>
//#include <SmartThingsESP8266_WIFI_SSID.h>

// call out function forward decalaration
SmartThingsCallout_t messageCallout;

//******************************************************************************************
/// NTP CLIENT
//******************************************************************************************
// Define NTP Client to get time
WiFiUDP ntpUDP;

// NTP server
//World
//char timeServer[] = "time.nist.gov"; // WIFI_SSID: ping fails for this one...
// Canada
char timeServer[] = "0.ca.pool.ntp.org";
//char timeServer[] = "1.ca.pool.ntp.org";
//char timeServer[] = "2.ca.pool.ntp.org";
//char timeServer[] = "3.ca.pool.ntp.org";
// Europe
//char timeServer[] = ""europe.pool.ntp.org";

#define TIME_ZONE_OFFSET_HRS            (-4)
#define NTP_UPDATE_INTERVAL_MS          60000L

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, timeServer, (3600 * TIME_ZONE_OFFSET_HRS), NTP_UPDATE_INTERVAL_MS);

//******************************************************************************************
/// ajax headers
//******************************************************************************************
#include "index.h"  //Web page header file
#include "index2.h" // simplified page


//******************************************************************************************
/// Wifi Terminal
//******************************************************************************************
#include "WiFiTerm.h"// OTA DEBUG LIB Wifi Terminal


//******************************************************************************************
/// Arduino OTA (redundent but necessary for custom name for now)
//******************************************************************************************
#include <ArduinoOTA.h>
