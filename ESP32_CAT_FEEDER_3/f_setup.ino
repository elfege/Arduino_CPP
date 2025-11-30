void setup() {
  pinMode(LED, OUTPUT);
  delay(1);
  Blink(3, 50);



  //******************************************************************************************
  // WIFI & http
  //******************************************************************************************
  EverythingInit();  // connect wifi and start hub services
  // WiFiConnect();
  _server.begin();
  term.begin(_server);
  initXMLhttp();
  OTAconfig();


  //******************************************************************************************
  // Serial
  //******************************************************************************************
  // Serial.begin(115200); // NOT WITH ST INITIALIZED!
  // Serial.println("Booting sequence started...");
  term.link(Serial);  //optional : echo every print() on Serial


  //******************************************************************************************
  // Analog calibration
  //******************************************************************************************
  //******************************************************************************************
  // Setup the default values for the ADC.  Used for analog voltage reads.
  //  Notes:  analogReadResolution(12) sets the resolution for all pins. 12 = 0-4095, 11 = 0-2047, 10 = 0-1024, 9 = 0-512
  //          analogSetAttenuation(ADC_11db) sets the attenuation for all pins.  11db = 0-3.3v, 6dB range = 0-2.2v, 2.5db = 0-1.5v, 0db = 0-1v
  //          analogSetPinAttenuation(A7, ADC_11db) sets the attenuation for individual pins.
  //******************************************************************************************
  term.println("Calibrating analog channels. Resolution: 11, noise: ADC_6db");
  analogReadResolution(11);       // Default of 12 is not very linear.  Recommended to use 10 or 11 depending on needed resolution.
  analogSetAttenuation(ADC_6db);  // Default is 11db which is very noisy.  Recommended to use 2.5 (noted "ADC_2_5db") or 6.

  //******************************************************************************************
  // I/O
  //******************************************************************************************
  term.println("Assigning pins");

  pinMode(ACTUATOR_A, OUTPUT);
  pinMode(ACTUATOR_B, OUTPUT);
  pinMode(ACTR_ENABLE_PWM, OUTPUT);
  pinMode(OPENER_A, OUTPUT);
  pinMode(OPENER_B, OUTPUT);
  pinMode(OPEN_ENABLE_PWM, OUTPUT);

  // pinMode(SMALL_PUSHER_A, OUTPUT);
  // pinMode(SMALL_PUSHER_B, OUTPUT);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  SMALL_PUSHER.setPeriodHertz(50);  // Standard 50hz servo


  pinMode(vertPush_A, OUTPUT);
  pinMode(vertPush_B, OUTPUT);
  pinMode(wheel, INPUT_PULLUP);

  pinMode(COSINUS_A, OUTPUT);
  pinMode(COSINUS_B, OUTPUT);
  pinMode(COSINUS_PWM, OUTPUT);


  pinMode(IR1_pile, INPUT);
  pinMode(IR2_pile, INPUT);

  pinMode(candown, INPUT);  // connected to a pulldown resistor

  pinMode(lockSensor, INPUT_PULLUP);
  pinMode(canSensor, INPUT_PULLUP);
  pinMode(eatingZoneSensor, INPUT_PULLUP);
  pinMode(pusherRetract, INPUT_PULLUP);

  pinMode(deformSensor, INPUT);
  // pinMode(unusedAnalog, INPUT);



  // MOTORS CONFIGURATION
  // configure PWM functionalitites
  ledcSetup(pwmChannelActuator, freq, resolution);
  ledcSetup(pwmChannelCanOpener, freq, resolution);
  ledcSetup(pwmChannelCos, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ACTR_ENABLE_PWM, pwmChannelActuator);
  ledcAttachPin(OPEN_ENABLE_PWM, pwmChannelCanOpener);
  ledcAttachPin(COSINUS_PWM, pwmChannelCos);

  term.println("connected");
  term.print("IP ADDRESS: ");
  term.println(WiFi.localIP());
  term.print("MAC ADDRESS: ");
  term.println(WiFi.macAddress());
  term.print("SSID: ");
  term.println("SSID: " + String(WiFi.SSID()));
  term.println("RSSI: " + String(WiFi.RSSI()));
  term.println("SETUP SUCCESSFUL!");
  
  // unlock();        // in case it's holding a can, must be unlocked before pulling up... otherwise this could make a big big mess! :) .. problematic when loses WiFi runs over and over when lock sensor fails.
  verticalPull();  // needed to initialize its boolean
  cosinusPull();
  smallPull();  // needed to initialize its boolean

  simpleStop();

  term.println("CAT FEEDER IS READY");
  Blink(3, 250);
}

void EverythingInit() {
  st::Everything::debug = false;
  st::Executor::debug = false;
  SmartThingsCallout_t messageCallout;
  st::Everything::SmartThing = new st::SmartThingsESP32WiFi_DUAL_SSID(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);  // st::receiveSmartString);
  //  st::Everything::SmartThing = new st::SmartThingsESP32_WIFI_SSID(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);
  // st::Everything::SmartThing = new st::SmartThingsESP32WiFi(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);                       // st::receiveSmartString);
  st::Everything::init();
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
      readyOTA = true;
      stopAll = true; 
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
      term.printf("Progress: %u%%\r", (progress / (total / 100)), "\n");
      digitalWrite(LED, !digitalRead(LED));
      term.handleClient();
    })
    .onError([](ota_error_t error) {
      readyOTA = false;
      term.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) term.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) term.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) term.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) term.println("Receive Failed");
      else if (error == OTA_END_ERROR) term.println("End Failed");
      //    Reset();// no more hard reset in this function
    });
  ArduinoOTA.begin();

  // create a FreeRTOS task
  // xTaskCreate(OTAHandleTask, "OTA_Handle", 4096, NULL, tskIDLE_PRIORITY + 1, NULL);
}