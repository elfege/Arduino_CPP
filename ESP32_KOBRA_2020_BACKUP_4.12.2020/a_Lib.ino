#include <math.h>
#include <driver/adc.h>

// SERIAL ///
//#include "HardwareSerial.h"
//HardwareSerial Serial2(2); // as of ESP32 1.0.1 the object Serial2 seems to be already created in the HardwareSerial.cpp file... 
//#include "driver/adc.h"
//#include "esp_adc_cal.h"
//#include "SPIFFS.h"

///
#include <ESP32Servo.h>

#include "Arduino.h"
//#include <Streaming.h>      // Ref: http://arduiniana.org/libraries/streaming/ // not compatible under ESP32 hardware Serial
//#include <IRremote.h>


//******************************************************************************************
/// Wifi (for ajax webpage)
//******************************************************************************************
#include <ETH.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "index.h"  //Web page header file

//******************************************************************************************
/// Arduino OTA (redundent but necessary for custom name for now)
//******************************************************************************************
#include <ArduinoOTA.h>
