void setup()
{
  //*****************************************************************************
  //PINS CONFIGURATION
  //*****************************************************************************
  pinMode(Signal, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(Asensor, INPUT);

  digitalWrite(LED, 1);
  Blink(3, 50);

  // setup debug serial port
  Serial.begin(115000);         // setup serial with a baud rate of 9600
  term.link(Serial); //optional : echo every print() on Serial
  term.println("setup..");  // print out 'setup..' on start

  //*****************************************************************************
  //NTP CLIENT INIT
  //*****************************************************************************
  term.println("Starting TimeClient");
  timeClient.begin();

  //*****************************************************************************
  //Infra Red
  //*****************************************************************************
  irsend.begin();

  // create OTA name
  const char *NameOTA = "AC_BEDROOM_JESSICA";
  ArduinoOTA.setHostname(NameOTA); // must be created before st initialization

  //*****************************************************************************
  //IoT HUB interface initialization
  //*****************************************************************************
  smartthing.init();


  //******************************************************************************************
  // XML WEB SERVER INITIALIZATION
  //******************************************************************************************
  initXMLhttp();
  _server.begin();
  term.println("Ajax Web Server started");

  //******************************************************************************************
  // WiFi Terminal
  //******************************************************************************************
  term.begin(_server);
  term.println("WiFi Terminal ok!");

  //*****************************************************************************
  //OTA initialization
  //*****************************************************************************
  ArduinoOTA.onStart([]() {
    StopAll = true;
    term.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    term.println("\nEnd");
    ESP.reset(); // HARD RESET
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    term.printf("Progress: %u%%\r", (progress / (total / 100)));
    if (millis() - previousMillisBlink > 10)
    {
      digitalWrite(LED, !digitalRead(LED));
      previousMillisBlink = millis();
    }
  });
  ArduinoOTA.onError([](ota_error_t error) {
    StopAll = false;
    term.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
    {
      term.println("Auth Failed");
      ESP.restart();
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      term.println("Begin Failed");
      ESP.restart();
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      term.println("Connect Failed");
      ESP.restart();
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      term.println("Receive Failed");
      ESP.restart();
    }
    else if (error == OTA_END_ERROR) {
      term.println("End Failed");
      ESP.restart();
    }
  });
  ArduinoOTA.begin();
  term.println("ArduinoOTA Ready");
  term.print("IP address: ");
  term.println(WiFi.localIP());
  term.print("ArduionOTA Host Name: ");
  term.println(ArduinoOTA.getHostname());
  term.println();

  updateNtpTime();
  lastNTP = millis();
  millisBoot = millis();
  lastBootDate = timeClient.getFormattedDateTime();
  term.println("boot : " + lastBootDate);

  buildDebug("thermostatMode off"); // absolutely needed in order to refresh thermostatMode value; smart apps will do the rest
  currentMode = "OFF";
}
