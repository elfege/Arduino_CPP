#include "driver/adc.h"
#include "esp_adc_cal.h"
#include <math.h>
#include "Arduino.h"
#include <Streaming.h>      // Ref: http://arduiniana.org/libraries/streaming/
/// 
#include <WiFi.h>
#include <IRremote.h>


//******************************************************************************************
/// Arduino OTA 
//******************************************************************************************
#include <ArduinoOTA.h>


//******************************************************************************************
// SmartThings Library for ESP32WiFi
//******************************************************************************************
#include <SmartThingsESP32WiFi.h>

#include <Everything.h>      //Master Brain of ST_Anything library that ties everything together and performs ST Shield communications

#include <Executor.h>        //Generic Executor Class, typically receives data from ST Cloud (e.g. Switch)
