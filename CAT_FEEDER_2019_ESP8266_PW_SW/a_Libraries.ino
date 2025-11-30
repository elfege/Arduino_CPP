#include <ESP8266Ping.h>

//#include <SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB.h>
#include <SmartThingsESP8266WiFi_DUAL_SSID.h>
//#include <SmartThingsESP8266WiFi.h>
//#include <SmartThingsESP8266_WIFI_SSID.h>

// call out function forward decalaration
SmartThingsCallout_t messageCallout;

#include <ESP8266Ping.h>


#include "Arduino.h"
#include <Streaming.h>      // Ref: http://arduiniana.org/libraries/streaming/ // not compatible under ESP32 hardware Serial


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "index.h"  //Web page header file


//******************************************************************************************
/// Arduino OTA (redundent but necessary for custom name for now)
//******************************************************************************************
#include <ArduinoOTA.h>
