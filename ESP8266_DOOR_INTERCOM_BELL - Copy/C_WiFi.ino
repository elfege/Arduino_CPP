// NOTE: Create secrets.h from secrets.h.example with your WiFi credentials (SSID and password)
#include "secrets.h"

//******************************************************************************************
//ESP8266 WiFi Information    CHANGE THIS INFORMATION ACCORDINGLY FOR YOUR NETWORK!
//******************************************************************************************
String str_ssid     = "CM_2G";                           
String str_ssid2     = WIFI_SSID;                           
String str_password = WIFI_PASSWORD;                   

const char* ssidChar = "CM_2G";     //adjust to your own values
const char* ssidChar2 = WIFI_SSID;
const char *passwordChar = WIFI_PASSWORD;         

IPAddress iptoping (192, 168, 10, 2);   // The remote ip to ping
IPAddress ip(192, 168, 10, 228);       // Device IP Address      
IPAddress gateway(192, 168, 10, 1);    //router gateway          
IPAddress subnet(255, 255, 255, 0);   //LAN subnet mask         
IPAddress dnsserver(8, 8, 8, 8);  //DNS server              
IPAddress secondaryDNS(8, 8, 4, 4);
const unsigned int serverPort = 8090; // port to run the http server on


// Smart Home Hub Information
IPAddress hubIp(192, 168, 10, 69);    // Hubitat hub ip     
const unsigned int hubPort = 39501;   // Hubitat hub port
IPAddress hubIp2(192, 168, 10, 70);    // Hubitat hub ip     
const unsigned int hubPortb = 39501;   // Hubitat hub port
IPAddress hubIp3(192, 168, 10, 71);    // Hubitat hub ip     
const unsigned int hubPortc = 39501;   // Hubitat hub port


//Create a SmartThings Ethernet ESP8266WiFi object
//st::SmartThingsESP8266WiFi_DUAL_SSID smartthing(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);

// last used (Nov 2025)
// st::SmartThingsESP8266WiFi_DUAL_SSID_3_HUBS smartthing(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, hubIp2, hubPortb, hubIp3, hubPortc, messageCallout);

//st::SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB smartthing(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, hubIp2, hubPortb, messageCallout);
//st::SmartThingsESP8266WiFi smartthing(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);
//st::SmartThingsESP8266_WIFI_SSID smartthing(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);


ESP8266WebServer _server(80);


const IPAddress remote_ip(192, 168, 10, 1); // ip address to ping
