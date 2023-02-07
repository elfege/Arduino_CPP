void setup()
{
  digitalWrite(LED, 1);
  
  
  Serial.begin(115200);         // setup serial with a baud rate of 9600
  Serial.println("");
  Serial.println("setup..");  // print out 'setup..' on start

  // create OTA name
  const char *NameOTA = "TV_REMOTE_ESP8266";
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



  // setup hardware pins

  Serial.begin(115200);
  irsend.begin();


  pinMode(Signal, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(Asensor, INPUT);


  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();
  delay(100);

  //digitalWrite(LED, LOW); // turn on the LED

  //Serial.println("STARTING WEB SERVER");
  //server.begin();

  // set default temp
  Serial.println("");


  //******************************************************************************************
  // XML WEB SERVER INITIALIZATION
  //******************************************************************************************
  initXMLhttp();
  _server.begin();
  justbooted = true;
//  ESP.wdtDisable(); // disable the software watchdog, which triggers the hardware watchdog, which will trigger if software is not fed (ESP.wdtFeed()) before 6 seconds
  //buildDebug("BOOT OK");
}
