// MAC : 

#include <ArduinoOTA.h>


#include <SmartThingsESP8266WiFi_DUAL_SSID_3_HUBS.h>
//#include <SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB.h>
//#include <SmartThingsESP8266WiFi_DUAL_SSID.h>
//#include <SmartThingsESP8266WiFi.h>
//#include <SmartThingsESP8266_WIFI_SSID.h>


// call out function forward decalaration
SmartThingsCallout_t messageCallout;

//******************************************************************************************
/// Wifi (for ajax webpage)
//******************************************************************************************
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "index.h"  //Web page header file
