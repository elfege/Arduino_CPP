void setup()
{
  
  // create OTA name
  const char *NameOTA = "CURTAINS_LIVING";
  ArduinoOTA.setHostname(NameOTA);

  // setup default state of global variables
  isDebugEnabled = true;

  Serial.begin(115200);

  // setup hardware pins

  pinMode(LED, OUTPUT);
  pinMode(OpenPin, OUTPUT);
  pinMode(ClosePin, OUTPUT);
  pinMode(StopPin, OUTPUT);
  digitalWrite(OpenPin, LOW);
  digitalWrite(StopPin, LOW);
  digitalWrite(ClosePin, LOW);
  digitalWrite(LED, LOW); // turn on the LED

  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();



  //******************************************************************************************
  // XML WEB SERVER INITIALIZATION
  //******************************************************************************************
  initXMLhttp();
  _server.begin();


  ArduinoOTA.setHostname(NameOTA);
  ArduinoOTA.onStart([]() {
    StopAll = true;
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
    ESP.reset(); // HARD RESET
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
    StopAll = false;
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
    {
      Serial.println("Auth Failed");
      ESP.restart();
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      Serial.println("Begin Failed");
      ESP.restart();
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      Serial.println("Connect Failed");
      ESP.restart();
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      Serial.println("Receive Failed");
      ESP.restart();
    }
    else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
      ESP.restart();
    }
  });
  ArduinoOTA.begin();
  Serial.println("ArduinoOTA Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("ArduionOTA Host Name: ");
  Serial.println(ArduinoOTA.getHostname());
  Serial.println();

  Serial.println("SETUP OK");

  Blink(10, 50);
  //On(); // to sync the boolean properly
  CurtainsAreOpen = true;// force true to prevent noisy reboots
  delay(1);
  Stop();
  buildDebug("BOOT OK");
  Refresh();

}
