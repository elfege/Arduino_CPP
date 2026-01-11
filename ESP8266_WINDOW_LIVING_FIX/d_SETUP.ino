void setup()
{

  loopTime = millis();

  Blink(3, 100);

  // create OTA name
  const char *NameOTA = "WINDOW_LIVING";
  ArduinoOTA.setHostname(NameOTA);

  // setup debug serial port
  Serial.begin(115000);         // setup serial with a baud rate of 9600
  Serial.println("boot Time = " + String(lastBoot));

  //*****************************************************************************
  //Changing PWM frequency from 1Khz default to 60khz
  //*****************************************************************************
analogWriteFreq(frequency);


  //*****************************************************************************
  //PINS CONFIGURATION
  //*****************************************************************************
  pinMode(LED, OUTPUT);
  pinMode(pwm, OUTPUT);
  pinMode(MA, OUTPUT);
  pinMode(MB, OUTPUT);
  pinMode(contact_sensor, INPUT_PULLUP);



  //*****************************************************************************
  //"ST" initialization
  //*****************************************************************************
  // callout function forward decalaration
  SmartThingsCallout_t messageCallout;
  // setup default state of global variables
  isDebugEnabled = true;

  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();

  Serial.println("-------------------------------ST initialized--------------------------------");

  //******************************************************************************************
  // XML WEB SERVER INITIALIZATION
  //******************************************************************************************
  initXMLhttp();
  _server.begin();
  Serial.println("Ajax Web Server started");

  //*****************************************************************************
  //OTA initialization
  //*****************************************************************************
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
    StopAll = true;
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
    digitalWrite(LED, 1);//boot fails if pulled low, should be handeled by controller 
    hardReset(); // ESP8266 needs hard reset after flashing
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r\n", (progress / (total / 100)));
    
      digitalWrite(LED, !digitalRead(LED));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    digitalWrite(LED, 1);//boot fails if pulled low, should be handeled by controller 
    hardReset();
  });
  ArduinoOTA.begin();
  Serial.println("ArduinoOTA Ready");

  //Serial.println("Starting TimeClient");
  //buildDebug("Starting TimeClient");
  //timeClient.begin();

  // update smartthings hub
  Refresh();

  String var = "window boot";
  buildDebug(var); ;

  Blink(2, 300); // signal end of setup
}
