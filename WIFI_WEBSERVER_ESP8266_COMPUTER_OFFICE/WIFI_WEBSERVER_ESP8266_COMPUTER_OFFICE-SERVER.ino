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
// ping library
#include <ESP8266Ping.h>
#include <SmartThingsESP8266WiFi.h>

//*****************************************************************************
// Pin Definitions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                    V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************

#define LED D0
#define ASensor A0 // power status sensor

#define switchPin D5 //Digital Output Pin - DanO
#define RESET_PIN D2 // pin to connect to RST // ADD 1K resistor between RST and D2 pins to prevent loss of flash capability 
// call out function forward decalaration
SmartThingsCallout_t messageCallout;



//*****************************************************************************
// Global Variables   | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                    V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************


//Application Global Variables

const int DELAY_SEC = 120; //in seconds (to facilitate debugging!) - DanO
const int OTHER_DELAY = 42000; // 12 hours
//******************************************************************************************
//ESP8266 WiFi Information    CHANGE THIS INFORMATION ACCORDINGLY FOR YOUR NETWORK!
//******************************************************************************************
String str_ssid     = "WIFI_SSID2";                           //  <---You must edit this line!
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
bool isDebugEnabled = true;    // enable or disable debug 
unsigned long previousMillis = 0; //needed for new timing mechanism - DanO
unsigned long previousMillis_IGNORE_RESET = 0;

boolean ignorecommand = false;

//*****************************************************************************
// API Functions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void setup()
{
  // create OTA name
  const char *NameOTA = "COMPUTER_OFFICE";
  ArduinoOTA.setHostname(NameOTA);

  
    Serial.begin(115000);         // setup serial with a baud rate of 9600
    Serial.println("");
    Serial.println("setup..");  // print out 'setup..' on start
 

  // setup hardware pins

  pinMode(switchPin, OUTPUT);
  pinMode(LED, OUTPUT);  // define LED as an output
  pinMode(RESET_PIN, OUTPUT); //pulled down by digitalWrite to RST board
  digitalWrite(RESET_PIN, HIGH); //  HIGH by default 

  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();
  //digitalWrite(LED, LOW); // turn on the LED

  Serial.println("");  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.println("");
  Serial.println("STARTING WEB SERVER");
  server.begin();

  // update smartthings hub
  Refresh();
  Blink(10, 50);
}

//*****************************************************************************
void loop()
{
  webserver();
  smartthing.run();
  // refresh statuses every x minutes
  if (millis() - previousMillis > DELAY_SEC * 1000)
  {
    previousMillis = millis();
    Refresh();
    ignorecommand = false; // reset this value
  }
  if (millis() - previousMillis_IGNORE_RESET > OTHER_DELAY * 1000)
  {
    previousMillis_IGNORE_RESET = millis();
    ignorecommand = false; // reset this value
  }

  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("");
    Serial.print("LOST CONNECTION... RESETING ESP");
    delay(1000);
    Reset();
  }
  
 
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
  if (message.equals("switch ignorecommand")) {
    ignorecommand = true;
  }
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
  else if (message.equals("switch refresh"))
  {
    Refresh();
  }
}

///// LOCAL FUNCTIONS ////
void on() {
  ShutDown = false;
  smartthing.send("turningon"); // inform ST HUB of the attempt
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

  delay(3000); // leave enough time for the device handler to change the state to "turningon"
  if (isOn()) {
    smartthing.send("running"); // tell device handler that computer is now on and running
  }
  else {
    smartthing.send("error"); // tell hub that the computer failed to turnon
  }
}

void off() {
  smartthing.send("turningoff"); // inform ST HUB of the attempt
  if (!ignorecommand) {
    if (isOn()) {
      digitalWrite(switchPin, 1);
      // turning off computer needs 4 seconds
      // so set a 4+1 seconds timer for the while loop
      int DELAY = 5 * 1000;
      int c = 0;
      delay(1);
      while (isOn() && millis() < DELAY) {
        delay(1000);
        c++;
      }
      //    delay(500);
      digitalWrite(switchPin, 0);

      Serial.println("");
      Serial.println("");
      Serial.println("COMPUTER TURNED OFF");
    }
    else {
      Serial.println("ALREADY OFF");
    }

    if (!isOn()) {
      smartthing.send("shutdown"); // tell device handler that computer is now turned off
      ShutDown = true;
    }
    else {
      //smartthing.send("shutdown"); // FOR TEST ONLY
      smartthing.send("error"); // tell hub that the computer failed to turnoff
      ShutDown = false;
    }
  }
  else {
    smartthing.send("commandignored");
  }
}

void Sleep() {
  ShutDown = false;
  smartthing.send("tosleep"); // inform ST HUB of the attempt
  if (!ignorecommand) {
    if (isOn()) {
      digitalWrite(switchPin, 1);
      delay(500);
      digitalWrite(switchPin, 0);

      // Putting a computer to sleep, depending on its running processes
      // can take some time
      // so set a 60 seconds timer for the while loop

      int DELAY = 60;
      int c = 0;
      while (isOn() && c < DELAY) {
        /// 5 SECONDS FOR TEST ONLY, normal time should be at least 30 seconds.
        delay(1000);
        c++;
      }
      if (!isOn()) {

        Serial.println("");
        Serial.println("");
        Serial.println("COMPUTER PUT TO SLEEP");
        smartthing.send("sleeping"); // tell device handler that computer is now in sleep mode
      }
      else {
        // try again
        digitalWrite(switchPin, 1);
        delay(500);
        digitalWrite(switchPin, 0);
        c = 0;
        while (isOn() && c < DELAY) {
          delay(1000);
          c++;
        }
        if (!isOn()) {
          // smartthing.send("sleeping"); // for tests purpose, delete this afterward and uncomment the line below!
          smartthing.send("error"); // tell the hub that the computer failed to go into sleep mode
        }
      }
    }
    else {
      Serial.println("ALREADY OFF");
    }
  }
  smartthing.send("commandignored");
}

void Refresh() {
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
/////////////////// BOOLEANS /////////////////////////
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
  smartthing.send("reset");
  Serial.println("RESETING ESP8266");
 
  digitalWrite(RESET_PIN, LOW);

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

