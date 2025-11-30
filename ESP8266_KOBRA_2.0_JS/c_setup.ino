void setup() {
  pinMode(rst, OUTPUT);
  digitalWrite(rst, 1);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
  isDebugEnabled = true;
  Serial.begin(115200);
  pinMode(power, OUTPUT);
  pinMode(CHARGER, OUTPUT);
  pinMode(CHARGERin, INPUT);
  pinMode(ESP32rst, OUTPUT);
  digitalWrite(ESP32rst, 1);// low is reset

  pinMode(CAM, OUTPUT);
  digitalWrite(CAM, 0);


  // create OTA name before smartthings does
  const char *NameOTA = "KOBRA_PW_ESP8266";
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
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
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
  //  On(); // during devlpmt only!
  Blink(4, 100);

}
