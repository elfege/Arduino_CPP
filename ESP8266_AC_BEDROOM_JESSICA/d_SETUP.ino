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

  //******************************************************************************************
  /// RTC memory storage (resists soft resets, not hard resets)...
  //******************************************************************************************
  state.registerVar( &lastOperation );
  state.registerVar( &reset_counter );
  state.registerVar( &lastSetPointRTC);
  state.registerVar( &lastTimeCmdSent);


  if (state.loadFromRTC()) {            // we load the values from rtc memory back into the registered variables
    reset_counter++;
    term.println("This is reset no. " + (String)reset_counter);
    term.println("lastOperation = " + (String)lastOperation);
    term.println("lastSetPointRTC = " + (String)lastSetPointRTC);
    lastSetPoint = String(lastSetPointRTC);
    state.saveToRTC();                  // since we changed a state relevant variable, we store the new values
  } else {
    reset_counter = 0;                  // cold boot part
    term.println("This seems to be a cold boot. We don't have a valid state on RTC memory");
    lastOperation = 4; // reinstate default value for lastOperation (undefined)
    lastSetPointRTC = lastSetPoint.toInt();
    fanMode = 0; // 0 = on 1 = auto
    lastSetpointRequested = lastSetPoint;
    lastTimeCmdSent = 1000 * 60 * 5; 
  }


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
    tempRequest = false;
    StopAll = true;
    term.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    term.println("\nEnd");
    ESP.reset(); // HARD RESET
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    term.printf("Progress: %u%%\r", (progress / (total / 100)));
    digitalWrite(LED, !digitalRead(LED));
    term.handleClient();
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

  term.println("This is reset no. " + (String)reset_counter);

  //  buildDebug("thermostatMode undefined"); // absolutely needed in order to refresh thermostatMode value; smart apps will do the rest
  //  must not be any of existing states. Otherwise if, for example, the app turned it off but this controller rebooted while the AC was still on,
  //  then the app will believe it's already on and let it run forever.
  //  currentMode = "undefined";
}
