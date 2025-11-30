void loop() {
  loopTime = millis();
  if (!StopAll) {

    _server.handleClient();

    smartthing.run();

  
    // refresh statuses every x minutes
    if (millis() - previousMillis > DELAY_SEC * 1000)
    {
      previousMillis = millis();
      Refresh();
      Blink(1, 1000);
    }
    //glow();
  }
 
    ArduinoOTA.handle(); 
 
  elapsed = millis() - loopTime;
}
