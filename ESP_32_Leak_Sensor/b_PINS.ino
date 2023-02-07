//******************************************************************************************
//Define which Arduino Pins will be used for each device
//******************************************************************************************
//"RESERVED" pins for ESP32 - best to avoid
#define PIN_0_RESERVED             0  //reserved ESP32 boot/program upload
#define PIN_1_RESERVED             1  //reserved ESP32 for TX0
#define PIN_3_RESERVED             3  //reserved ESP32 for RX0
#define PIN_6_RESERVED             6  //reserved ESP32 for flash
#define PIN_7_RESERVED             7  //reserved ESP32 for flash
#define PIN_8_RESERVED             8  //reserved ESP32 for flash
#define PIN_9_RESERVED             9  //reserved ESP32 for flash
#define PIN_10_RESERVED           10  //reserved ESP32 for flash
#define PIN_11_RESERVED           11  //reserved ESP32 for flash

//Analog Pins
#define PIN_WATER_1               A0  //(GPIO 36) SmartThings Capability "Water Sensor"
//#define PIN_WATER_2               A3  //(GPIO 39) SmartThings Capability "Water Sensor"
//#define PIN_ILLUMINANCE_1         A6  //(GPIO 34) SmartThings Capability "Illuminance Measurement"
//#define PIN_ILLUMINANCE_2         A7  //(GPIO 35) SmartThings Capability "Illuminance Measurement"
//#define PIN_VOLTAGE_1             A4  //(GPIO 32) SmartThings Capability "Voltage Measurement"
//#define PIN_SMOKE_1               A5  //(GPIO 33) SmartThings Capability "Smoke Detector" (MQ-2)
//
////Digital Pins
//#define PIN_TEMPERATUREHUMIDITY_1 25  //SmartThings Capabilities "Temperature Measurement" and "Relative Humidity Measurement"
//#define PIN_TEMPERATURE_2         26  //SmartThings Capabilty "Temperature Measurement" (Dallas Semiconductor DS18B20)
//
//#define PIN_MOTION_1              27  //SmartThings Capability "Motion Sensor"
//#define PIN_MOTION_2              14  //SmartThings Capability "Motion Sensor"
//#define PIN_CONTACT_1             12  //SmartThings Capability "Contact Sensor"
//#define PIN_CONTACT_2             13  //SmartThings Capability "Contact Sensor"
//#define PIN_SWITCH_1              23  //SmartThings Capability "Switch"
//#define PIN_SMOKE_2               22  //SmartThings Capability "Smoke Detector"
//#define PIN_ALARM_1               21  //SmartThings Capability "Alarm"
//#define PIN_ALARM_2               19  //SmartThings Capability "Alarm"
//#define PIN_STROBE_2              18  //SmartThings Capability "Alarm"              
////#define PIN_DOORCONTROL_CONTACT_1  5  //SmartThings Capabilty "Door Control"
//#define PIN_DOORCONTROL_RELAY_1   17  //SmartThings Capabilty "Door Control" 
//#define PIN_BUTTON_1              16  //SmartThings Capabilty Button / Holdable Button
//#define PIN_BUTTON_2               4  //SmartThings Capabilty Button / Holdable Button
//#define PIN_TIMEDRELAY_1           2  //SmartThings Capability "Relay Switch"
//#define PIN_CO_1                  15  //SmartThings Capability "Carbon Monoxide Detector"

#define LED 2
