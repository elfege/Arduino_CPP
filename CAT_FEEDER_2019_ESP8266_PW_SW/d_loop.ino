void loop() {
  if (!StopAll)
  {
    _server.handleClient();
  }


  
  if (millis() - lastOnMillis > offTimer * 60 * 1000 && !digitalRead(power))
  {
    Serial.println("Off time is out, turning off the cat feeder"); // IMPORTANT SAFETY
    Off();
  }
  ArduinoOTA.handle();
  glow(1);
  yield();
}
