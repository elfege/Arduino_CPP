void loop() {
  if (!StopAll)
  {

    _server.handleClient();

    smartthing.run();
    // refresh statuses every x minutes
    if (millis() - previousMillis > DELAY_SEC * 1000)
    {
      previousMillis = millis();
      Refresh();
      ignorecommand = false; // reset this value
    }
    if (millis() - previousMillis_IGNORE_RESET > OTHER_DELAY * 1000)
    {
      previousMillis_IGNORE_RESET = millis();
      ignorecommand = false; // reset this value
    }
  }
  else
  {
    ArduinoOTA.handle(); // also handled by smartthings ESP8266 library
  }

}
