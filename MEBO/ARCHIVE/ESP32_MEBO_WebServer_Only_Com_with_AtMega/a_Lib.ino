
#include "HardwareSerial.h"
HardwareSerial Serial2(2);


#include "driver/adc.h"
#include "esp_adc_cal.h"
#include <math.h>
#include "Arduino.h"
#include <Streaming.h>      // Ref: http://arduiniana.org/libraries/streaming/ // not compatible under ESP32 hardware Serial
/// 
#include <WiFi.h>
#include <IRremote.h>



//******************************************************************************************
/// Arduino OTA (redundent but necessary for custom name for now)
//******************************************************************************************
#include <ArduinoOTA.h>


