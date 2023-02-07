void setup()
{

  //*****************************************************************************
  //PINS CONFIGURATION
  //*****************************************************************************
  pinMode(LED,        OUTPUT);
  pinMode(modeSw,     OUTPUT);
  pinMode(lowerTemp,  OUTPUT);
  pinMode(power,      OUTPUT);
  pinMode(raiseTemp,  OUTPUT);
  pinMode(fanSpeed,      OUTPUT);

  digitalWrite(LED, 0);
  digitalWrite(modeSw, 1);
  digitalWrite(lowerTemp, 1);
  digitalWrite(power, 1);
  digitalWrite(raiseTemp, 1);
  digitalWrite(fanSpeed, 1);


  pinMode(cooling,    INPUT_PULLUP );
  pinMode(dehumid,    INPUT );
  pinMode(fanonly,    INPUT );
  pinMode(fanlow,     INPUT );
  pinMode(fanmed,     INPUT );
  pinMode(fanhigh,    INPUT );
  pinMode(fanauto,    INPUT );



  //******************************************************************************************
  // Setup the default values for the ADC.  Used for analog voltage reads.
  //  Notes:  analogReadResolution(12) sets the resolution for all pins. 12 = 0-4095, 11 = 0-2047, 10 = 0-1024, 9 = 0-512
  //          analogSetAttenuation(ADC_11db) sets the attenuation for all pins.  11db = 0-3.3v, 6dB range = 0-2.2v, 2.5db = 0-1.5v, 0db = 0-1v
  //          analogSetPinAttenuation(A7, ADC_11db) sets the attenuation for individual pins.
  //******************************************************************************************

  analogReadResolution(10);        // Default of 12 is not very linear.  Recommended to use 10 or 11 depending on needed resolution.
  analogSetAttenuation(ADC_2_5db);   // Default is 11db which is very noisy.  Recommended to use 2.5 or 6.


  //*****************************************************************************
  //Initialize the "Everything" Class
  //*****************************************************************************
  //*****************************************************************************
  //  Configure debug print output from each main class
  //  -Note: Set these to "false" if using Hardware Serial on pins 0 & 1
  //         to prevent communication conflicts with the ST Shield communications
  //*****************************************************************************
  st::Everything::debug = true;
  st::Executor::debug = true;
  st::Everything::debug = true;
  //  st::PollingSensor::debug = true;
  //  st::InterruptSensor::debug = true;
  //call out function forward decalaration
  SmartThingsCallout_t messageCallout;
  bool enableDebug = true;
  //Create the SmartThings ESP32WiFi Communications Object
  //  st::Everything::SmartThing = new st::SmartThingsESP32WiFi_DUAL_SSID(str_ssid, str_ssid2, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);
  //    st::Everything::SmartThing = new st::SmartThingsESP32WiFi(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);// st::receiveSmartString);
  st::Everything::SmartThing = new st::SmartThingsESP32WiFi(str_ssid, str_password, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, st::receiveSmartString);

  //Run the Everything class' init() routine which establishes WiFi communications with SmartThings Hub
  st::Everything::init();

  //******************************************************************************************
  // WiFi Terminal
  //******************************************************************************************
  term.begin(_server);
  term.println("WiFi Terminal ok!");

  //******************************************************************************************
  // XML WEB SERVER INITIALIZATION
  //******************************************************************************************
  initXMLhttp();
  _server.begin();



  //*****************************************************************************
  //OTA initialization
  //*****************************************************************************
  const char *NameOTA = "AC_MINERVA";
  ArduinoOTA.setHostname(NameOTA);

  // Port defaults to 3232
  ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // No authentication by default
  // ArduinoOTA.setPassword("admin");
  // Password can be set with its md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
  .onStart([]() {
    OTAstart = millis();
    stopAll = true;
    //    timeClient.end();
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    term.println("Start updating " + type);
  })
  .onEnd([]() {
    term.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    if (millis() - OTAstart > 1000) {
      term.printf("Progress: %u%%\r", (progress / (total / 100)));
      term.println();
    }
    digitalWrite(LED, !digitalRead(LED));
  })
  .onError([](ota_error_t error) {
    term.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) term.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) term.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) term.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) term.println("Receive Failed");
    else if (error == OTA_END_ERROR) term.println("End Failed");
    ESP.restart();
  });

  ArduinoOTA.begin();


  // setup debug serial port
  //  Serial.begin(115200); // DO NOT RUN THIS... CONFLICTS WITH STAnything lib that already initializes Serial. (since last ESP update, used to not be a problem)
  //  term.println("");
  //  term.println("setup..");
  term.link(Serial); //optional : echo every print() on Serial

  term.println("SETUP OK...");
}
