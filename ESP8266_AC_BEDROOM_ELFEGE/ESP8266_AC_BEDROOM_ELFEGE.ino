// DO NOT ATTEMPT TO USER OTHER LIBRARIES THAN THE ONE IN THIS INSTALL (arduino/libraries/IRremoteESP8266)
//- IT IS NOT THE SAME AS OFFICIAL (which doens't work with 32 bits integers)

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
#include <NTPClient_Generic.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SmartThingsESP8266WiFi_DUAL_SSID_3_HUBS.h>
// #include <SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB.h>
// #include <SmartThingsESP8266WiFi_DUAL_SSID.h>
// #include <SmartThingsESP8266WiFi.h>
// #include <SmartThingsESP8266_WIFI_SSID.h>

#include <ESP8266HTTPClient.h> // httpGet/Post requests
#include <ArduinoJson.h>

// call out function forward decalaration
SmartThingsCallout_t messageCallout;

//******************************************************************************************
/// NTP CLIENT
//******************************************************************************************
// Define NTP Client to get time
WiFiUDP ntpUDP;

// NTP server
// World
// char timeServer[] = "time.nist.gov"; // WIFI_SSID: ping fails for this one...
// Canada
char timeServer[] = "0.ca.pool.ntp.org";
// char timeServer[] = "1.ca.pool.ntp.org";
// char timeServer[] = "2.ca.pool.ntp.org";
// char timeServer[] = "3.ca.pool.ntp.org";
//  Europe
// char timeServer[] = ""europe.pool.ntp.org";

#define TIME_ZONE_OFFSET_HRS (-4)
#define NTP_UPDATE_INTERVAL_MS 60000L

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, timeServer, (3600 * TIME_ZONE_OFFSET_HRS), NTP_UPDATE_INTERVAL_MS);

//******************************************************************************************
/// ajax headers
//******************************************************************************************
#include "index.h"  //Web page header file
#include "index2.h" // simplified page
#include "WifiDisconnected.h"

//******************************************************************************************
/// Wifi Terminal
//******************************************************************************************
#include "WiFiTerm.h" // OTA DEBUG LIB Wifi Terminal

//******************************************************************************************
/// Arduino OTA (redundent but necessary for custom name for now)
//******************************************************************************************
#include <ArduinoOTA.h>

/******************************************************************************************
 RTC memory storage (resists soft resets, not hard resets)...
 By default, the maximum number of variables is 32, it can be set to a higher value if needed
 More needs more RAM, (about 6 Bytes per Variable), is a definition in lib's .h file
 number is limited by total RTC memory
 Use getFreeRTCMem() and getFreeRTCMem() to check at runtime
******************************************************************************************/
#include <RTCVars.h>
RTCVars state; // create the state object
int reset_counter;
int resetClosingSpeed = 430; // this is the default value after a cold reset (power off/on)
int lastOperation;
int fanMode;
int program_step;
int lastSetPointRTC;
int lastTimeCmdSent = 1000 * 60 * 5; // 5 minutes delay must be valid after fresh reboot...
int fanInsteadOfCoolRTC = 1; // RTC 1/0 true/false 