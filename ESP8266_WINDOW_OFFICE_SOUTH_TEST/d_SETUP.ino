void setup()
{


  //*****************************************************************************
  //PINS CONFIGURATION
  //*****************************************************************************

  pinMode(LED, OUTPUT);
  pinMode(pwm, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(contact_sensor_open, INPUT_PULLUP);
  pinMode(contact_sensor_fullyOpen, INPUT_PULLUP);
  Stop();

  //*****************************************************************************
  //Changing PWM frequency from 1Khz default to 60khz
  //*****************************************************************************
  analogWriteFreq(frequency);

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
  const char *NameOTA = "WINDOW_BEDROOM";
  ArduinoOTA.setHostname(NameOTA);

  //*****************************************************************************
  //"ST" initialization
  //*****************************************************************************
  // callout function forward declaration
  SmartThingsCallout_t messageCallout;
  // setup default state of global variables
  isDebugEnabled = true;

  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();
  term.println("ST initialized");

  //******************************************************************************************
  // XML WEB SERVER INITIALIZATION
  //******************************************************************************************
  initXMLhttp();
  _server.begin();
  term.println("Ajax Web Server started");

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
    digitalWrite(LED, 1);//boot fails if pulled low, should be handeled by controller
    hardReset(); // ESP8266 needs hard reset after flashing
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    term.printf("Progress: %u%%\r\n", (progress / (total / 100)));

    digitalWrite(LED, !digitalRead(LED));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    term.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) term.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) term.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) term.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) term.println("Receive Failed");
    else if (error == OTA_END_ERROR) term.println("End Failed");
    digitalWrite(LED, 1);//boot fails if pulled low, should be handeled by controller though
    //hardReset();
  });
  ArduinoOTA.begin();
  term.println("ArduinoOTA Ready");

 
  // update hub
  Refresh();

  String var = "window boot";
  buildDebug(var); ;

  windowLastState = isOpen() ? "window is open" : "window is closed";

  updateNtpTime();
  lastNTP = millis();

  term.println("boot : " + timeClient.getFormattedDateTime());


}
