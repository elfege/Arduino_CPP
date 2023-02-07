
#include <ESP32Servo.h>
#include <math.h>
#include <driver/adc.h>

//******************************************************************************************
/// Wifi (for ajax webpage)
//******************************************************************************************
#include <ETH.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "index.h"  //Web page header file


//******************************************************************************************
/// OTA DEBUG LIB Wifi Terminal
//******************************************************************************************
#include "WiFiTerm.h"// OTA DEBUG LIB Wifi Terminal

//******************************************************************************************
// SmartThings Library for ESP32WiFi
//******************************************************************************************
//#include <SmartThingsESP32WiFi_DUAL_SSID.h>
//#include <SmartThingsESP32_WIFI_SSID.h>
//#include <SmartThingsESP32WiFi.h>

//#include <Everything.h>      //Master Brain of ST_Anything library that ties everything together and performs ST Shield communications
//#include <Executor.h>        //Generic Executor Class, typically receives data from ST Cloud (e.g. Switch)

//******************************************************************************************
/// Arduino OTA (redundent but necessary for custom name for now)
//******************************************************************************************
#include <ArduinoOTA.h>
