void loop() {
  //loopTime = millis();
  if (!StopAll)
  {
    smartthing.run();
    _server.handleClient();
    currentState = closed() ? "closed" : "open";

    // when the TV shuts itself off due to power saving mode, update smart hub's device state
    if (millis() - previousMillis > 2000) // check every 2 seconds to avoid data overflow 
    {
      if (lastState != currentState)
      {
        _server.send(200, "text/html", currentState);
        buildDebug("contact " + currentState);
        lastState = currentState;
      }
      previousMillis = millis();

      Serial.println(analogRead(Asensor));
    }

    if (millis() - previousMillisRefresh > 5 * 60 * 1000) // refresh every 5 minutes
    {
      Refresh();
      previousMillisRefresh = millis();
    }

  }

  ArduinoOTA.handle();



  delay(1);
  yield();
  //Serial.println(counter);
  //elapsed = millis() - loopTime;


}
