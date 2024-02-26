void setup() {


  unsigned long setupStart = millis();
  //*****************************************************************************
  //PINS CONFIGURATION
  //*****************************************************************************
  pinConfig();

  Blink(10, 50);

  //*****************************************************************************
  //Changing PWM frequency from 1Khz default to 60khz
  //*****************************************************************************
  // analogWriteFreq(frequency);
  // analogWriteResolution(10);


  // setup debug serial port
  Serial.begin(115200);  // setup serial with a baud rate of 115200
  term.link(Serial);     //optional : echo every print() on Serial

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
  term.println("ST initialized");


  //*****************************************************************************
  //NTP CLIENT INIT
  //*****************************************************************************
  // term.println("Starting TimeClient");
  // timeClient.begin();

  //******************************************************************************************
  // WiFi Terminal
  //******************************************************************************************
  term.begin(_server);
  term.println("WiFi Terminal ok!");

  //******************************************************************************************
  // XML WEB SERVER INITIALIZATION
  //******************************************************************************************
  initXMLhttp();
  _server.begin();
  term.println("Ajax Web Server started");

  //*****************************************************************************
  //OTA initialization
  //*****************************************************************************
  OTAConfig();

  // update hub
  Refresh(); 

  Blink(3, 500);

  term.println("REBOOT DONE");

}

void pinConfig(){
  pinMode(DOOR, OUTPUT);
  pinMode(TALK, OUTPUT);
  pinMode(LED, OUTPUT);
  // pinMode(sensorPin, INPUT); // A0 always input... 
}


void OTAConfig() {
  ArduinoOTA.onStart([]() {
    term.println("Start");
    StopAll = true;
  });
  ArduinoOTA.onEnd([]() {
    term.println("\nEnd");
    //digitalWrite(LED, 1);  //boot fails if pulled low, should be handeled by controller
    //hardReset(); // ESP8266 needs hard reset after flashing // deletes all RTCVars
    ESP.restart();  // soft reset instead
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    term.printf("Progress: %u%%\r\n", (progress / (total / 100)));
    term.handleClient();
    digitalWrite(LED, !digitalRead(LED));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    term.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) term.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) term.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) term.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) term.println("Receive Failed");
    else if (error == OTA_END_ERROR) term.println("End Failed");
    digitalWrite(LED, 1);  //boot fails if pulled low, should be handeled by controller though
    //hardReset();
  });
  ArduinoOTA.begin();
  term.println("ArduinoOTA Ready");
}


