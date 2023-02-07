#include <math.h>
#include <driver/adc.h>

// SERIAL ///
#include "HardwareSerial.h"
//HardwareSerial Serial2(2); // as of ESP32 1.0.1 the object Serial2 seems to be already created in the HardwareSerial.cpp file... 
#include "driver/adc.h"
#include "esp_adc_cal.h"

///
#include "Arduino.h"
#include <Streaming.h>      // Ref: http://arduiniana.org/libraries/streaming/ // not compatible under ESP32 hardware Serial
#include <IRremote.h>

//******************************************************************************************
/// Wifi (for ajax webpage)
//******************************************************************************************
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "index.h"  //Web page header file
#include "video_stream.h"


//#include <ESPAsyncWebServer.h>

#include "AiEsp32RotaryEncoder.h"





//******************************************************************************************
/// Arduino OTA (redundent but necessary for custom name for now)
//******************************************************************************************
#include <ArduinoOTA.h>


//******************************************************************************************
// SmartThings Library for ESP32WiFi
//******************************************************************************************
#include <SmartThingsESP32WiFi_DUAL_SSID.h>

#include <Everything.h>      //Master Brain of ST_Anything library that ties everything together and performs ST Shield communications
#include <Executor.h>        //Generic Executor Class, typically receives data from ST Cloud (e.g. Switch)
