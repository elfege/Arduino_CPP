// TaskHandle_t TaskA;  /// ASSIGN TASKS TO DIFFERENT ESP CORES


void setup() {

  WiFi.mode(WIFI_STA);

  // Serial.begin(115200);
  // Serial.println("Intializing DRY FOOD CAT FEEDER...");

  // xTaskCreatePinnedToCore(
  //   CPU_0_task1, /* pvTaskCode */
  //   "Workload1", /* pcName */
  //   1000,        /* usStackDepth */
  //   NULL,        /* pvParameters */
  //   0,           /* uxPriority */
  //   &TaskA,      /* pxCreatedTask */
  //   0);          /* xCoreID */

  // setup hardware pins
  Serial.println("Intializing PINS...");
  pinMode(sensor, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(feeder, OUTPUT);
  digitalWrite(feeder, LOW);
  pinMode(watchDogPin, OUTPUT);

  // pinMode(RESET_PIN, OUTPUT);    //pulled down by digitalWrite to RST board
  // digitalWrite(RESET_PIN, LOW);  //  LOW by default

  // //******************************************************************************************
  // // WIFI & http
  // //******************************************************************************************
  
  EverythingInit();  // connect wifi and start hub services
  // WiFiConnect();
  _server.begin();
  
  // Serial.println("Initializing Wifi Terminal...");
  term.begin(_server);
  term.link(Serial);  //optional : echo every print() on Serial

  initXMLhttp();
  OTAconfig();

  // //******************************************************************************************
  // // Setup the default values for the ADC.  Used for analog voltage reads.
  // //  Notes:  analogReadResolution(12) sets the resolution for all pins. 12 = 0-4095, 11 = 0-2047, 10 = 0-1024, 9 = 0-512
  // //          analogSetAttenuation(ADC_11db) sets the attenuation for all pins.  11db = 0-3.3v, 6dB range = 0-2.2v, 2.5db = 0-1.5v, 0db = 0-1v
  // //          analogSetPinAttenuation(A7, ADC_11db) sets the attenuation for individual pins.
  // //******************************************************************************************

  analogReadResolution(10);       // Default of 12 is not very linear.  Recommended to use 10 or 11 depending on needed resolution.
  analogSetAttenuation(ADC_2_5db);  // Default is 11db which is very noisy.  Recommended to use 2.5 or 6.


  // update smartthings hub
  Blink(3, 300);  // serves as delay to allow st to return backed up values
  //  Refresh();

  stopAll = false;  // main loop can start

  Serial.println("BOOT OK");
}

void OTAconfig() {

  // Port defaults to 3232
  ArduinoOTA.setPort(3232);
  // Hostname defaults to esp3232-[MAC]
  // create OTA name
  const char *NameOTA = "DRY FOOD CAT FEEDER";
  ArduinoOTA.setHostname(NameOTA);
  // No authentication by default
  // ArduinoOTA.setPassword("admin");
  // Password can be set with its md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
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

void EverythingInit() {
  Serial.println("Intializing ST EVERYTHING...");
  st::Everything::debug = true;
  st::Executor::debug = false;
  SmartThingsCallout_t messageCallout;
  st::Everything::SmartThing = new st::SmartThingsESP32WiFi_DUAL_SSID(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);  // st::receiveSmartString);
  //  st::Everything::SmartThing = new st::SmartThingsESP32_WIFI_SSID(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);
  // st::Everything::SmartThing = new st::SmartThingsESP32WiFi(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);                       // st::receiveSmartString);
  st::Everything::init();
}
