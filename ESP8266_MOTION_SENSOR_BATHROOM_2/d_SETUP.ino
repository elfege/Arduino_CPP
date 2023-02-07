void setup()
{
  //ESP.wdtDisable(); // disable the software watchdog, which triggers the hardware watchdog, which will trigger if software is not fed (ESP.wdtFeed()) before 6 seconds
  // DON't use wdt because ST or Hubitat might take more than 3 secs to respond
  // or any router update will lock the ESP into a reboot loop

  //  pinMode(rst, OUTPUT);
  //  digitalWrite(rst, 1);
  // setup hardware pins

  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  pinMode (ASensor, INPUT);
  digitalWrite(LED, LOW); // turn on the LED
  // create OTA name

  const char *NameOTA = "Motion_Sensor_BATHROOM_2";
  ArduinoOTA.setHostname(NameOTA);

  // setup default state of global variables
  isDebugEnabled = true;




  //Run the SmartThings init() routine to make sure the ThingShield is connected to the ST Hub
  smartthing.init();


  // Start the server
  initXMLhttp();
  _server.begin();
  Serial.println("Server started");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
    StopAll = true;
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
    Blink(2, 1000);
    hardReset(); // ESP8266 needs hard reset after flashing
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

  Serial.println("Starting UDP");
  //buildDebug("Starting UDP");

  //udp.begin(localPort);
  //buildDebug("Local port: " + String(udp.localPort()));

  timeClient.begin();

  //Blink(4, 500);

justbooted = true; 
}
