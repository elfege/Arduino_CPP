// NOTE: Create secrets.h from secrets.h.example with your WiFi credentials (SSID and password)
#include "secrets.h"

//******************************************************************************************
//ESP8266 WiFi Information    CHANGE THIS INFORMATION ACCORDINGLY FOR YOUR NETWORK!
//******************************************************************************************
String str_ssid     = WIFI_SSID;                           // SSID - set in secrets.h
String str_ssid2     = WIFI_SSID2;  
String str_password = PASSWORD;                   // password - set in secrets.h
IPAddress iptoping (192, 168, 10, 1);   // The remote ip to ping
IPAddress ip(192, 168, 10, 209);       // Device IP Address      //  <---You must edit this line!
IPAddress gateway(192, 168, 10, 1);    //router gateway          //  <---You must edit this line!
IPAddress subnet(255, 255, 255, 0);   //LAN subnet mask         //  <---You must edit this line!
IPAddress dnsserver(192, 168, 10, 1);  //DNS server              //  <---You must edit this line!
const uint32_t serverPort = 8090; // port to run the http server on

// Smartthings Hub Information
//IPAddress hubIp(192, 168, 10, 145);    // smartthings hub ip     //  <---You must edit this line!
//const uint32_t hubPort = 39500;   // smartthings hub port
IPAddress hubIp(192, 168, 10, 69);    // hubitat hub ip     //  <---You must edit this line!
const uint32_t hubPort = 39501;   // hubitat hub port


// Define NTP Client to get time
//WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP);

WebServer _server(80);
