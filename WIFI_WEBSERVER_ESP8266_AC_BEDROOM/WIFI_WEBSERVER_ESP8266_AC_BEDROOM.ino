// DO NOT EVER ATTEMPT AGAIN TO USE ESP32 WITH IRREMOTE IT SIMPLY DOESN'T WORK WITH SEND() !!! YOU WASTED TWICE AN ENTIRE DAY 
// BECAUSE YOU FUCKING ALZHEIMER'S IDIOT FORGOT IT HAD ALREADY BEEN ESTABLISHED INSIDE YOUR FUCKING STUPID LITTLE BRAIN !!!

//*****************************************************************************
/// @file
/// @brief
///   Arduino SmartThings Ethernet ESP8266 WiFi AC REMOTE CONTROL
///
/// 
///
///
//*****************************************************************************


#include <ArduinoOTA.h>

#include <SmartThingsESP8266WiFi_DUAL_SSID_3_HUBS.h>
//#include <SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB.h>
//#include <SmartThingsESP8266WiFi_DUAL_SSID.h>
//#include <SmartThingsESP8266WiFi.h>
//#include <SmartThingsESP8266_WIFI_SSID.h>

// call out function forward decalaration

SmartThingsCallout_t messageCallout;

#include <IRremoteESP8266.h>
//******************************************************************************************
/// Wifi (for ajax webpage)
//******************************************************************************************
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "index.h"  //Web page header file

#define DELAY_CMD 200
/// check samsung AC codes here:
