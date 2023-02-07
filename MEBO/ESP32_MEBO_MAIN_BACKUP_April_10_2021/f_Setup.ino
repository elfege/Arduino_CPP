
TaskHandle_t TaskA;/// ASSIGN TASKS TO DIFFERENT ESP CORES


void setup()
{
  Serial.begin(115200);
  term.link(Serial); //optional : echo every print() on Serial
  //******************************************************************************************
  // ASSIGN TASKS TO CORE 0
  //******************************************************************************************
  term.println("Assigning encoders tasks to CPU 0...");
  xTaskCreatePinnedToCore(
    CPU_0_task1,            /* pvTaskCode */
    "Workload1",            /* pcName */
    1000,                   /* usStackDepth */
    NULL,                   /* pvParameters */
    0,                      /* uxPriority */
    &TaskA,                 /* pxCreatedTask */
    0);                     /* xCoreID */
  pinMode(LED, OUTPUT);
  delay(1);
  digitalWrite(LED, 0); // 1 IS LED ON
  //******************************************************************************************
  // Serial
  //******************************************************************************************

  term.println("Establishing serial connection with AtMega board...");
  // com with atMega2560
  Serial2.begin(115200, SERIAL_8N1, rx2, tx2); // pins 16 rx2, 17 tx2, 115200 bps, 8 bits no parity 1 stop bit


  WiFiConnect();
  _server.begin();
  term.begin(_server);

  term.println("Booting sequence started...");

  //******************************************************************************************
  // http
  //******************************************************************************************
  term.println("starting http...");
  //EverythingInit(); // connect wifi and hub services
  initXMLhttp();
  OTAconfig();
  term.println("Connectivity done");



  //******************************************************************************************
  // Analog calibration
  //******************************************************************************************
  term.println("Calibrating analog channels. Resolution: 11, noise: ADC_6db");
  analogReadResolution(11);        // Default of 12 is not very linear.  Recommended to use 10 or 11 depending on needed resolution.
  analogSetAttenuation(ADC_6db);   // Default is 11db which is very noisy.  Recommended to use 2.5 (noted "ADC_2_5db") or 6.

  //******************************************************************************************
  // I/O
  //******************************************************************************************
  term.println("Assigning pins to main motors");
  pinMode(ENABLE_LEFT, OUTPUT); ////
  pinMode(MOTOR_LEFT1, OUTPUT);
  pinMode(MOTOR_LEFT2, OUTPUT);
  pinMode(ENABLE_RIGHT, OUTPUT);
  pinMode(MOTOR_RIGHT1, OUTPUT);
  pinMode(MOTOR_RIGHT2, OUTPUT);
  term.println("Assigning pin to charger");
  pinMode(CHARGERelay, OUTPUT);
  term.println("Assigning pin to watchdog");
  pinMode(watchDogPin, OUTPUT);
  term.println("Assigning pins to line and cliff sensors");
  pinMode(LineLeftFront, INPUT);
  pinMode(LineRightFront, INPUT);
  pinMode(LineLeftRear, INPUT);
  pinMode(LineRightRear, INPUT);
  term.println("Assigning pins to encoders");
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);


  term.println("Configuring motors channels...");
  // MOTORS CONFIGURATION
  // configure PWM functionalitites
  ledcSetup(pwmChannelL, freq, resolution);
  ledcSetup(pwmChannelR, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ENABLE_LEFT,  pwmChannelL);
  ledcAttachPin(ENABLE_RIGHT, pwmChannelR);

  //GLOWING LED CONFIGURATION
  //  ledcSetup(pwmChannelGlow, freq, resolution); // makes the whole ESP crash for some reason, to be investigated if necessary
  //  ledcAttachPin(LED,  pwmChannelGlow);

  term.println("SETUP SUCCESSFUL!");
  Blink(3, 500);
}

void WiFiConnect() {

  // Configures static IP address
  if (!WiFi.config(ip, gateway, subnet, dnsserver, secondaryDNS)) {
    term.println("STA Failed to configure");
  }

  term.println("\nConnecting to WiFi");
  WiFi.begin(ssidChar, passwordChar);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    term.print('.');
  }
  term.println("connected");
  term.print("IP ADDRESS: ");
  term.println(WiFi.localIP());
  term.print("MAC ADDRESS: ");
  term.println(WiFi.macAddress());
  term.print("SSID: ");
  term.println(WiFi.SSID());
}
void EverythingInit()
{
  st::Everything::debug = false;
  st::Executor::debug = false;
  SmartThingsCallout_t messageCallout;
  bool enableDebug = false;
  //  st::Everything::SmartThing = new st::SmartThingsESP32WiFi_DUAL_SSID(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);
  //  st::Everything::SmartThing = new st::SmartThingsESP32_WIFI_SSID(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);
  //  st::Everything::SmartThing = new st::SmartThingsESP32WiFi(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);

  //  st::Everything::init();
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
      term.println("*****************************************************************************");
      term.println("************ IR RECEIVER IS ACTIVE! RESET REQUIRED BEFORE OTA FLASH *********");
      term.println("*****************************************************************************");
      Reset();
    }
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    //SPIFFS.end();
    term.println("Start updating " + type);
  })
  .onEnd([]() {
    term.println("\nEnd");
    term.println("UPDATE SUCCESSFUL");
    Blink(3, 300);
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    if (millis() - previousMillisBlink > 1000)
    {
      term.printf("Progress: %u%%\r", (progress / (total / 100)));
      previousMillisBlink = millis();
    }
    digitalWrite(LED, !digitalRead(LED));
  })
  .onError([](ota_error_t error) {
    OTArequest = false;
    term.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) term.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) term.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) term.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) term.println("Receive Failed");
    else if (error == OTA_END_ERROR) term.println("End Failed");
    //    Reset();// no more hard reset in this function
  });
  ArduinoOTA.begin();
}
