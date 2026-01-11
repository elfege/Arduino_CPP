//*****************************************************************************
/// @file
/// @brief
///   Arduino SmartThings Ethernet ESP8266 WiFi On/Off with LED Example
///
///   Revised by Dan Ogorchock on 2017-02-11 to work with new "SmartThings v2.0" Library
///
///   Notes: The NodeMCU ESP communicates via WiFi to your home network router,
///          then to the ST Hub, and eventually to the ST cloud servers.
///
///
//*****************************************************************************



//*****************************************************************************
// Pin Definitions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                    V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************

#define LED LED_BUILTIN
#define ASensor A0 // power status sensor

#define switchPin 5 //Digital Output Pin - DanO

  
void callback(const String &msg)
{
  //Uncomment if it weould be desirable to using this function
  //Serial.print(F("ST_Anything_Miltiples Callback: Sniffed data = "));
  //Serial.println(msg);
  
  //TODO:  Add local logic here to take action when a device's value/state is changed
  
  //Masquerade as the ThingShield to send data to the Arduino, as if from the ST Cloud (uncomment and edit following line(s) as you see fit)
  //st::receiveSmartString("Put your command here!");  //use same strings that the Device Handler would send
}

//*****************************************************************************
// Global Variables   | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                    V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************

//Application Global Variables

const int DELAY_SEC = 5; //in seconds (to facilitate debugging!) - DanO

//******************************************************************************************
//ESP8266 WiFi Information    CHANGE THIS INFORMATION ACCORDINGLY FOR YOUR NETWORK!
//******************************************************************************************
String str_ssid     = "WIFI_SSID";                           //  <---You must edit this line!
String str_password = "WIFI_PASSWORDGahsh4479#$";                   //  <---You must edit this line!
IPAddress iptoping (192, 168, 10, 2);   // The remote ip to ping
IPAddress ip(192, 168, 10, 190);       // Device IP Address      //  <---You must edit this line!
IPAddress gateway(192, 168, 10, 1);    //router gateway          //  <---You must edit this line!
IPAddress subnet(255, 255, 255, 0);   //LAN subnet mask         //  <---You must edit this line!
IPAddress dnsserver(192, 168, 10, 1);  //DNS server              //  <---You must edit this line!
const unsigned int serverPort = 8090; // port to run the http server on

// Smartthings Hub Information
IPAddress hubIp(192, 168, 10, 145);    // smartthings hub ip     //  <---You must edit this line!
const unsigned int hubPort = 39500;   // smartthings hub port


//Create a SmartThings Ethernet ESP8266WiFi object
//st::SmartThingsESP8266WiFi smartthing(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);

bool ShutDown = false;
bool isDebugEnabled;    // enable or disable debug in this example
unsigned long previousMillis = 0; //needed for new timing mechanism - DanO
unsigned long MaxMillis = 4000000; // needed in order to not reach max millis()


