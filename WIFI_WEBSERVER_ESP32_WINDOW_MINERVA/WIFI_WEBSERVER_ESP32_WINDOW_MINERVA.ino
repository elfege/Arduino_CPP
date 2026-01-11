//*****************************************************************************
/// @file
/// @brief
///   Arduino SmartThings Ethernet ESP8266 WiFi Humidifier control
///
///   Revised by Dan Ogorchock on 2017-02-11 to work with new "SmartThings v2.0" Library
///
///   Notes: The NodeMCU ESP communicates via WiFi to your home network router,
///          then to the ST Hub, and eventually to the ST cloud servers.
///
///
//*****************************************************************************


#include <SmartThingsESP32WiFi.h>

#include <Everything.h>      //Master Brain of ST_Anything library that ties everything together and performs ST Shield communications

#include <Executor.h>        //Generic Executor Class, typically receives data from ST Cloud (e.g. Switch)



//******************************************************************************************
/// Arduino OTA 
//******************************************************************************************
#include <ArduinoOTA.h>




