
IRrecv irrecv(RECV_PIN);
decode_results results;

TaskHandle_t TaskA;/// ASSIGN TASKS TO DIFFERENT ESP CORES

void setup()

{


  /// ASSIGN TASKS TO DIFFERENT ESP CORES
  xTaskCreatePinnedToCore(
    Task1,                  /* pvTaskCode */
    "Workload1",            /* pcName */
    1000,                   /* usStackDepth */
    NULL,                   /* pvParameters */
    0,                      /* uxPriority */
    &TaskA,                 /* pxCreatedTask */
    0);                     /* xCoreID */

  // com with atMega2560
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // pins 16 rx2, 17 tx2, 19200 bps, 8 bits no parity 1 stop bit

  Serial.begin(115000);

  pinMode(RELAY2, OUTPUT); // priority to this relay in case loss of power to resume charge
  digitalWrite(RELAY2, 0);// which means that it is by default ON upon reset


  //******************************************************************************************
  // Setup the default values for the ADC.  Used for analog voltage reads.
  //  Notes:  analogReadResolution(12) sets the resolution for all pins. 12 = 0-4095, 11 = 0-2047, 10 = 0-1024, 9 = 0-512
  //          analogSetAttenuation(ADC_11db) sets the attenuation for all pins.  11db = 0-3.3v, 6dB range = 0-2.2v, 2.5db = 0-1.5v, 0db = 0-1v
  //          analogSetPinAttenuation(A7, ADC_11db) sets the attenuation for individual pins.
  //******************************************************************************************

  analogReadResolution(11);        // Default of 12 is not very linear.  Recommended to use 10 or 11 depending on needed resolution.
  analogSetAttenuation(ADC_6db);   // Default is 11db which is very noisy.  Recommended to use 2.5 or 6.


  //pinMode(LED, OUTPUT); in conflict with trigger pin Ultra Sound


  Serial.println("Enabling IRin");
  //irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");



  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  
  pinMode(RST, OUTPUT); // reset pin
  digitalWrite(RST, LOW);
  

  WiFi.begin(ssid, password);

  int cW = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    cW++;
    if (cW > 50) {
      Serial.println("");
      Serial.print("connection failed, restarting ESP");
      delay(1500);
      Reset();
    }
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  server.begin();


  // set the pins outputs
  pinMode(RELAY1, OUTPUT);
  digitalWrite(RELAY1, 0);
  //pinMode(PW, INPUT); /// charging power measurment

  pinMode(ENABLE_LEFT, OUTPUT); ////
  pinMode(PWM_LEFT1, OUTPUT);
  pinMode(PWM_LEFT2, OUTPUT);
  pinMode(ENABLE_RIGHT, OUTPUT);
  pinMode(PWM_RIGHT1, OUTPUT);
  pinMode(PWM_RIGHT2, OUTPUT);

  /// configure encoder pins
  pinMode(25, INPUT);

  //// ENCODERS

  pinMode(RIGHT, INPUT);
  pinMode(LEFT, INPUT);
  //  attachInterrupt(digitalPinToInterrupt(LEFT), counterL, FALLING);
  //  attachInterrupt(digitalPinToInterrupt(RIGHT), counterR, FALLING);

  /////////// MOTORS
  // configure PWM functionalitites
  ledcSetup(pwmChannelL, freq, resolution);
  ledcSetup(pwmChannelR, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ENABLE_LEFT,  pwmChannelL);
  ledcAttachPin(ENABLE_RIGHT, pwmChannelR);


  //irrecv.enableIRIn(); // Start the receiver ---------------------------------------------------------------------------------------------------------------------


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
  Serial.println("READY.. ");
}






