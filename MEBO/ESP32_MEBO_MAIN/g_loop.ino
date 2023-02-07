void loop() {

  if (!OTArequest) {
    MainFunctions();
  }
  ArduinoOTA.handle();

  checkWiFi();


  delay(10);
  yield();
  elapsed = millis() - loopTime;
}
