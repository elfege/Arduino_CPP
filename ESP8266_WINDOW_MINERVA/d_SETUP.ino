

void setup()
{
  RTCRegistration();
  state.loadFromRTC();
  if (failSafeBoot == 1)
  {

    failSafeSetup();

  }
  else
  {

    unsigned long setupStart = millis();
    //*****************************************************************************
    //PINS CONFIGURATION
    //*****************************************************************************
    pinConfig();

    //*****************************************************************************
    //Changing PWM frequency from 1Khz default to 60khz
    //*****************************************************************************
    analogWriteFreq(frequency);
    analogWriteResolution(10);


    // setup debug serial port
    Serial.begin(115200);         // setup serial with a baud rate of 115200
    term.link(Serial); //optional : echo every print() on Serial

    // create OTA name

    ArduinoOTA.setHostname(NameOTA);// must be created before st initialization


    //*****************************************************************************
    //IoT HUB interface initialization
    //*****************************************************************************
    // callout function forward decalaration
    SmartThingsCallout_t messageCallout;
    // setup default state of global variables
    isDebugEnabled = true;

    //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
    smartthing.init();
    term.println("ST initialized");


    //*****************************************************************************
    //NTP CLIENT INIT
    //*****************************************************************************
    term.println("Starting TimeClient");
    timeClient.begin();

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

    String var = "window boot";
    sendHub(var); ;
    windowLastState = isOpen() ? "window is open" : "window is closed";

    //******************************************************************************************
    /// RTC memory storage (resists soft resets, not hard resets)...
    //******************************************************************************************


    updateFromRTC();

    updateNtpTime();
    lastNTP = millis();
    millisBoot = millis();
    term.println("---------------------------------------------------------------");
    term.println("Program took " + String(millis()) + " milliseconds to boot up");
    term.println("---------------------------------------------------------------");
    term.println("Setup took " + String(millis()) + " milliseconds");
    term.println("---------------------------------------------------------------");
  }

 

}

void updateFromRTC() {
  if (state.loadFromRTC()) {            // we load the values from rtc memory back into the registered variables
    reset_counter++;
    term.println("This is reset no. " + (String)reset_counter);
    state.saveToRTC();                  // since we changed a state relevant variable, we store the new values
    term.println("RTC vars are:");
    RTCVarsDebug();
  } else {
    term.println("This seems to be a cold boot. We don't have a valid state on RTC memory");
    resetRTCVars();
  }
}


void resetRTCVars() {
  reset_counter = 0;                  // cold boot part
  CLOSINGSPEED = resetClosingSpeed;
  ULTRASLOW = ULTRASLOW_RESTORE;
  FIRSTPUSH = ULTRASLOW + 100;
  lastSpeedVal = ULTRASLOW;
  openLearn = 0; // pseudo-boolean RTC mem can't store booleans
  closeLearn = 0;  // pseudo-boolean
  successfulOpenLearn = 0;  // pseudo-boolean
  successfulCloseLearn = 0;  // pseudo-boolean
  lastOperation = 0;  // pseudo-boolean
  donotAcceptCmdsFromHubs = 0;  // pseudo-boolean
  method_1_Failed = 0;  // pseudo-boolean
  //failSafeBoot = 0;
  state.saveToRTC();
  term.println("ALL RTC VARS SET TO INITIAL DEFAULT VALUES!");
  RTCVarsDebug();
}
void RTCVarsDebug() {
  term.println("************************RTC VARS STATES*************************************");
  term.print("CLOSINGSPEED ="); term.println(CLOSINGSPEED);
  term.print("ULTRASLOW ="); term.println(ULTRASLOW);
  term.print("FIRSTPUSH ="); term.println(FIRSTPUSH);
  term.print("lastSpeedVal ="); term.println(lastSpeedVal);
  term.print("openLearn ="); term.println(String(openLearn == 1 ? "true" : "false"));
  term.print("closeLearn ="); term.println(String(closeLearn == 1 ? "true" : "false"));
  term.print("successfulOpenLearn ="); term.println(String(successfulOpenLearn == 1 ? "true" : "false"));
  term.print("successfulCloseLearn ="); term.println(String(successfulCloseLearn == 1 ? "true" : "false"));
  term.print("donotAcceptCmdsFromHubs ="); term.println(String(donotAcceptCmdsFromHubs == 1 ? "true" : "false"));
  term.print("method_1_Failed ="); term.println(String(method_1_Failed == 1 ? "true" : "false"));
  term.print("failSafeBoot ="); term.println(String(failSafeBoot == 1 ? "true" : "false"));
  term.println("*************************************************************");
}

void failSafeSetup() {

  StopAll = true;
  pinMode(LED, OUTPUT); // no other pin config in fail safe mode, especially ISR's!
  Serial.begin(115200);
  term.link(Serial); //optional : echo every print() on Serial
  RTCRegistration();
  ArduinoOTA.setHostname(NameOTA);//
  WiFiConfig();
  term.begin(_server);
  term.println("WiFi Terminal ok!");
  initXMLhttp();
  _server.begin();
  term.println("Ajax Web Server started");
  OTAConfig();
  //failSafeBoot = 0; // has to be reset by new request on fail safe boot button or "/failsafe"
  inFailSafeMode = true;
  term.println("FAIL SAFE BOOT DONE!");
}
