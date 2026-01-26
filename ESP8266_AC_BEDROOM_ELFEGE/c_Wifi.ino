// NOTE: Create secrets.h from secrets.h.example with your WiFi credentials (SSID and password)
#include "secrets.h"

//******************************************************************************************
//ESP8266 WiFi Information    CHANGE THIS INFORMATION ACCORDINGLY FOR YOUR NETWORK!
//******************************************************************************************
String str_ssid     = WIFI_SSID;                           // SSID - set in secrets.h
String str_ssid2     = WIFI_SSID2;                           // SSID2 - set in secrets.h
String str_password = WIFI_PASSWORD;                   // password - set in secrets.h
char ssid[] = WIFI_SSID;             // SSID - set in secrets.h
char pass[] = WIFI_PASSWORD;         // password - set in secrets.h
IPAddress ip(192, 168, 10, 211);       // Device IP Address      //  <---You must edit this line!
IPAddress gateway(192, 168, 10, 1);    //router gateway          //  <---You must edit this line!
IPAddress subnet(255, 255, 255, 0);   //LAN subnet mask         //  <---You must edit this line!
IPAddress dnsserver(8, 8, 8, 8);  //DNS server              //  <---You must edit this line!
IPAddress secondaryDNS(8, 8, 4, 4);
uint32_t serverPort = 8090; // port to run the http server on


ESP8266WebServer _server(80);


// Smartthings Hub Information
IPAddress hubIp(192, 168, 10, 69);    // Hubitat hub ip     //  <---You must edit this line!
const unsigned int hubPort = 39501;   // Hubitat hub port
IPAddress hubIp2(192, 168, 10, 70);    // Hubitat hub ip     //  <---You must edit this line!
const unsigned int hubPortb = 39501;   // Hubitat hub portport
IPAddress hubIp3(192, 168, 10, 71);    // Hubitat hub ip     //  <---You must edit this line!
const unsigned int hubPortc = 39501;   // Hubitat hub portport


//Create a SmartThings Ethernet ESP8266WiFi object
//st::SmartThingsESP8266WiFi_DUAL_SSID smartthing(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);
st::SmartThingsESP8266WiFi_DUAL_SSID_3_HUBS smartthing(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, hubIp2, hubPortb, hubIp3, hubPortc, messageCallout);
//st::SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB smartthing(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, hubIp2, hubPortb, messageCallout);
//st::SmartThingsESP8266WiFi smartthing(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);
//st::SmartThingsESP8266_WIFI_SSID smartthing(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);


// hubitat API (to get OpenWeather temperature value to control the unit's behaviour)
String API_KEY = "HUBITAT_API_KEY" ;
String API_IP = "192.168.10.72";
String API_APP_NUMBER = "35";
String API_DEVICE = "631"; // OpenWeather device on hub 4;

// http://192.168.10.72/apps/api/35/devices/523/events?access_token=HUBITAT_API_KEY
String API_PATH = "http://"+API_IP+"/apps/api/"+API_APP_NUMBER+"/devices/"+API_DEVICE+"?access_token="+API_KEY;