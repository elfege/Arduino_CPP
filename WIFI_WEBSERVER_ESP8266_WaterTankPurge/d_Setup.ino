void setup()
{
  ESP.wdtDisable();
  // create OTA name
  const char *NameOTA = "CatsWaterTank";
  ArduinoOTA.setHostname(NameOTA);

  // setup default state of global variables
  isDebugEnabled = true;


  Serial.begin(115200);         // setup serial with a baud rate of 9600
  Serial.println("");
  Serial.println("setup..");  // print out 'setup..' on start


  // setup hardware pins

  pinMode(LED, OUTPUT);
  pinMode(Signal, OUTPUT);
  digitalWrite(Signal, 0);


  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();
  digitalWrite(LED, LOW); // turn on the LED

  Serial.println("STARTING WEB SERVER");
  initXMLhttp();
  _server.begin();

  // update smartthings hub
  Refresh();

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
    StopAll = true;
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
    //Blink(2, 1000);
    //    delay(2000);
    //    Blink(5, 100);
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    if (millis() - previousMillisBlink > 10)
    {
      digitalWrite(LED, !digitalRead(LED));
      previousMillisBlink = millis();
    }
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("ArduinoOTA Ready");

  buildDebug("BOOT OK");
  //Blink(10, 50);

}
