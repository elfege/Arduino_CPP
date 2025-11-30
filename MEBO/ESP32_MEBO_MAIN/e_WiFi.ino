//******************************************************************************************
//ESP32 WiFi Information
//******************************************************************************************
String str_ssid     = "WIFI_SSID";  //  <---You must edit this line!
String str_ssid2     = "WIFI_SSID2"; 
const char* ssidChar = "WIFI_SSID";     //adjust to your own values
const char* ssidChar2 = "WIFI_SSID2";
const char* passwordChar = "WIFI_PASSWORDGahsh4479#$"; //adjust to your own values

String str_password = "WIFI_PASSWORDGahsh4479#$";                   //  <---You must edit this line!
IPAddress ip(192, 168, 10, 222);       //Device IP Address       //  <---You must edit this line!
IPAddress gateway(192, 168, 10, 1);    //Router gateway          //  <---You must edit this line!
IPAddress subnet(255, 255, 255, 0);   //LAN subnet mask         //  <---You must edit this line!
IPAddress dnsserver(8, 8, 8, 8);  //DNS server              //  <---You must edit this line!
IPAddress secondaryDNS(8, 8, 4, 4);
const unsigned int serverPort = 8090; // port to run the http server on

// Smartthings / Hubitat Hub TCP/IP Address
IPAddress hubIp(192, 168, 10, 145);    // smartthings/hubitat hub ip //  <---You must edit this line!


WebServer _server(80);

// SmartThings / Hubitat Hub TCP/IP Address: UNCOMMENT line that corresponds to your hub, COMMENT the other
const unsigned int hubPort = 39500;   // smartthings hub port
//const unsigned int hubPort = 39501;   // hubitat hub port
