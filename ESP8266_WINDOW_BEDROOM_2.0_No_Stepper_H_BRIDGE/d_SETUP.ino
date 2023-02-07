/* IDENTICAL TO THE OTHER WINDOW USING SAME THREADED ROD BASED ACTUATOR */
void setup()
{
  //*****************************************************************************
  //PINS CONFIGURATION
  //*****************************************************************************

  pinMode(LED, OUTPUT);
  pinMode(pwm, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(FAN, OUTPUT);
  digitalWrite(FAN, 0);
  pinMode(contact_sensor_open, INPUT_PULLUP);
  pinMode(contact_sensor_fullyOpen, INPUT_PULLUP);
  Stop();


  //*****************************************************************************
  //Changing PWM frequency from 1Khz default to 60khz
  //*****************************************************************************
  analogWriteFreq(frequency);
  analogWriteResolution(10);

  Serial.begin(115000);         // setup serial with a baud rate of 115000
  term.link(Serial); //optional : echo every print() on Serial
  //  term.println("boot Time = " + String(lastBoot));

  //*****************************************************************************
  //NTP CLIENT INIT
  //*****************************************************************************
  term.println("Starting TimeClient");
  timeClient.begin();


  //******************************************************************************************
  /// RTC memory storage (resists soft resets, not hard resets)...
  //******************************************************************************************
  state.registerVar( &FINALSPEED );  // we send a pointer to each of our variables
  state.registerVar( &ULTRASLOW );
  state.registerVar( &lastOperation );
  state.registerVar( &reset_counter );
  state.registerVar( &speedVal );
  state.registerVar( &doneLearning );  
  state.registerVar( &resetFINALSPEED );  // we send a pointer to each of our variables
  state.registerVar( &program_step );
  
  if (state.loadFromRTC()) {            // we load the values from rtc memory back into the registered variables
    reset_counter++;
    term.println("This is reset no. " + (String)reset_counter);
    state.saveToRTC();                  // since we changed a state relevant variable, we store the new values
  } else {
    reset_counter = 0;                  // cold boot part
    term.println("This seems to be a cold boot. We don't have a valid state on RTC memory");
    resetRTCVars();
  }


  //*****************************************************************************
  //IoT HUB interface initialization
  //*****************************************************************************
  // create OTA name
  const char *NameOTA = "WINDOW_BEDROOM";
  ArduinoOTA.setHostname(NameOTA);

  // callout function forward decalaration
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
    term.handleClient();// WiFi terminal
    digitalWrite(LED, !digitalRead(LED));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    term.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) term.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) term.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) term.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) term.println("Receive Failed");
    else if (error == OTA_END_ERROR) term.println("End Failed");
    digitalWrite(LED, 1);//boot fails if pulled low, should be handeled by controller
    hardReset();
  });
  ArduinoOTA.begin();
  term.println("ArduinoOTA Ready");


  // update hub
  Refresh();

  String var = "window boot";
  buildDebug(var); ;

  windowLastState = isOpen() ? "window is open" : "WINDOW IS CLOSED";

  updateNtpTime();
  lastNTP = millis();
  millisBoot = millis();
  lastBootDate = timeClient.getFormattedDateTime();
  term.println("boot : " + lastBootDate);

}


void resetRTCVars()
{
  term.println("RESET ALL RTC VARS!");
  ULTRASLOW = 450; //PWM signals on ESP8266 have 10-bit resolution
  FINALSPEED = resetFINALSPEED; // DEPRECATED // reinstate default value for FINALSPEED
  lastOperation = 0;
  speedVal = ULTRASLOW;
  doneLearning = 0;
}
