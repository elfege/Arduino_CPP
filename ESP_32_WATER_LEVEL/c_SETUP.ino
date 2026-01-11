void setup()
{

  //******************************************************************************************
  // WIFI & http
  //******************************************************************************************
  EverythingInit(); // connect wifi and start hub services
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
  term.link(Serial); // optional : echo every print() on Serial

  //*****************************************************************************
  // PINS CONFIGURATION (analog settings must be set after wifi config since they work on the same stack)
  //*****************************************************************************
  term.println("Assigning pins");
  pinMode(TEMP_SENSOR_PIN, INPUT); // Set the temperature sensor pin as input
  pinMode(sensor, INPUT_PULLUP);   // set the water level sensor pin as input
  pinMode(LED, OUTPUT);            // set the LED port as output

  //*****************************************************************************
  // ADC CONFIGURATION
  //*****************************************************************************
  analogSetWidth(12);             // Set ADC resolution to 12 bits
  analogSetAttenuation(ADC_11db); // Set attenuation for 3.3V max voltage
}

void EverythingInit()
{
  st::Everything::debug = false;
  st::Executor::debug = false;
  SmartThingsCallout_t messageCallout;
  st::Everything::SmartThing = new st::SmartThingsESP32WiFi_DUAL_SSID(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout); // st::receiveSmartString);
  //  st::Everything::SmartThing = new st::SmartThingsESP32_WIFI_SSID(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);
  // st::Everything::SmartThing = new st::SmartThingsESP32WiFi(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);                       // st::receiveSmartString);
  st::Everything::init();
}

void OTAconfig()
{

  // Port defaults to 3232
  ArduinoOTA.setPort(3232);
  // Hostname defaults to esp3232-[MAC]
  // create OTA name
  const char *NameOTA = "WATER_LEVEL_POOL";
  ArduinoOTA.setHostname(NameOTA);
  // No authentication by default
  // ArduinoOTA.setPassword("admin");
  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
      .onStart([]()
               {
      stopAll = true;
      term.println("UPDATE STARTED... Please, wait...");
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else  // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      //SPIFFS.end();
      term.println("Start updating " + type); })
      .onEnd([]()
             {
      term.println("\nEnd");
      term.println("UPDATE SUCCESSFUL"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  {
      term.printf("Progress: %u%%\r", (progress / (total / 100)), "\n");
      term.println();
      digitalWrite(LED, !digitalRead(LED));
      term.handleClient(); })
      .onError([](ota_error_t error)
               {
                 OTArequest = false;
                 term.printf("Error[%u]: ", error);
                 if (error == OTA_AUTH_ERROR)
                   term.println("Auth Failed");
                 else if (error == OTA_BEGIN_ERROR)
                   term.println("Begin Failed");
                 else if (error == OTA_CONNECT_ERROR)
                   term.println("Connect Failed");
                 else if (error == OTA_RECEIVE_ERROR)
                   term.println("Receive Failed");
                 else if (error == OTA_END_ERROR)
                   term.println("End Failed");
                 //    Reset();// no more hard reset in this function
               });
  ArduinoOTA.begin();
}