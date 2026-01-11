

/************************************** NTP CONNECTIVITY *****************************************/

#include <NTPClient_Generic.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

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


/************************************** ****************** *****************************************/


//******************************************************************************************
/// Wifi (for ajax webpage)
//******************************************************************************************

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "index.h"  //Web page header file
#include "index2.h" // simplified page


#include "WiFiTerm.h"// OTA DEBUG LIB Wifi Terminal
 


#include <SmartThingsESP8266WiFi_DUAL_SSID_3_HUBS.h>
//#include <SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB.h>
//#include <SmartThingsESP8266WiFi_DUAL_SSID.h>
//#include <SmartThingsESP8266WiFi.h>
//#include <SmartThingsESP8266_WIFI_SSID.h>

// call out function forward decalaration
SmartThingsCallout_t messageCallout;

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
int ULTRASLOW = 550; //PWM signals on ESP8266 have 10-bit resolution
int reset_counter;
int resetFINALSPEED = 500; // this is the default value after a cold reset (power off/on)
int FINALSPEED; // will be updated by learning process // we want to keep these values after reset
int speedVal;
int lastOperation;
int program_step;
int doneLearning;
