
void setup()
{


  Serial.println("SERIAL BEGIN");
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);

  Serial.println("MotorsPw");
  pinMode(MotorsPw, OUTPUT);
  digitalWrite(MotorsPw, 1); //on
  
  Serial.println("TRACKS PINS");
  pinMode(TRACK_MOTOR_A, OUTPUT);
  pinMode(TRACK_MOTOR_B, OUTPUT);
  delay(1);
  trackstop();

  Serial.println("MOTORS PINS");
  pinMode(ENABLE_LEFT, OUTPUT); ////
  pinMode(MOTOR_LEFT1, OUTPUT);
  pinMode(MOTOR_LEFT2, OUTPUT);
  pinMode(ENABLE_RIGHT, OUTPUT);
  pinMode(MOTOR_RIGHT1, OUTPUT);
  pinMode(MOTOR_RIGHT2, OUTPUT);

  pinMode(LineLeftFront, INPUT);
  pinMode(LineRightFront, INPUT);


  Serial.println("ATTACHING SERVOS");
  tower.setPeriodHertz(50);    // standard 50 hz servo
  tower.attach(towerPin, 500, 3000);
  arm.setPeriodHertz(50);    // standard 50 hz servo
  arm.attach(armPin, 500, 2500);
  forearm.setPeriodHertz(50);    // standard 50 hz servo
  forearm.attach(forearmPin, 500, 2500);
  wrist.setPeriodHertz(50);    // standard 50 hz servo0
  clamp.setPeriodHertz(50);    // standard 50 hz servo
  clamp.attach(clampPin, 500, 2500);


  Serial.println("MOTORS CONFIGURATION");
  // configure PWM functionalitites
  ledcSetup(pwmChannelL, freq, resolution);
  ledcSetup(pwmChannelR, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ENABLE_LEFT,  pwmChannelL);
  ledcAttachPin(ENABLE_RIGHT, pwmChannelR);
  delay(1);
  Serial.println("STOPPING MOTORS");
  STOP();

  Serial.println("CHARGER");
  pinMode(CHARGER, OUTPUT);

  Serial.println("SETTING ADC");
  analogReadResolution(12);        // Default of 12 is not very linear.  Recommended to use 10 or 11 depending on needed resolution.
//  analogSetAttenuation(ADC_11db);   // Default is 11db which is very noisy.  Recommended to use 2.5 (noted "ADC_2_5db") or 6.
  /*
    ADC_0db: sets no attenuation (1V input = ADC reading of 1088).
    ADC_2_5db: sets an attenuation of 1.34 (1V input = ADC reading of 2086).
    ADC_6db: sets an attenuation of 1.5 (1V input = ADC reading of 2975).
    ADC_11db: sets an attenuation of 3.6 (1V input = ADC reading of 3959).
  */
  analogSetPinAttenuation(LineLeftFront, ADC_6db);
  analogSetPinAttenuation(LineRightFront, ADC_6db);
  analogSetPinAttenuation(PW, ADC_11db);

  connection();
  Serial.println("STARTING SERVER AND XMLHttp");
  initXMLhttp();
  server.begin();

  Serial.println("STARTING OTA CONFIG");
  OTAconfig();

  Serial.println("SETUP SUCCESSFUL");
  Blink(5, 100);
}

void OTAconfig()
{

  // Port defaults to 3232
  ArduinoOTA.setPort(3232);
  // Hostname defaults to esp3232-[MAC]
  // create OTA name
  const char *NameOTA = "KOBRA";
  ArduinoOTA.setHostname(NameOTA);
  // No authentication by default
  // ArduinoOTA.setPassword("admin");
  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
  .onStart([]() {
    OTArequest = true;

    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd");
    Blink(3, 50);
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    int a = progress / (total / 100);
    if (a >= lastProgress + 10)
    {
      Serial.println();
      Serial.printf("OTA Upload: %u%%\r", a);
      lastProgress = a;
    }
    digitalWrite(LED, !digitalRead(LED));

  })
  .onError([](ota_error_t error) {
    OTArequest = false;
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    Reset();
  });
  ArduinoOTA.begin();
}


void connection()
{
  lastWifiConnection = millis();
  WiFi.disconnect(true);
  delay(1000);

  Serial.println(F("Disabling ESP32 WiFi Access Point"));
  Serial.println(F(""));
  //WiFi.mode(WIFI_STA);

  WiFi.config(ip, gateway, subnet, dnsserver);

  Serial.println(F("Initializing ESP32 WiFi network.  Please be patient..."));
  delay(1000);

  // attempt to connect to WiFi network
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(str_ssid);
  lastSSID = 1;
  WiFi.begin(str_ssid, str_password);

  Start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - Start < 15000) {
    Serial.print(F("."));
    delay(500); // wait for connection:
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("FAILED!");
    Serial.print("TRYING TO CONNECT TO SECOND SSID: ");
    Serial.println(str_ssid2);
    lastSSID = 2;
    WiFi.begin(str_ssid2, str_password);

    Start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - Start < 15000) {
      Serial.print("- ");
      delay(500);
    }
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println(F(" ***********************************************"));
      Serial.println(F("           FAILED TO CONNECT TO ANY WIFI        "));
      Serial.println(F(" ***********************************************"));
      Serial.println(F("               EXECUTING HARD RESET             "));
      Serial.println(F(" ***********************************************"));

      esp_restart();
    }

  }
  else {
    Serial.println("");
    Serial.println("WiFi connected!");
  }


  Serial.println();
  Serial.println(F(""));
  Serial.print(F("localIP = "));
  Serial.println(WiFi.localIP());
  Serial.print(F("MAC address = "));
  Serial.println(WiFi.macAddress());

}
