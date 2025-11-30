//******************************************************************************************
//ESP32 WiFi Information
//******************************************************************************************
const char * str_ssid     = "WIFI_SSID";  //  <---You must edit this line!
const char * str_ssid2     = "WIFI_SSID2";
const char * str_password = "WIFI_PASSWORDGahsh4479#$";                   //  <---You must edit this line!
IPAddress ip(192, 168, 10, 223);       //Device IP Address       //  <---You must edit this line!
IPAddress gateway(192, 168, 10, 1);    //Router gateway          //  <---You must edit this line!
IPAddress subnet(255, 255, 255, 0);   //LAN subnet mask         //  <---You must edit this line!
IPAddress dnsserver(192, 168, 10, 1);  //DNS server              //  <---You must edit this line!


WebServer server(80);
