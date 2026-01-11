void loop() {
  if (!StopAll)
  {

    master();
  }
  else
  {
    ArduinoOTA.handle(); // also handled by smartthings ESP8266 library
  }

}

void master() {
  _server.handleClient();
  smartthing.run();
  
  // refresh statuses every x minutes
  if (millis() - previousMillis > DELAY_SEC * 1000)
  {
    previousMillis = millis();
    refresh();
    userOverride = false; // reset this value
    String var = "Override Inactive";
    _server.send(200, "text/html", var);
    buildDebug(var);
  }
  if (millis() - previousMillis_override_RESET > OTHER_DELAY * 1000)
  {
    previousMillis_override_RESET = millis();
      userOverride = false; // reset this value
    String var = "Override Inactive";
    _server.send(200, "text/html", var);
    buildDebug(var);
  }
}
