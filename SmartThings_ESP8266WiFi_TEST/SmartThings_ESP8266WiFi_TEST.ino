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
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266Ping.h>
#include <SmartThingsESP8266WiFi.h>

//*****************************************************************************
// Pin Definitions    | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                    V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
//******************************************************************************************
//NodeMCU ESP8266 Pin Definitions (makes it much easier as these match the board markings)
//******************************************************************************************
#define LED_BUILTIN 16
#define BUILTIN_LED 16

#define D0 16
#define D1  5
#define D2  4
#define D3  0
#define D4  2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9  3
#define D10 1


#define LED LED_BUILTIN  //Onboard LED

//*****************************************************************************
int const Signal =  D5; // (GPIO16 NO PULLUP RESISTOR)
int const ASensor = A0; // sensor input

//*****************************************************************************
SmartThingsCallout_t messageCallout;    // call out function forward decalaration

//******************************************************************************************
//ESP8266 WiFi Information    CHANGE THIS INFORMATION ACCORDINGLY FOR YOUR NETWORK!
//******************************************************************************************
String str_ssid     = "WIFI_SSID";                           //  <---You must edit this line!
String str_password = "WIFI_PASSWORDGahsh4479#$";                   //  <---You must edit this line!
//static byte g_abyMyMacAddress[] = {0x02, 0x1A, 0x4B, 0x38, 0x3F, 0x8E};
IPAddress ip(192, 168, 10, 190);       // Device IP Address      //  <---You must edit this line!
IPAddress gateway(192, 168, 10, 1);    //router gateway          //  <---You must edit this line!
IPAddress subnet(255, 255, 255, 0);   //LAN subnet mask         //  <---You must edit this line!
IPAddress dnsserver(192, 168, 10, 1);  //DNS server              //  <---You must edit this line!
const unsigned int serverPort = 8090; // port to run the http server on

// Smartthings Hub Information
IPAddress hubIp(192, 168, 10, 145);    // smartthings hub ip     //  <---You must edit this line!
const unsigned int hubPort = 39500;   // smartthings hub port


//Create a SmartThings Ethernet ESP8266WiFi object
st::SmartThingsESP8266WiFi smartthing(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);

bool isDebugEnabled;    // enable or disable debug in this example

///////////////////
int blinks;
int blinksCount;
int lapse = 200;

WiFiServer server(80);
IPAddress pingAddr(192, 168, 10, 1); // ip address to ping



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
  pinMode(Signal, OUTPUT);     // define Signal as an output
  digitalWrite(Signal, 0);   // set value to 0

  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();

  //synch up the ST cloud
  smartthing.send("off");       // send message to cloud

  //// OTA
  Serial.println("Booting OTA");
  //  WiFi.mode(WIFI_STA);
  //  WiFi.begin(ssid, password);
  //  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
  //    Serial.println("Connection Failed! Rebooting...");
  //    delay(5000);
  //    ESP.restart();
  //  }

  // Port defaults to 8266
  ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("ComputerOfficeESP8266OTA");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("STARTING WEB SERVER");
   server.begin();
}

//*****************************************************************************
void loop()
{
  // run smartthing logic
  smartthing.run();
  ArduinoOTA.handle();

  WiFiClient client = server.available();
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            // refresh the page automatically every 1 sec
            //client.println("Refresh: 5");
            //client.println();

            // the content of the HTTP response follows the header:
            client.println("<HTML>");
            client.println("<BODY style='background-color:teal'>");
            client.println("<font style='color:green'>");
            client.println("<font style='font-family:electric toaster'>");
            client.println("<center>");
            client.println("<H1 style='font-size:400%;color:yellow';>COMPUTER OFFICE</H1>");

            client.println("<br />");
            //client.println(WiFi.localIP());
            client.println("<br />");
            if (isOn()) {
              client.println("<H3 style='font-size:100%;color:yellow';>COMPUTER OFFICE IS RUNNING</H3>");
            }
            else {
              client.println("<H3 style='font-size:100%;color:white';>COMPUTER OFFICE IS OFF</H3>");
            }


            client.println("<strong>");
            client.println("<a href=\"/ON\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>   ON  </button></a>");
            client.println("<br />");
            client.println("<a href=\"/OFF\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>   OFF  </button></a>");
            client.println("<br />");
            client.println("<br />");
            client.println("<a href=\"/SLEEP\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>   SLEEP  </button></a>");
            client.println("<br />");
            client.println("<br />");
            client.println("<a href=\"/R\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   reset controller  </button></a>");
            client.println("<strong>");
            //            client.println("<a href=\"/TEST_1\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>  TEST ON  </button></a>");
            //            client.println("<br />");
            //            client.println("<a href=\"/TEST_0\"\"><button style='font-size:170%;background-color:darkgray; color:green; border-radius:2000px; position:center; top:530px; left:300px;'>  TEST OFF  </button></a>");
            //            client.println("<br />");

            client.println("</center>");
            client.println(F("<hr />"));
            client.println("</strong>");
            client.println("</BODY>");
            client.println("</HTML>");


            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
       
        if (currentLine.endsWith("GET /ON")) {
          On();
        }
        if (currentLine.endsWith("GET /OFF")) {
          Off();
        }
        if (currentLine.endsWith("GET /SLEEP")) {
          Sleep();
        }
        if (currentLine.endsWith("GET /R")) {

          String Message1 = "<a style='font-size:400%;'>This page will refresh in </a>";
          String refreshtime = "<a style='font-size:400%;'>30</a>";
          String Message2 = "<a style='font-size:400%;'> seconds</a>";
          String MessageFinal = Message1 + refreshtime + Message2;


          // the content of the HTTP response follows the header:
          client.println("<HTML>");
          client.println("<BODY style='background-color:black'>");
          client.println("<font style='color:green'>");
          client.println("<font style='font-family:electric toaster'>");
          client.println("<center>");
          client.println("<H1 style='font-size:400%;'>RESETING CONTROLER</H1>");
          client.println();
          client.print(MessageFinal);


          //client.print("<meta http-equiv='refresh' content='10; url=http://philonyc.com/' />");


          String url1 = "<meta http-equiv='refresh' content='20; url=http://";
          String url2 = "'/>";
          String UrlFinal = url1 + WiFi.localIP() + url2 ;
          Serial.println(UrlFinal);
          client.print(UrlFinal);


          client.println();

          client.println("</center>");
          client.println("</strong>");
          client.println("</BODY>");
          client.println("</HTML>");

          client.stop();

          ESP.restart();
          break;

        }

      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }

  delay(50);

}

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
  if (message.equals("on"))
  {
    On();
  }
  else if (message.equals("off"))
  {
    Off();
  }
  else if (message.equals("reset"))
  {
    Reset();
  }
  else if (message.equals("sleep"))
  {
    Sleep();
  }
}


///// LOCAL FUNCTIONS ////
void On() {

  if (!isOn()) {
    digitalWrite(Signal, 1);
    delay(500);
    digitalWrite(Signal, 0);

   
    Serial.println("");
    Serial.println("");
    Serial.println("COMPUTER TURNED ON");
    smartthing.send("on");       // send message to cloud
  }
  else {
    Serial.println("ALREADY RUNNING");
  }

}
void Off() {

  if (isOn()) {
    digitalWrite(Signal, 1);
    while (isOn()) {
      yield();
    }
    digitalWrite(Signal, 0);
    
    Serial.println("");
    Serial.println("");
    Serial.println("COMPUTER TURNED OFF");
    smartthing.send("off");       // send message to cloud
  }
  else {
    Serial.println("ALREADY OFF");
  }
}


void Sleep() {

  if (isOn()) {
    digitalWrite(Signal, 1);
    delay(500);
    digitalWrite(Signal, 0);
    
    Serial.println("");
    Serial.println("");
    Serial.println("COMPUTER PUT TO SLEEP");
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

  Serial.println("client disonnected");
  Serial.println("");
  ESP.restart();
}
