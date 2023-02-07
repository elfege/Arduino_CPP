void setup()
{
  //*****************************************************************************
  //PINS CONFIGURATION
  //*****************************************************************************
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW); // turn on the LED
  pinMode(Signal, OUTPUT);
  digitalWrite(Signal, 0);

  //*****************************************************************************
  //Serial
  //*****************************************************************************
  Serial.begin(115200);         // setup serial with a baud rate of 115200
  term.println("setup..");  // print out 'setup..' on start
  Serial.begin(115000);         // setup serial with a baud rate of 115000
  term.link(Serial); //optional : echo every print() on Serial
  //  term.println("boot Time = " + String(lastBoot));

  //*****************************************************************************
  //NTP CLIENT INIT
  //*****************************************************************************
  term.println("Starting TimeClient");
  timeClient.begin();

  //*****************************************************************************
  //IoT HUB interface initialization
  //*****************************************************************************
  // create OTA name
  const char *NameOTA = "CatsWaterTank";
  ArduinoOTA.setHostname(NameOTA);

  //*****************************************************************************
  //"ST" initialization
  //*****************************************************************************
  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();
  term.println("ST initialized");

  //******************************************************************************************
  // XML WEB SERVER INITIALIZATION
  //******************************************************************************************
  term.println("STARTING WEB SERVER");
  initXMLhttp();
  _server.begin();

  //******************************************************************************************
  // WiFi Terminal
  //******************************************************************************************
  //server.begin();
  term.begin(_server);
  term.println("WiFi Terminal ok!");

  //*****************************************************************************
  //OTA initialization
  //*****************************************************************************

  ArduinoOTA.onStart([]() {
    term.println("Start");
    StopAll = true;
  });
  ArduinoOTA.onEnd([]() {
    term.println("\nEnd");
    //Blink(2, 1000);
    //    delay(2000);
    //    Blink(5, 100);
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    term.printf("Progress: %u%%\r", (progress / (total / 100)));
    digitalWrite(LED, !digitalRead(LED));
    term.handleClient();// WiFi terminal
  });
  ArduinoOTA.onError([](ota_error_t error) {
    term.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) term.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) term.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) term.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) term.println("Receive Failed");
    else if (error == OTA_END_ERROR) term.println("End Failed");
  });
  ArduinoOTA.begin();
  term.println("ArduinoOTA Ready");

  // update hub
  Refresh();

  String var = "device boot";
  buildDebug(var); ;

  wet(); // update 'wetState' global varialble

  updateNtpTime();
  lastNTP = millis();

  term.println("boot time: " + timeClient.getFormattedDateTime());

}
