void setup() {

  //******************************************************************************************
  // Serial
  //******************************************************************************************
  Serial.begin(115200);
  Serial.println("Booting sequence started...");
  term.link(Serial);  //optional : echo every print() on Serial

  //******************************************************************************************
  // WIFI
  //******************************************************************************************
  WiFiConnect();
  _server.begin();
  term.begin(_server);

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
  analogReadResolution(11);       // Default of 12 is not very linear.  Recommended to use 10 or 11 depending on needed resolution.
  analogSetAttenuation(ADC_6db);  // Default is 11db which is very noisy.  Recommended to use 2.5 (noted "ADC_2_5db") or 6.

  //******************************************************************************************
  // I/O
  //******************************************************************************************



#define US1_trig 16
#define US1_echo 17

#define US2_trig 26
#define US2_echo 25

#define IRsensor1 39

  term.println("Assigning pins");
  pinMode(LED, OUTPUT);
  pinMode(ACTUATOR_A, OUTPUT);
  pinMode(ACTUATOR_B, OUTPUT);
  pinMode(ACTR_ENABLE_PWM, OUTPUT);
  pinMode(OPENER_A, OUTPUT);
  pinMode(OPENER_B, OUTPUT);
  pinMode(OPN_ENABLE_PWM, OUTPUT);

  pinMode(US1_trig, OUTPUT);
  pinMode(US1_echo, INPUT);
  pinMode(US2_trig, OUTPUT);
  pinMode(US2_echo, INPUT);

  pinMode(IRsensor1, INPUT);

  digitalWrite(LED, 0);  // 1 IS LED ON

  // MOTORS CONFIGURATION
  // configure PWM functionalitites
  ledcSetup(pwmChannelL, freq, resolution);
  ledcSetup(pwmChannelR, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ACTR_ENABLE_PWM, pwmChannelL);
  ledcAttachPin(OPN_ENABLE_PWM, pwmChannelR);

  term.println("connected");
  term.print("IP ADDRESS: ");
  term.println(WiFi.localIP());
  term.print("MAC ADDRESS: ");
  term.println(WiFi.macAddress());
  term.print("SSID: ");
  term.println("SSID: " + String(WiFi.SSID()));
  term.println("RSSI: " + String(WiFi.RSSI()));
  term.println("SETUP SUCCESSFUL!");
  Blink(3, 500);
}

void EverythingInit() {
//  st::Everything::debug = false;
//  st::Executor::debug = false;
//  SmartThingsCallout_t messageCallout;
  //  st::Everything::SmartThing = new st::SmartThingsESP32WiFi_DUAL_SSID(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);
  //  st::Everything::SmartThing = new st::SmartThingsESP32_WIFI_SSID(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);
  //  st::Everything::SmartThing = new st::SmartThingsESP32WiFi(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);

  //  st::Everything::init();
}

void OTAconfig() {

  // Port defaults to 3232
  ArduinoOTA.setPort(3232);
  // Hostname defaults to esp3232-[MAC]
  // create OTA name
  const char *NameOTA = "CAT_FEEDER_3";
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
      term.println("UPDATE STARTED... Please, wait...");
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else  // U_SPIFFS
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
      term.printf("Progress: %u%%\r", (progress / (total / 100)));
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
