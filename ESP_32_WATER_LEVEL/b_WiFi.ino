// NOTE: Create secrets.h from secrets.h.example with your WiFi credentials (SSID and password)
#include "secrets.h"

//******************************************************************************************
//ESP32 WiFi Information
//******************************************************************************************
String str_ssid     = WIFI_SSID;                           // SSID - set in secrets.h
String str_ssid2     = WIFI_SSID;
const char* ssidChar = WIFI_SSID;     //adjust to your own values
const char* ssidChar2 = WIFI_SSID;
const char* passwordChar = WIFI_PASSWORD; //adjust to your own values

String str_password = WIFI_PASSWORD;                   // password - set in secrets.h
IPAddress ip(192, 168, 10, 224);       //Device IP Address       //  <---You must edit this line!
IPAddress gateway(192, 168, 10, 1);    //Router gateway          //  <---You must edit this line!
IPAddress subnet(255, 255, 255, 0);   //LAN subnet mask         //  <---You must edit this line!
IPAddress dnsserver(8, 8, 8, 8);  //DNS server              //  <---You must edit this line!
IPAddress secondaryDNS(8, 8, 4, 4);
const unsigned int serverPort = 8090; // port to run the http server on

// Hubitat Hub TCP/IP Address
IPAddress hubIp(192, 168, 10, 69);    // hubitat hub ip //  <---You must edit this line!
const unsigned int hubPort = 39501;   // hubitat hub port


WebServer _server(80);
