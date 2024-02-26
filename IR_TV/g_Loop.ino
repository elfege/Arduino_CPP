void loop() {
  loopTime = millis();
  if (!StopAll)
  {
    
    http_handle();    
    currentState = tvIsOn() ? "on" : "off";

    // when the TV shuts itself off due to power saving mode, update smart hub's device state
    if (millis() - previousMillis > 1500) // check every xxx millis
    {
      if (lastRequestedState != currentState)
      {
        msgState = "switch " + currentState;
        sendData(msgState);
        lastRequestedState = currentState;
      }
      previousMillis = millis();
    }

    if (millis() - previousMillisRefresh > 5 * 60 * 1000) // check every 2 seconds to avoid data overflow and other complications due to IR delays
    {
      Refresh();
      previousMillisRefresh = millis();
    }
  }

  ArduinoOTA.handle();

  delay(10);
  yield();


}

void http_handle(){
  // ESP.wdtDisable();
    smartthing.run();
    //glow();
    _server.handleClient();
    //lastRequestedState =

    term.handleClient();
}
