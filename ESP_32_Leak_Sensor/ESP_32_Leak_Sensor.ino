// MAC = B4E62D976861

//******************************************************************************************
//  File: ST_Anything_Multiples_ESP32WiFi.ino
//  Authors: Dan G Ogorchock & Daniel J Ogorchock (Father and Son)
//
//  Summary:  This Arduino Sketch, along with the ST_Anything library and the revised SmartThings
//            library, demonstrates the ability of one ESP32 to implement
//            a multi input/output custom device for integration into SmartThings.
//            The ST_Anything library takes care of all of the work to schedule device updates
//            as well as all communications with the ESP32's WiFi.
//
//            ST_Anything_Multiples implements the following ST Capabilities as a demo of what is possible with a single ESP32
//              - 2 x Water Sensor devices (using an analog input pin to measure voltage from a water detector board)
//              - 2 x Illuminance Measurement devices (using a photoresitor attached to ananlog input)
//              - 1 x Voltage Measurement devices (using a photoresitor attached to ananlog input)
//              - 1 x Door Control devices (used typically for Garage Doors - input pin (contact sensor) and output pin (relay switch)
//              - 2 x Contact Sensor devices (used to monitor magnetic door sensors)
//              - 1 x Switch devices (used to turn on a digital output (e.g. LED, relay, etc...)
//              - 1 x Smoke Detector device (using simple digital input)
//              - 1 x MQ-2 Smoke Detector devices (using simple analog input compared to user defined limit)
//              - 1 x Carbon Monoxide Detector device (using simple digital input)
//              - 2 x Motion devices (used to detect motion)
//              - 1 x Temperature Measurement device (Temperature from DHT22 device)
//              - 1 x Humidity Measurement device (Humidity from DHT22 device)
//              - 1 x Temperature Measurement device (Temperature from Dallas Semi 1-Wire DS18B20 device)
//              - 1 x Relay Switch device (used to turn on a digital output for a set number of cycles And On/Off times (e.g.relay, etc...))
//              - 2 x Button devices (sends "pushed" if held for less than 1 second, else sends "held"
//              - 2 x Alarm devices - 1 siren only, 1 siren and strobe (using simple digital outputs)
//
//
//  Change History:
//
//    Date        Who            What
//    ----        ---            ----
//    2017-08-14  Dan Ogorchock  Original Creation - Adapted from ESP8266 to work with ESP32 board
//    2018-02-09  Dan Ogorchock  Added support for Hubitat Elevation Hub
//
//   Special thanks to Joshua Spain for his contributions in porting ST_Anything to the ESP32!
//
//******************************************************************************************

//****************************************************************************************************************************
//NodeMCU-32s ESP32 Pin Definitions (just for reference from ..hardware\espressif\esp32\variants\nodemcu-32s\pins_arduino.h)
//****************************************************************************************************************************
//#define LED_BUILTIN 2
//#define BUILTIN_LED 2
//
//#define A0 = 36;
//#define A3 = 39;
//#define A4 = 32;
//#define A5 = 33;
//#define A6 = 34;
//#define A7 = 35;
//#define A10 = 4;
//#define A11 = 0;
//#define A12 = 2;
//#define A13 = 15;
//#define A14 = 13;
//#define A15 = 12;
//#define A16 = 14;
//#define A17 = 27;
//#define A18 = 25;
//#define A19 = 26;

