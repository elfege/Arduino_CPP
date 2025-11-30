void setup() {

  unsigned long setupStart = millis();
  //*****************************************************************************
  //PINS CONFIGURATION
  //*****************************************************************************
  pinConfig();
  off();  //always off after a reset

  Blink(10, 50);

  //*****************************************************************************
  //Changing PWM frequency from 1Khz default to 60khz
  //*****************************************************************************
  // analogWriteFreq(frequency);
  // analogWriteResolution(10);

  /*
Once you start using GPIO1 (TX) and GPIO3 (RX) as normal I/O, don't call Serial.begin(115200) 
as it will confuse things (but see next for alternatives). So cannot debug using the Serial connection.
However you can still debug via GPIO2 (TX only) by starting Serial1 instead of Serial. 
See this project for an example of sending debug to GPIO2: https://www.forward.com.au/pfod/CheapWifiShield/NMEAbridge/index.html.
*/

  // setup debug serial port
  // Serial.begin(115200);  // setup serial with a baud rate of 115200
  // term.link(Serial);

  //Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);  // This allows you to use RX (GPIO3) as normal I/O, while still writing debug messages to Serial.

  //Serial1.begin(115200);  // use Serial1 (tx only on GPIO2) to free GPIO 1 (LED)
  // term.link(Serial1);     //optional : echo every print() on Serial


  // create OTA name

  ArduinoOTA.setHostname(NameOTA);  // must be created before st initialization


  //*****************************************************************************
  //IoT HUB interface initialization
  //*****************************************************************************
  // callout function forward decalaration
  SmartThingsCallout_t messageCallout;
  // setup default state of global variables
  // isDebugEnabled = true;

  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();
  if(debug) term.println("ST initialized");


  //*****************************************************************************
  //NTP CLIENT INIT
  //*****************************************************************************
  // if(debug) term.println("Starting TimeClient");
  // timeClient.begin();

  //******************************************************************************************
  // WiFi Terminal
  //******************************************************************************************
  // term.begin(_server); // conditioned to debug boolean to preserve GPIO1 / LED
  // if(debug) term.println("WiFi Terminal ok!");

  //******************************************************************************************
  // XML WEB SERVER INITIALIZATION
  //******************************************************************************************
  initXMLhttp();
  _server.begin();
  if(debug) term.println("Ajax Web Server started");

  //*****************************************************************************
  //OTA initialization
  //*****************************************************************************
  OTAConfig();

  // update hub
  Refresh();

  Blink(3, 500);



  Serial.println("MEBO'S AUXILIARY POWER CONTROLLER READY.. ");
}

void pinConfig() {

  pinMode(POWER, OUTPUT);
  pinMode(POWER_B, OUTPUT);
  // pinMode(POWER_C, OUTPUT);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  digitalWrite(POWER, 0);
  digitalWrite(POWER_B, 0);
  // digitalWrite(POWER_C, 0);
}


void OTAConfig() {
  ArduinoOTA.onStart([]() {
    if(debug) term.println("Start");
    StopAll = true;
  });
  ArduinoOTA.onEnd([]() {
    if(debug) term.println("\nEnd");
    //digitalWrite(LED, 1);  //boot fails if pulled low, should be handeled by controller
    //hardReset(); // ESP8266 needs hard reset after flashing // deletes all RTCVars
    ESP.restart();  // soft reset instead
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    if(debug) term.printf("Progress: %u%%\r\n", (progress / (total / 100)));
    term.handleClient();
    digitalWrite(LED, !digitalRead(LED));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    if(debug) term.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) if(debug) term.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) if(debug) term.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) if(debug) term.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) if(debug) term.println("Receive Failed");
    else if (error == OTA_END_ERROR) if(debug) term.println("End Failed");
    digitalWrite(LED, 1);  //boot fails if pulled low, should be handeled by controller though
    //hardReset();
  });
  ArduinoOTA.begin();
  if(debug) term.println("ArduinoOTA Ready");
}
