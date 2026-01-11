/* tried to read serial commands to no avail. Will not succeed this way for lack of
 *  understanding. 
 *  
 *  I will need to read status from LED's on the black and decker board, which should work just fine. 
 *  
 *  But, for that, we will need many GPIO's. ESP8266 won't cut it. 
 *  
 *  So I will use ESP32 instead. Tomorrow, after a good long night of sleep. 
 */

// #include <SmartThingsESP32WiFi_DUAL_SSID.h>
//#include <SmartThingsESP32_WIFI_SSID.h>
#include <SmartThingsESP32WiFi.h>

#include <Everything.h>      //Master Brain of ST_Anything library that ties everything together and performs ST Shield communications
#include <Executor.h>        //Generic Executor Class, typically receives data from ST Cloud (e.g. Switch)

//#include <NTPClient.h>
//#include <WiFiUdp.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "index.h"  //Web page header file

//******************************************************************************************
/// Arduino OTA 
//******************************************************************************************
#include <ArduinoOTA.h>


//******************************************************************************************
/// Wifi Terminal
//******************************************************************************************
#include "WiFiTerm.h"// OTA DEBUG LIB Wifi Terminal
