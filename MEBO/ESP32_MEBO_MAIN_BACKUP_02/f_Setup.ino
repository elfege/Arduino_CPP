

IRrecv IRAM_ATTR irrecv(RECV_PIN);
decode_results results;

TaskHandle_t TaskA;/// ASSIGN TASKS TO DIFFERENT ESP CORES


void setup()

{
  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);

  /// ASSIGN TASKS TO DIFFERENT ESP CORES
  xTaskCreatePinnedToCore(
    CPU_0_task1,                  /* pvTaskCode */ 
    "Workload1",            /* pcName */
    1000,                   /* usStackDepth */
    NULL,                   /* pvParameters */
    0,                      /* uxPriority */
    &TaskA,                 /* pxCreatedTask */ 
    0);                     /* xCoreID */

  // com with atMega2560
  Serial2.begin(115200, SERIAL_8N1, rx2, tx2); // pins 16 rx2, 17 tx2, 19200 bps, 8 bits no parity 1 stop bit

  Serial.begin(115000);

  //******************************************************************************************
  // Setup the default values for the ADC.  Used for analog voltage reads.
  //  Notes:  analogReadResolution(12) sets the resolution for all pins. 12 = 0-4095, 11 = 0-2047, 10 = 0-1024, 9 = 0-512
  //          analogSetAttenuation(ADC_11db) sets the attenuation for all pins.  11db = 0-3.3v, 6dB range = 0-2.2v, 2.5db = 0-1.5v, 0db = 0-1v
  //          analogSetPinAttenuation(A7, ADC_11db) sets the attenuation for individual pins.
  //******************************************************************************************

  analogReadResolution(11);        // Default of 12 is not very linear.  Recommended to use 10 or 11 depending on needed resolution.
  analogSetAttenuation(ADC_6db);   // Default is 11db which is very noisy.  Recommended to use 2.5 or 6.

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

  //st::OTA_Name::CustDeviceName(NameOTA);

  //Run the Everything class' init() routine which establishes WiFi communications with SmartThings Hub
  st::Everything::init();

  //*****************************************************************************
  //END OF I"Everything" Class initialization
  //*****************************************************************************


  //******************************************************************************************
  // IR RECEIVER CONFIGURATION
  //******************************************************************************************
  Serial.println("Enabling IRin");
  //irrecv.enableIRIn(); // Start the receiver // not here, only upon requests because this disables ArduinoOTA
  Serial.println("IRin Enabled");


  //******************************************************************************************
  // WEB SERVER INITIALIZATION
  //******************************************************************************************
  initJava();

  _server.begin();
  
//  _server.begin();


  //******************************************************************************************
  // PINS CONFIGURATION
  //******************************************************************************************

  pinMode(ENABLE_LEFT, OUTPUT); ////
  pinMode(MOTOR_LEFT1, OUTPUT);
  pinMode(MOTOR_LEFT2, OUTPUT);
  pinMode(ENABLE_RIGHT, OUTPUT);
  pinMode(MOTOR_RIGHT1, OUTPUT);
  pinMode(MOTOR_RIGHT2, OUTPUT);
  pinMode(CamAndArmsRelay, OUTPUT);
  pinMode(CHARGERelay, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);

  pinMode(LineLeft, INPUT);
  pinMode(LineRight, INPUT);
  

  pinMode(PW, INPUT); /// charging power measurment


  /// configure encoder pins
  pinMode(RIGHT, INPUT);
  pinMode(LEFT, INPUT);
  //  attachInterrupt(digitalPinToInterrupt(LEFT), counterL, FALLING);
  //  attachInterrupt(digitalPinToInterrupt(RIGHT), counterR, FALLING);

  //******************************************************************************************
  // MOTORS CONFIGURATION
  //******************************************************************************************
  // configure PWM functionalitites
  ledcSetup(pwmChannelL, freq, resolution);
  ledcSetup(pwmChannelR, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ENABLE_LEFT,  pwmChannelL);
  ledcAttachPin(ENABLE_RIGHT, pwmChannelR);


  //******************************************************************************************
  // OTA..
  //******************************************************************************************

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
    Serial2.end();
    OTArequest = true;
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
    delay(1000);
    Reset();
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.println();
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
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

  Serial.println("ESP32 READY.. ");
  Blink(5, 50);
}
