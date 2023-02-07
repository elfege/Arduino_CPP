/**************************** CONSTANTS *****************************/
// Setting PWM properties
const int freq = 30000;
const int pwmChannelL = 0;
const int pwmChannelR = 1;
const int resolution = 8;


/**************************** VARIABLES *****************************/
boolean stopAll = false;
boolean OTArequest = false;
boolean wifiLostAware = false;


unsigned long previousMillisBlink = millis();
unsigned long WiFiLostMillis = millis();
