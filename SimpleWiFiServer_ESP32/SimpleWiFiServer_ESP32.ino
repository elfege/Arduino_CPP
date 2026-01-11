/// libraries needed for OTA
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
//******************************************************************************************
// SmartThings Library for ESP32WiFi
//******************************************************************************************
#include <SmartThingsESP32WiFi.h>

//******************************************************************************************
// ST_Anything Library
//******************************************************************************************
#include <Constants.h>       //Constants.h is designed to be modified by the end user to adjust behavior of the ST_Anything library
#include <Device.h>          //Generic Device Class, inherited by Sensor and Executor classes
#include <Sensor.h>          //Generic Sensor Class, typically provides data to ST Cloud (e.g. Temperature, Motion, etc...)
#include <Executor.h>        //Generic Executor Class, typically receives data from ST Cloud (e.g. Switch)
#include <InterruptSensor.h> //Generic Interrupt "Sensor" Class, waits for change of state on digital input 
#include <PollingSensor.h>   //Generic Polling "Sensor" Class, polls Arduino pins periodically
#include <Everything.h>      //Master Brain of ST_Anything library that ties everything together and performs ST Shield communications

#include <PS_DS18B20_Temperature.h>  //Implements a Polling Sesnor (PS) to measure Temperature via DS18B20 libraries 
#include <PS_Illuminance.h>  //Implements a Polling Sensor (PS) to measure light levels via a photo resistor on an analog input pin 
#include <PS_Voltage.h>      //Implements a Polling Sensor (PS) to measure voltage on an analog input pin 
#include <PS_TemperatureHumidity.h>  //Implements a Polling Sensor (PS) to measure Temperature and Humidity via DHT library
#include <PS_Water.h>        //Implements a Polling Sensor (PS) to measure presence of water (i.e. leak detector) on an analog input pin 
#include <PS_MQ2_Smoke.h>    //Implements an Polling Sensor (PS) to monitor the status of an analog input pin from a MQ2 sensor
#include <IS_Motion.h>       //Implements an Interrupt Sensor (IS) to detect motion via a PIR sensor on a digital input pin
#include <IS_Contact.h>      //Implements an Interrupt Sensor (IS) to monitor the status of a digital input pin
#include <IS_Smoke.h>        //Implements an Interrupt Sensor (IS) to monitor the status of a digital input pin
#include <IS_CarbonMonoxide.h> //Implements an Interrupt Sensor (IS) to monitor the status of a digital input pin
#include <IS_DoorControl.h>  //Implements an Interrupt Sensor (IS) and Executor to monitor the status of a digital input pin and control a digital output pin
#include <IS_Button.h>       //Implements an Interrupt Sensor (IS) to monitor the status of a digital input pin for button presses
#include <EX_Switch.h>       //Implements an Executor (EX) via a digital output to a relay
#include <EX_Alarm.h>        //Implements Executor (EX)as an Alarm capability with Siren and Strobe via digital outputs to relays
#include <S_TimedRelay.h>    //Implements a Sensor to control a digital output pin with timing/cycle repeat capabilities
#include <EX_Switch_Dim.h>   //Implements an Executor (EX) for a switch (on/off) and pwm output (level) uses 2 digital output pins

void setup(){
  SETUP();
}

void loop(){
webserver();
}
