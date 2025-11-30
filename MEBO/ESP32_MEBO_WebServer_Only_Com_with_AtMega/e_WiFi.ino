const char* ssid     = "WIFI_SSID2";
const char* password = "WIFI_PASSWORDGahsh4479#$";

IPAddress ip(192, 168, 10, 222);       //Device IP Address       //  <---You must edit this line!
IPAddress gateway(192, 168, 10, 1);    //Router gateway          //  <---You must edit this line!
IPAddress subnet(255, 255, 255, 0);   //LAN subnet mask         //  <---You must edit this line!
IPAddress dnsserver(192, 168, 10, 1);  //DNS server              //  <---You must edit this line!

WiFiServer server(80); // different port to avoid conflict with OTA 


