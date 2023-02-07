TaskHandle_t TaskA;/// ASSIGN TASKS TO DIFFERENT ESP CORES


void setup()
{
  // MAKE SURE TO SET stopAll to TRUE BEFORE UPLOADING !!! 

  xTaskCreatePinnedToCore(
    CPU_0_task1,                  /* pvTaskCode */
    "Workload1",            /* pcName */
    1000,                   /* usStackDepth */
    NULL,                   /* pvParameters */
    0,                      /* uxPriority */
    &TaskA,                 /* pxCreatedTask */
    0);                     /* xCoreID */

  //  timeClient.begin();
  //  timeClient.setTimeOffset(-3600 * 5);

  // setup default state of global variables
  isDebugEnabled = true;

  // setup debug serial port
  Serial.begin(115200);         // setup serial with a baud rate of 9600
  Serial.println("");
  Serial.println("setup..");  // print out 'setup..' on start

  //******************************************************************************************
  // Setup the default values for the ADC.  Used for analog voltage reads.
  //  Notes:  analogReadResolution(12) sets the resolution for all pins. 12 = 0-4095, 11 = 0-2047, 10 = 0-1024, 9 = 0-512
  //          analogSetAttenuation(ADC_11db) sets the attenuation for all pins.  11db = 0-3.3v, 6dB range = 0-2.2v, 2.5db = 0-1.5v, 0db = 0-1v
  //          analogSetPinAttenuation(A7, ADC_11db) sets the attenuation for individual pins.
  //******************************************************************************************

  analogReadResolution(10);        // Default of 12 is not very linear.  Recommended to use 10 or 11 depending on needed resolution.
  analogSetAttenuation(ADC_11db);   // Default is 11db which is very noisy.  Recommended to use 2.5 or 6.
  //*****************************************************************************
  //  Configure debug print output from each main class
  //  -Note: Set these to "false" if using Hardware Serial on pins 0 & 1
  //         to prevent communication conflicts with the ST Shield communications
  //*****************************************************************************
  st::Everything::debug = true;
  st::Executor::debug = true;
  //  st::Device::debug = true;
  //  st::PollingSensor::debug = true;
  //  st::InterruptSensor::debug = true;

  //*****************************************************************************
  //Initialize the "Everything" Class
  //*****************************************************************************

  //Initialize the optional local callback routine (safe to comment out if not desired)
  //st::Everything::callOnMsgSend = callback;

  // call out function forward decalaration
  SmartThingsCallout_t messageCallout;

  //Create the SmartThings ESP32WiFi Communications Object
  //STATIC IP Assignment - Recommended
  bool enableDebug = true;
  st::Everything::SmartThing = new st::SmartThingsESP32WiFi_DUAL_SSID(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);
//st::Everything::SmartThing = new st::SmartThingsESP32_WIFI_SSID(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);

  //st::OTA_Name::CustDeviceName(NameOTA);

  //Run the Everything class' init() routine which establishes WiFi communications with SmartThings Hub
  st::Everything::init();

  //*****************************************************************************
  //END OF "Everything" Class initialization
  //*****************************************************************************


  //******************************************************************************************
  // XML WEB SERVER INITIALIZATION
  //******************************************************************************************
  initXMLhttp();

  _server.begin();


  // setup hardware pins
  pinMode(sensor, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(feeder, OUTPUT);
  digitalWrite(feeder, LOW);
  pinMode(watchDogPin, OUTPUT);

  pinMode(RESET_PIN, OUTPUT); //pulled down by digitalWrite to RST board
  digitalWrite(RESET_PIN, LOW); //  LOW by default

  //digitalWrite(LED, LOW); // turn on the LED

  /////////// MOTORS
  // configure PWM functionalitites
  // ledcSetup(pwmChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  //  ledcAttachPin(EnablePin,  pwmChannel);

  //*****************************************************************************
  //END OF PINS CONFIGURATION
  //*****************************************************************************



  //  Serial.println("STARTING wifi WEB SERVER"); // not compatible with ST on ESP32
  //  server.begin();

  // Setup OTA Updates
  // create OTA name
  const char *NameOTA = "PETSAFE_CATFEEDER";
  ArduinoOTA.setHostname(NameOTA);

  // Port defaults to 3232
  ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]

  // No authentication by default
  // ArduinoOTA.setPassword("admin");
  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
  .onStart([]() {
    stopAll = true;
    //    timeClient.end();
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    if (millis() - previousMillisBlink > 10)
    {
      digitalWrite(LED, !digitalRead(LED));
      previousMillisBlink = millis();
    }
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();

  //*****************************************************************************
  //END OF OTA initialization
  //*****************************************************************************




  buildDebug("BOOT OK");
  // update smartthings hub
  Blink(3, 300); // serves as delay to allow st to return backed up values
  //  Refresh();

  stopAll = false; // main loop can start
}
