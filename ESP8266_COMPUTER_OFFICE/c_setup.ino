void setup()
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
  // setup debug serial port
  Serial.begin(115200); // setup serial with a baud rate of 115200
  term.link(Serial);    // optional : echo every print() on Serial

  //*****************************************************************************
  // PINS CONFIGURATION
  //*****************************************************************************
  pinMode(switchPin, OUTPUT);
  pinMode(LED, OUTPUT);          // define LED as an output
  pinMode(RESET_PIN, OUTPUT);    // pulled down by digitalWrite to RST board
  digitalWrite(RESET_PIN, HIGH); //  HIGH by default

  // create OTA name before smartthings does
  const char *NameOTA = "COMPUTER_OFFICE";
  ArduinoOTA.setHostname(NameOTA);

  smartthing.init();

  initXMLhttp();
  _server.begin();

  //******************************************************************************************
  // WiFi Terminal
  //******************************************************************************************
  term.begin(_server);
  term.println("WiFi Terminal ok!");

  ArduinoOTA.onStart([]()
                     {
    term.println("Start");
    StopAll = true; });
  ArduinoOTA.onEnd([]()
                   {
    term.println("\nEnd");
    Blink(2, 1000);
    delay(2000);
    Blink(5, 100); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        {
    term.printf("Progress: %u%%\r\n", (progress / (total / 100)));
    term.handleClient();
    digitalWrite(LED, !digitalRead(LED)); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
    term.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) term.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) term.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) term.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) term.println("Receive Failed");
    else if (error == OTA_END_ERROR) term.println("End Failed"); });
  ArduinoOTA.begin();
  term.println("ArduinoOTA Ready");

  var = "SWITCH REBOOT";
  send_data(var);
  refresh();
  bootTime = millis();
}
