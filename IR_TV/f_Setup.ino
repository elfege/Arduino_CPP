void setup() {
  digitalWrite(LED, 1);


  // setup debug serial port
  Serial.begin(115200);  // setup serial with a baud rate of 115200
  term.link(Serial);     //optional : echo every print() on Serial

  ArduinoOTA.setHostname(NameOTA);// must be created before st initialization

  pinMode(Signal, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(Asensor, INPUT);


  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();

  //digitalWrite(LED, LOW); // turn on the LED

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


  //*****************************************************************************
  //OTA initialization
  //*****************************************************************************
  OTAConfig();
}

void OTAConfig() {
  // create OTA name
  
  ArduinoOTA.setHostname(NameOTA);
  ArduinoOTA.onStart([]() {
    StopAll = true;
    term.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    term.println("\nEnd");
    ESP.reset();  // HARD RESET
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    if (millis() - previousMillisBlink > 10) {
      digitalWrite(LED, !digitalRead(LED));
      previousMillisBlink = millis();
    }
  });
  ArduinoOTA.onError([](ota_error_t error) {
    StopAll = false;
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      term.println("Auth Failed");
      ESP.restart();
    } else if (error == OTA_BEGIN_ERROR) {
      term.println("Begin Failed");
      ESP.restart();
    } else if (error == OTA_CONNECT_ERROR) {
      term.println("Connect Failed");
      ESP.restart();
    } else if (error == OTA_RECEIVE_ERROR) {
      term.println("Receive Failed");
      ESP.restart();
    } else if (error == OTA_END_ERROR) {
      term.println("End Failed");
      ESP.restart();
    }
  });
  ArduinoOTA.begin();
  term.println("ArduinoOTA Ready");
  Serial.print("IP address: ");
  term.println(WiFi.localIP());
  Serial.print("ArduionOTA Host Name: ");
  term.println(ArduinoOTA.getHostname());
  term.println();
}