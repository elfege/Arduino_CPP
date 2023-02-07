

void setup()
{

  Serial.begin(115200);
  EverythingInit(); // connect wifi and hub services
  OTAconfig();


  pinMode(ENABLE_LEFT, OUTPUT); ////
  pinMode(MOTOR_LEFT1, OUTPUT);
  pinMode(MOTOR_LEFT2, OUTPUT);
  pinMode(ENABLE_RIGHT, OUTPUT);
  pinMode(MOTOR_RIGHT1, OUTPUT);
  pinMode(MOTOR_RIGHT2, OUTPUT);
  pinMode(CHARGERelay, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(POWER, OUTPUT);
  digitalWrite(POWER, 1);
  digitalWrite(LED, 1); // 1 IS LED OFF

  pinMode(LED, OUTPUT);

  // com with atMega2560
  Serial2.begin(115200, SERIAL_8N1, rx2, tx2); // pins 16 rx2, 17 tx2, 19200 bps, 8 bits no parity 1 stop bit

  analogReadResolution(11);        // Default of 12 is not very linear.  Recommended to use 10 or 11 depending on needed resolution.
  analogSetAttenuation(ADC_6db);   // Default is 11db which is very noisy.  Recommended to use 2.5 (noted "ADC_2_5db") or 6.

  initXMLhttp();
  _server.begin();
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);

  // MOTORS CONFIGURATION
  // configure PWM functionalitites
  ledcSetup(pwmChannelL, freq, resolution);
  ledcSetup(pwmChannelR, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ENABLE_LEFT,  pwmChannelL);
  ledcAttachPin(ENABLE_RIGHT, pwmChannelR);

  //GLOWING LED CONFIGURATION
  ledcSetup(pwmChannelGlow, freq, resolution);
  ledcAttachPin(LED, pwmChannelGlow);

}

void EverythingInit()
{
  st::Everything::debug = false;
  st::Executor::debug = false;
  SmartThingsCallout_t messageCallout;
  bool enableDebug = false;
  // st::Everything::SmartThing = new st::SmartThingsESP32WiFi_DUAL_SSID(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);
  st::Everything::SmartThing = new st::SmartThingsESP32_WIFI_SSID(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);

  st::Everything::init();
}

void OTAconfig()
{

  // Port defaults to 3232
  ArduinoOTA.setPort(3232);
  // Hostname defaults to esp3232-[MAC]
  // create OTA name
  const char *NameOTA = "MEBO";
  ArduinoOTA.setHostname(NameOTA);
  // No authentication by default
  // ArduinoOTA.setPassword("admin");
  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
  .onStart([]() {
    stopAll = true;
    _server.stop();
    Serial2.end();
    OTArequest = true;
    if (runIRreceiver == true)
    {
      Serial.println("*****************************************************************************");
      Serial.println("************ IR RECEIVER IS ACTIVE! RESET REQUIRED BEFORE OTA FLASH *********");
      Serial.println("*****************************************************************************");
      Reset();
    }
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    //SPIFFS.end();
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd");
    Blink(3, 300);
    delay(1000);
    //    Reset();// no more hard reset in this function
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.println();
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    if (millis() - previousMillisBlink > 30)
    {
      ledcWrite(pwmChannelGlow, 0);
      previousMillisBlink = millis();
    }
    else {
      ledcWrite(pwmChannelGlow, 128);
    }
  })
  .onError([](ota_error_t error) {
    OTArequest = false;
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    //    Reset();// no more hard reset in this function
  });
  ArduinoOTA.begin();
}
