//******************************************************************************************
//ESP8266 WiFi Information    CHANGE THIS INFORMATION ACCORDINGLY FOR YOUR NETWORK!
//******************************************************************************************
String str_ssid     = "WIFI_SSID";                           //  <---You must edit this line!
String str_ssid2     = "WIFI_SSID2";
String str_password = "WIFI_PASSWORDGahsh4479#$";                   //  <---You must edit this line!


IPAddress iptoping (192, 168, 10, 2);   // The remote ip to ping
IPAddress ip(192, 168, 10, 190);       // Device IP Address      //  <---You must edit this line!
IPAddress gateway(192, 168, 10, 1);    //router gateway          //  <---You must edit this line!
IPAddress subnet(255, 255, 255, 0);   //LAN subnet mask         //  <---You must edit this line!
IPAddress dnsserver(192, 168, 10, 1);  //DNS server              //  <---You must edit this line!
const unsigned int serverPort = 8090; // port to run the ST library's http server on


// Smartthings Hub Information
IPAddress hubIp(192, 168, 10, 69);    // Hubitat hub ip     //  <---You must edit this line!
const unsigned int hubPort = 39501;   // Hubitat hub port
IPAddress hubIp2(192, 168, 10, 71);    // Hubitat hub ip        //  <---You must edit this line!
const unsigned int hubPortb = 39501;   // Hubitat hub ip     
IPAddress hubIp3(192, 168, 10, 70);    // Hubitat hub ip     //  <---You must edit this line!
const unsigned int hubPortc = 39501;   // Hubitat hub portport


//Create a SmartThings Ethernet ESP8266WiFi object
//st::SmartThingsESP8266WiFi_DUAL_SSID smartthing(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);
st::SmartThingsESP8266WiFi_DUAL_SSID_3_HUBS smartthing(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, hubIp2, hubPortb, hubIp3, hubPortc, messageCallout);
//st::SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB smartthing(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, hubIp2, hubPortb, messageCallout);
//st::SmartThingsESP8266WiFi smartthing(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);
//st::SmartThingsESP8266_WIFI_SSID smartthing(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);

ESP8266WebServer _server(80);
