//*****************************************************************************
//  ESP8266 DOOR INTERCOM CONTROLLER
//*****************************************************************************
//  
//  HARDWARE: NodeMCU v1.0 (ESP8266-12E)
//  
//  PURPOSE:
//  Automates building door unlock when someone rings the intercom.
//  
//  HOW IT WORKS:
//  1. Monitors voltage from intercom LCD power line (analog A0)
//  2. When voltage rises above threshold, someone rang the doorbell
//  3. Automatically "answers" the intercom (TALK relay)
//  4. Unlocks the building door (DOOR relay)
//  5. "Hangs up" the call (TALK relay again)
//  
//  FEATURES:
//  - Web UI for manual control and monitoring
//  - OTA (Over-The-Air) firmware updates
//  - WiFi terminal for remote debugging
//  - Dual SSID failover
//  - State machine architecture for reliable operation
//  
//  PIN USAGE:
//    A0 (analog) - Voltage sensor input (intercom LCD power)
//    D4 (GPIO2)  - Onboard LED (status indicator)
//    D5 (GPIO14) - TALK relay control
//    D7 (GPIO13) - DOOR relay control
//  
//  AUTHOR: Elfege
//  LAST UPDATE: 2025
//  
//*****************************************************************************

//*****************************************************************************
// LIBRARY INCLUDES
//*****************************************************************************
// System States types
#include "A_Types.h"

// ESP8266 WiFi stack
#include <ESP8266WiFi.h>

// HTTP web server for UI and API
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// WiFi-based serial terminal (for remote debugging without USB)
#include "WiFiTerm.h"

// OTA (Over-The-Air) update support
#include <ArduinoOTA.h>

// HTML page for web UI (stored in PROGMEM to save RAM)
#include "index.h"

//*****************************************************************************
// OPTIONAL/UNUSED INCLUDES (kept for reference)
//*****************************************************************************

// NTP time client - uncomment if time-based features are needed
// #include <NTPClient_Generic.h>
// #include <WiFiUdp.h>
// WiFiUDP ntpUDP;
// char timeServer[] = "0.ca.pool.ntp.org";
// #define TIME_ZONE_OFFSET_HRS (-4)
// #define NTP_UPDATE_INTERVAL_MS 60000L
// NTPClient timeClient(ntpUDP, timeServer, (3600 * TIME_ZONE_OFFSET_HRS), NTP_UPDATE_INTERVAL_MS);

// SmartThings/Hubitat library - disabled, using simpler HTTP approach
// #include <SmartThingsESP8266WiFi_DUAL_SSID_3_HUBS.h>

// RTC memory for persistence across soft resets - not currently used
// #include <RTCVars.h>
// RTCVars state;

//*****************************************************************************
// FILE ORGANIZATION
//*****************************************************************************
// 
// Arduino IDE concatenates all .ino files in a folder alphabetically.
// Files are named with prefixes to control load order:
//
//   A_PINS_Const_Vars.ino  - Pin definitions, constants, global variables
//   C_WiFi.ino             - WiFi configuration and credentials
//   C_XML.ino              - HTTP endpoint handlers
//   D_Setup.ino            - setup() function and initialization
//   E_Loop.ino             - loop() function and state machine
//   ESP8266_DOOR_...(this) - Main file, includes only (loaded first)
//   X_Functions.ino        - All other functions
//
// The main file (matching folder name) is always loaded first, so it
// contains only #includes. All code is in the other files.
//
//*****************************************************************************
