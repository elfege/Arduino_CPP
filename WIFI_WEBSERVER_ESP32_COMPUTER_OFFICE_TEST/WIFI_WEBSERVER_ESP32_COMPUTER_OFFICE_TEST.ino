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

#include <SmartThingsESP8266WiFi.h>
#include <SmartThingsESP32WiFi.h>

//*****************************************************************************
// Pin Definitions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                    V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************

#define LED D4
#define ASensor A0 // power status sensor

#define switchPin D5 //Digital Output Pin - DanO

// call out function forward decalaration
SmartThingsCallout_t messageCallout;

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

WiFiServer server(80);

//Create a SmartThings Ethernet ESP8266WiFi object
st::SmartThingsESP8266WiFi smartthing(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);

bool ShutDown = false;
bool isDebugEnabled;    // enable or disable debug in this example
unsigned long previousMillis = 0; //needed for new timing mechanism - DanO
unsigned long MaxMillis = 4000000; // needed in order to not reach max millis()


//*****************************************************************************
// API Functions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void setup()
{

  // setup default state of global variables
  isDebugEnabled = true;

  if (isDebugEnabled)
  { // setup debug serial port
    Serial.begin(9600);         // setup serial with a baud rate of 9600
    Serial.println("");
    Serial.println("setup..");  // print out 'setup..' on start
  }

  // setup hardware pins

  pinMode(switchPin, OUTPUT);
  pinMode(LED, OUTPUT);  // define LED as an output

  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();
  //digitalWrite(LED, LOW); // turn on the LED


  Serial.println("");  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.println("");
  Serial.println("STARTING WEB SERVER");
  server.begin();


  Blink(10, 50);
}

//*****************************************************************************
void loop()
{
  
  webserver();
  smartthing.run();


  if (millis() - previousMillis > DELAY_SEC * 1000)
  {
    previousMillis = millis();

    if (!isOn() && !ShutDown) {
      Serial.println("reporting sleeping");
      smartthing.send("sleeping");
    }
    else if (!isOn() && ShutDown) {
      Serial.println("reporting shutdown");
      smartthing.send("shutdown");
    }
    else if (isOn())
    {
      Serial.println("reporting on");
      smartthing.send("running");
    }
    else {
      smartthing.send("ERROR");
    }
    previousMillis = millis();
    Blink(5, 50);
  }

//reset every 49 days to prevent crash due to previousMillis reaching ULONG limit
  if(previousMillis > MaxMillis){
    Reset();
  }

  yield();
}

//*****************************************************************************
// Local Functions  | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************

void messageCallout(String message)
{
  // if debug is enabled print out the received message
  if (isDebugEnabled)
  {
    Serial.print("Received message: '");
    Serial.print(message);
    Serial.println("' ");
  }

  // if message contents equals to 'on' then call on() function
  // else if message contents equals to 'off' then call off() function
  if (message.equals("switch on"))
  {
    on();
  }
  else if (message.equals("switch off"))
  {
    off();
  }
  else if (message.equals("switch sleep"))
  {
    Sleep();
  }
}

///// LOCAL FUNCTIONS ////
void on() {
ShutDown = false;
  if (!isOn()) {
    digitalWrite(switchPin, 1);
    delay(500);
    digitalWrite(switchPin, 0);


    Serial.println("");
    Serial.println("");
    Serial.println("COMPUTER TURNED ON");
  }
  else {
    Serial.println("ALREADY RUNNING");
  }
}

void off() {

  if (isOn()) {
    digitalWrite(switchPin, 1);
    while (isOn()) {
      yield();
    }
    delay(500);
    digitalWrite(switchPin, 0);

    Serial.println("");
    Serial.println("");
    Serial.println("COMPUTER TURNED OFF");
    smartthing.send("switch off");       // send message to cloud
  }
  else {
    Serial.println("ALREADY OFF");
  }
  ShutDown = true;
}


void Sleep() {
ShutDown = false;
  if (isOn()) {
    digitalWrite(switchPin, 1);
    delay(500);
    digitalWrite(switchPin, 0);

    Serial.println("");
    Serial.println("");
    Serial.println("COMPUTER PUT TO SLEEP");
    smartthing.send("switch sleep");       // send message to cloud
  }
  else {
    Serial.println("ALREADY OFF");
  }

}

boolean isOn() {
  bool result;
  if (analogRead(ASensor) >= 1020) {
    result = true;
  }
  else {
    result = false;
  }
  Serial.println(result);
  return result;
}

void Reset() {
  wdt_reset();
  pinMode(0, OUTPUT);
  delay(500);
  pinMode(2, OUTPUT);
  delay(500);
  pinMode(15, OUTPUT);
  delay(500);
  digitalWrite(0, HIGH);
  delay(500);
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(15, LOW);
  delay(500);
  ESP.restart();
}

void Blink(int times, int lapse) {
  int c = 0;
  while (c < times) {
    digitalWrite(LED, 1);
    delay(lapse);
    digitalWrite(LED, 0);
    delay(lapse);
    c++;
  }
}

