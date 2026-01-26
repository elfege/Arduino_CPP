#include <Arduino.h>
#line 1 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/c_setup.ino"
void setup();
#line 1 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/d_loop.ino"
void loop();
#line 19 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/d_loop.ino"
void master();
#line 60 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/d_loop.ino"
void servers();
#line 1 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
void on();
#line 22 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
void off();
#line 61 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
void update_state();
#line 76 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
void getPowerState();
#line 84 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
void getPowerStateOnly();
#line 91 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
void toggle_power();
#line 111 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
void hard_reboot();
#line 121 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
void shutDown();
#line 128 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
void refresh();
#line 158 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
boolean is_on();
#line 173 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
void send_data(String var);
#line 187 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
void getMac();
#line 194 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
void log();
#line 203 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
String TimeInfos();
#line 263 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
void glow(int DL);
#line 291 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
void Blink(int times, int lapse);
#line 304 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
void Reset();
#line 2 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/f_callOut.ino"
void messageCallout(String message);
#line 3 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/h_http_routes.ino"
void initXMLhttp();
#line 0 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/c_setup.ino"
#line 1 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/ESP8266_COMPUTER_OFFICE.ino"
//*****************************************************************************
/// @file
/// @brief
///   Arduino SmartThings Ethernet ESP8266 WiFi 
///
///   Revised by Dan Ogorchock on 2017-02-11 to work with new "SmartThings v2.0" Library
///
///   Notes: The NodeMCU ESP communicates via WiFi to your home network router,
///          then to the ST Hub, and eventually to the ST cloud servers.
///
///
//*****************************************************************************

#line 1 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/a_Libraries.ino"


#include <SmartThingsESP8266WiFi_DUAL_SSID_3_HUBS.h>
//#include <SmartThingsESP8266WiFi_DUAL_SSID_DUAL_HUB.h>
//#include <SmartThingsESP8266WiFi_DUAL_SSID.h>
//#include <SmartThingsESP8266WiFi.h>
//#include <SmartThingsESP8266_WIFI_SSID.h>

// call out function forward decalaration
SmartThingsCallout_t messageCallout;

#include <ESP8266Ping.h>

#include "secrets.h"

#include "Arduino.h"
#include <Streaming.h>      // Ref: http://arduiniana.org/libraries/streaming/ // not compatible under ESP32 hardware Serial


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "index.h"  //Web page header file


//******************************************************************************************
/// Arduino OTA (redundent but necessary for custom name for now)
//******************************************************************************************
#include <ArduinoOTA.h>


#include "WiFiTerm.h"// OTA DEBUG LIB Wifi Terminal
#line 1 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/a_aVariables.ino"
boolean goUp;
boolean StopAll = false;
boolean logs = false;
boolean isDebugEnabled = true;    // enable or disable debug
boolean userOverride = false;
boolean allowshutDownWhenFail; // value to be updated by the device driver on the hub end or at next refresh
boolean noSleep = false;
boolean STOP = false; // required when "off" command sent more than once within while() time
boolean OnOffRunning = false;
boolean new_state = false;


int glw;
int elapsed = 0;

unsigned long OTHER_DELAY = 1000 * 60 * 60 * 10; // 10 hours
unsigned long DELAY_SEC = 600; // 10 minutes refresh
unsigned long DELAY_SLEEP = 120000;
unsigned long DELAY_FAILSAFE = 600000;
unsigned long DELAY_NEWATTEMPT = 60000;
unsigned long lastStateRefreshMillis = millis();
unsigned long last_message_sent_millis = millis();

unsigned long previousMillisBlink = 0;
unsigned long lastOnMillis = 0;
unsigned long offTimer = 30; // in minutes
unsigned long previousMillis = 0; //needed for new timing mechanism - DanO
unsigned long previousMillis_override_RESET = 0;
unsigned long bootTime = millis();
unsigned long millisHeartBit = millis();


String var = "";
String last_state = "off";
String last_state_sent = last_state;

#line 1 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/aa_PINS.ino"
#define LED D4 // built-in LED
#define ASensor A0 // power status sensor

#define switchPin D5 //Digital Output Pin - DanO
#define RESET_PIN D2 // pin to connect to RST // ADD 1K resistor between RST and D2 pins to prevent loss of flash capability 

#line 1 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/b_WiFi.ino"
//******************************************************************************************
//ESP8266 WiFi Information    CHANGE THIS INFORMATION ACCORDINGLY FOR YOUR NETWORK!
//******************************************************************************************
String str_ssid     = WIFI_SSID2;                           //  <---You must edit this line!
String str_ssid2     = WIFI_SSID;
String str_password = PASSWORD;                   //  <---You must edit this line!


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

#line 1 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/c_setup.ino"
void setup()
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
  // setup debug serial port
  Serial.begin(115200); // setup serial with a baud rate of 115200
  term.link(Serial);    // optional : echo every print() on Serial

  //*****************************************************************************
  // PINS CONFIGURATION
  //*****************************************************************************
  pinMode(switchPin, OUTPUT);
  pinMode(LED, OUTPUT);          // define LED as an output
  pinMode(RESET_PIN, OUTPUT);    // pulled down by digitalWrite to RST board
  digitalWrite(RESET_PIN, HIGH); //  HIGH by default

  // create OTA name before smartthings does
  const char *NameOTA = "COMPUTER_OFFICE";
  ArduinoOTA.setHostname(NameOTA);

  smartthing.init();

  initXMLhttp();
  _server.begin();

  //******************************************************************************************
  // WiFi Terminal
  //******************************************************************************************
  term.begin(_server);
  term.println("WiFi Terminal ok!");

  ArduinoOTA.onStart([]()
                     {
    term.println("Start");
    StopAll = true; });
  ArduinoOTA.onEnd([]()
                   {
    term.println("\nEnd");
    Blink(2, 1000);
    delay(2000);
    Blink(5, 100); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        {
    term.printf("Progress: %u%%\r\n", (progress / (total / 100)));
    term.handleClient();
    digitalWrite(LED, !digitalRead(LED)); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
    term.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) term.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) term.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) term.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) term.println("Receive Failed");
    else if (error == OTA_END_ERROR) term.println("End Failed"); });
  ArduinoOTA.begin();
  term.println("ArduinoOTA Ready");

  var = "SWITCH REBOOT";
  send_data(var);
  refresh();
  bootTime = millis();
}

#line 1 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/d_loop.ino"
void loop()
{
  ArduinoOTA.handle();
  if (!StopAll)
  {
    master();

    if (millis() - millisHeartBit >= 1000)
    {
      //    term.println("state = "+last_state);
      millisHeartBit = millis();
      digitalWrite(LED, !digitalRead(LED));
    }
  }
  yield();
  delay(10);
}

void master()
{
  servers();

  if (!is_on() && last_state == "on")
  {
    term.println("UPDATE 1");
    last_state = "off";
    send_data("switch off");
    send_data("computer sleeping");
    send_data("status sleeping");
  }
  if (is_on() && last_state == "off")
  {
    term.println("UPDATE 2");
    last_state = "on";
    send_data("switch on");
    send_data("computer running");
    send_data("status running");
  }

  // refresh statuses every x minutes
  if (millis() - previousMillis > DELAY_SEC * 1000)
  {
    previousMillis = millis();
    refresh();
    userOverride = false; // reset this value
    String var = "Override Inactive";
    _server.send(200, "text/html", var);
    send_data(var);
  }
  if (millis() - previousMillis_override_RESET > OTHER_DELAY * 1000)
  {
    previousMillis_override_RESET = millis();
    userOverride = false; // reset this value
    String var = "Override Inactive";
    _server.send(200, "text/html", var);
    send_data(var);
  }
}

void servers()
{
  _server.handleClient();
  smartthing.run();
  term.handleClient();
}

#line 1 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/e_functions.ino"
void on()
{
  OnOffRunning = true;
  if (!is_on())
  {
    digitalWrite(switchPin, 1);
    delay(500);
    digitalWrite(switchPin, 0);
    term.println("");
    term.println("");
    term.println("COMPUTER TURNED ON");
  }
  else
  {
    term.println("ALREADY RUNNING");
  }
  update_state();

  OnOffRunning = false;
}

void off()
{
  // NB: NEVER USE A RETURN STATEMENT IN THIS METHOD
  OnOffRunning = true;
  if (!noSleep)
  {
    term.println("Putting computer to sleep");

    if (is_on())
    {
      digitalWrite(switchPin, 1);
      delay(500);
      digitalWrite(switchPin, 0);
      unsigned long s = millis();
      while (millis() - s < 70000 && !STOP && is_on())
      {
        master();
      }
      if (millis() - s > 70000 && !STOP && is_on() && !userOverride)
      {
        // failed so try again
        digitalWrite(switchPin, 1);
        delay(500);
        digitalWrite(switchPin, 0);
      }
      STOP = false; // reset this boolean after using it
    }
    else
    {
      term.println("Computer already off or sleeping");
    }
  }
  else
  {
    term.println("SLEEP OVERRIDE - noSleep BOOL TRUE");
  }
  update_state();
  OnOffRunning = false;
}
void update_state()
{
  if (is_on())
  {
    last_state = "on";
    send_data("switch on");
    send_data("computer running");
  }
  else
  {
    last_state = "off";
    send_data("switch off");
    send_data("computer sleeping");
  }
}
void getPowerState()
{
  toggle_power();
  String state = is_on() ? "computer running" : "computer sleeping";
  _server.send(200, "text/plain", state);
  send_data(state);
}

void getPowerStateOnly()
{
  String state = is_on() ? "computer running" : "computer sleeping";
  _server.send(200, "text/plain", state);
  send_data(state);
}

void toggle_power()
{
  if (OnOffRunning)
  {
    term.println("Operation already in progress - ignoring");
    return; // Prevent multiple triggers
  }
  OnOffRunning = true; // Set immediately to prevent race condition
  if (is_on())
  {
    STOP = true; // in case while() loop is running
    off();
  }
  else
  {
    STOP = true; // in case while() loop is running
    on();
  }
}

void hard_reboot()
{
  digitalWrite(switchPin, 1);
  delay(5000);
  digitalWrite(switchPin, 0);
  delay(5000);
  digitalWrite(switchPin, 1);
  delay(500);
  digitalWrite(switchPin, 0);
}
void shutDown()
{
  digitalWrite(switchPin, 1);
  delay(5000);
  digitalWrite(switchPin, 0);
}

void refresh()
{
  term.println("REFRESHING");

  last_state = is_on() ? "on" : "off";

  if (!is_on())
  {
    term.println("reporting 'computer sleeping'");
    send_data("switch off");
    send_data("computer sleeping");
    send_data("status sleeping");
  }
  else
  {
    term.println("reporting 'computer on'");
    send_data("switch on");
    send_data("computer running");
    send_data("status running");
  }

  String var = "";
  var = allowshutDownWhenFail ? "SHUTDOWN ALLOWED" : "SHUTDOWN FORBIDDEN";
  send_data(var);
  var = userOverride ? "Override Active" : "Override Inactive";
  //  _server.send(200, "text/html", var); // has to be called from js
  send_data(var);
  noSleep = false; // only place where this value is reset
}
/////////////////// BOOLEANS /////////////////////////
boolean is_on()
{
  bool result;
  if (analogRead(ASensor) >= 1020)
  {
    result = true;
  }
  else
  {
    result = false;
  }
  // term.println(result);
  return result;
}

void send_data(String var)
{

  new_state = last_state == "off" && var == "on" || last_state == "on" && var == "off";

  // refresh state every minute or if it changed
  if (millis() - last_message_sent_millis >= 60000 || new_state)
  {
    term.println("sending " + var + " to the hub");
    smartthing.send(var); // send the current value to smartthings
    last_message_sent_millis = millis();
  }
}

void getMac()
{
  String ip = WiFi.localIP().toString();
  var = "Mac: " + WiFi.macAddress() + " IP:" + ip + " " + String(WiFi.SSID());
  _server.send(200, "text/html", var);
}

void log()
{

  String debug1 = "";
  String debug2 = "";
  send_data(debug1);
  send_data(debug2);
}

String TimeInfos()
{
  unsigned long totalTimeSeconds = millis() / 1000;
  unsigned long totalTimeMinutes = millis() / 1000 / 60;
  unsigned long TotalTimeHours = millis() / 1000 / 60 / 60;
  unsigned long TotalTimeDays = millis() / 1000 / 60 / 60 / 24;

  unsigned long displaySeconds = (millis() - (totalTimeMinutes * 60 * 1000)) / 1000;
  unsigned long displayMinutes = (millis() - (TotalTimeHours * 1000 * 60 * 60)) / 1000 / 60;
  unsigned long displayHours = (millis() - (TotalTimeDays * 1000 * 60 * 60 * 24)) / 1000 / 60 / 60;
  unsigned long dispalyDays = TotalTimeDays;

  term.println("LOOP time = " + String(elapsed) + "ms");
  String result = "Time since last boot: ";

  if (dispalyDays == 1)
  {
    result = result + dispalyDays + " day";
    term.println(result);
  }
  else if (dispalyDays > 1)
  {
    result = result + dispalyDays + " days";
    term.println(result);
  }
  if (displayHours == 1)
  {
    result = result + displayHours + " hour ";
    term.println(result);
  }
  else if (displayHours > 1)
  {
    result = result + displayHours + " hours ";
    term.println(result);
  }
  if (displayMinutes == 1)
  {
    result = result + displayMinutes + " minute ";
    term.println(result);
  }
  else if (displayMinutes > 1)
  {
    result = result + displayMinutes + " minutes ";
    term.println(result);
  }
  if (displaySeconds == 1)
  {
    result = result + displaySeconds + " second ";
    term.println(result);
  }
  else if (displaySeconds > 1)
  {
    result = result + displaySeconds + " seconds ";
    term.println(result);
  }

  _server.send(200, "text/html", result);
  return result;
}

void glow(int DL)
{
  int s = 1024;

  if (glw >= s)
  {
    goUp = false;
  }
  else if (glw < 1)
  {
    goUp = true;
  }

  if (goUp && glw < s)
  {
    glw++;
    analogWrite(LED, glw);
    // term.print("GLOW "); term.println(i);
  }
  else if (!goUp && glw > 0)
  {
    glw--;
    analogWrite(LED, glw);
    // term.print("GLOW "); term.println(i);
  }
  delay(DL);
}

void Blink(int times, int lapse)
{
  int c = 0;
  while (c < times)
  {
    digitalWrite(LED, 1);
    delay(lapse);
    digitalWrite(LED, 0);
    delay(lapse);
    c++;
  }
}

void Reset()
{
  ESP.reset();
}

#line 1 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/f_callOut.ino"

void messageCallout(String message)
{
  // if debug is enabled print out the received message
  if (isDebugEnabled)
  {
    term.print("Received message: '");
    term.print(message);
    term.println("' ");
  }

  // if message contents equals to 'on' then call on() function
  // else if message contents equals to 'off' then call off() function
  if (message.equals("override")) {
    term.println("OVERRIDE");
    userOverride = !userOverride;
    String var = userOverride ? "Override Active" : "Override Inactive";
    _server.send(200, "text/html", var);
    send_data(var);
  }
  if (message.equals("on"))
  {
    STOP = true; // in case while() loop is running 
    unsigned long s = millis();
    while(OnOffRunning && millis() - s < 5000)
    {
      master(); // waiting for OnOffRunning to return false
    }
    if(OnOffRunning) // if still true means STOP didn't break the on() or off() inner loops within timeframe
    {
      term.println("TIMEOUT RESET");
      Reset();
    }
    on();
  }
  else if (message.equals("off"))
  {
    STOP = true; // in case while() loop is running 
    off();
  }
  else if (message.equals("sleep"))
  {
    STOP = true; // in case while() loop is running 
    off();
  }
  else if (message.equals("ALLOWSHUTDOWN"))
  {
    allowshutDownWhenFail = true;
    String var = "SHUTDOWN ALLOWED";
    send_data(var);
  }
  else if (message.equals("FORBIDSHUTDOWN"))
  {
    allowshutDownWhenFail = false;
    String var = "SHUTDOWN FORBIDDEN";
    send_data(var);
  }
  
  else if (message.equals("refresh"))
  {
    refresh();
  }
}

#line 1 "/home/elfege/0_ARDUINO/ESP8266_COMPUTER_OFFICE/h_http_routes.ino"


void initXMLhttp() {
  _server.on("/", []() {
    _server.send_P(200, "text/html", MAIN_page);
  });

  _server.on("/power", getPowerState);              // toggles
  _server.on("/getpowerstate", getPowerStateOnly);  // doesn't toggle, just refreshes
  _server.on("/togglepower", []() {
    toggle_power();
    String state = is_on() ? "computer running" : "computer sleeping";
    _server.send(200, "text/plain", state);
  });
  _server.on("/on", on);
  _server.on("/off", off);
  _server.on("/hard_reboot", hard_reboot);
  _server.on("/shutDown", shutDown);
  _server.on("/reset", Reset);
  _server.on("/getMac", getMac);
  // _server.on("/debug", getDataDebug);
  _server.on("/getTimeInfo", TimeInfos);

  _server.on("/override", []() {
    term.println("OVERRIDE");
    userOverride = !userOverride;
    String var = userOverride ? "Override Active" : "Override Inactive";
    _server.send(200, "text/html", var);
    send_data(var);
  });
  _server.on("/overrideCheck", []() {
    String var = userOverride ? "Override Active" : "Override Inactive";
    _server.send(200, "text/html", var);
    send_data(var);
  });

  _server.on("/nosleep", []() {
    term.println("NO SLEEP");
    noSleep = !noSleep;
    String var = noSleep ? "noSleep Active" : "noSleep Inactive";
    _server.send(200, "text/html", var);
    send_data(var);
  });

  _server.on("/nosleepcheck", []() {
    term.println("NO SLEEP CHECK");
    String var = noSleep ? "noSleep Active" : "noSleep Inactive";
    _server.send(200, "text/html", var);
    send_data(var);
  });
  _server.on("/shutdownbooltoggle", []() {
    allowshutDownWhenFail = !allowshutDownWhenFail;
    String var = allowshutDownWhenFail ? "SHUTDOWN ALLOWED" : "SHUTDOWN FORBIDDEN";
    _server.send(200, "text/html", var);
    send_data(var);
  });
  _server.on("/shutdownbool", []() {
    String var = allowshutDownWhenFail ? "SHUTDOWN ALLOWED" : "SHUTDOWN FORBIDDEN";
    _server.send(200, "text/html", var);
    if (millis() - bootTime > 15000)  // if page is active after a reboot, value won't be kept because js will request update too soon
    {
      send_data(var);  // do not update the hub if reboot is too recent so ALLOWED can be updated from device driver
    }
  });
}

