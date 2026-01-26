// NOTE: Create secrets.h from secrets.h.example with your WiFi credentials (SSID and password)
#include "secrets.h"

//******************************************************************************************
//ESP8266 WiFi Information    CHANGE THIS INFORMATION ACCORDINGLY FOR YOUR NETWORK!
//******************************************************************************************
String str_ssid     = WIFI_SSID;                           // SSID - set in secrets.h
String str_ssid2     = WIFI_SSID2;
String str_password = WIFI_PASSWORD;                   // password - set in secrets.h


IPAddress iptoping (192, 168, 10, 2);   // The remote ip to ping
IPAddress ip(192, 168, 10, 223);       // Device IP Address      //  <---You must edit this line!
IPAddress gateway(192, 168, 10, 1);    //router gateway          //  <---You must edit this line!
IPAddress subnet(255, 255, 255, 0);   //LAN subnet mask         //  <---You must edit this line!
IPAddress dnsserver(192, 168, 10, 1);  //DNS server              //  <---You must edit this line!
const unsigned int serverPort = 8090; // port to run the ST library's http server on

// Smartthings Hub Information
IPAddress hubIp(192, 168, 10, 69);    // smartthings hub ip     //  <---You must edit this line!
const unsigned int hubPort = 39500;   // smartthings hub port


WiFiServer server(80);
