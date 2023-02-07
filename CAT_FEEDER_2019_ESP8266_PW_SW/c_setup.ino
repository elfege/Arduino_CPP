void setup() {
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
  isDebugEnabled = true;
  Serial.begin(115200);
  pinMode(power, OUTPUT);
  pinMode(pwCoordination, INPUT);
  
  delay(1);
  Off(); // default is Off()


  // create OTA name before smartthings does
  const char *NameOTA = "Cat_Feeder_Power_Control";
  ArduinoOTA.setHostname(NameOTA);

  smartthing.init();

  initXMLhttp();
  _server.begin();

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
    StopAll = true;
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
    Blink(2, 1000);
    delay(2000);
    Blink(5, 100);
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
  Blink(3, 100);
}
