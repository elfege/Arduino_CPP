void loop() {
  //loopTime = millis();
  if (!StopAll)
  {
    smartthing.run();
    glow();
    _server.handleClient();
    if (logs)
    {
      log();
    }

    if (millis() - requestTurboMillis > 3000 && requestTurbo && !turboIsOn)
    {
      turbo();
      buildDebug("turbo turbo");
    }
    if (millis() - stopAllRequest > 60000 && StopAll && tempRequest)
    {
      StopAll = false;
      tempRequest = false;
    }
  }


  ArduinoOTA.handle();
  delay(10);
  //elapsed = millis() - loopTime;

}
