const char *NameOTA = "INTERCOM";

//*************************************************************************************************
//NodeMCU v1.0 ESP8266-12e Pin Definitions (makes it much easier as these match the board markings)
//*************************************************************************************************
//#define LED_BUILTIN 16
//#define BUILTIN_LED 16
//
//#define D0 16  //no internal pullup resistor
//#define D1  5
//#define D2  4  builtin led
//#define D3  0  //must not be pulled low during power on/reset, toggles value during boot
//#define D4  2  //must not be pulled low during power on/reset, toggles value during boot
//#define D5 14
//#define D6 12
//#define D7 13
//#define D8 15  //must not be pulled high during power on/reset
//*****************************************************************************
/// @file
/// @brief
///   Arduino SmartThings Ethernet ESP8266 WiFi Window Control August 2019
///
///
//*****************************************************************************


#define TALK  D5 // TALK PUSH BUTTON
#define DOOR  D7  // DOOR PUSH BUTTON
#define LED   D4 //
#define sensorPin A0 // VOTAGE SENSOR that detects intercom LCD screen's activation 

const int PRESS = 0;
const int RELEASE = 1;

int sensorValue = 0;  // variable to store the value read by the sensor

int Value = HIGH;
int threshold= 500;
int lastloop = 5; // we don't want last loop to run the first time this value is reset once powerOn was triggered



int frequency = 60000; //

bool wifiLostAware = false;
bool StopAll = false;
bool debug = false;
bool refreshrequest = false;
bool cmdFromHttp = false;
bool runsim = false;
bool switch_door_operation_is_running = false;
bool got_signal = false;
bool sent = false;


String lastMessage;

unsigned long lastMillisBlink = millis();
unsigned long lastRefresh = millis();
unsigned long WiFiLostMillis = millis();
unsigned long lastSignalDebug = millis();
unsigned long cmdFromHttpMillis = millis();
unsigned long lastRead = millis();