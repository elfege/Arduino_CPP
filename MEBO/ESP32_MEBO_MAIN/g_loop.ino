void loop() {

  if (!OTArequest) {
    MainFunctions();
    debugHandler();
  }
  ArduinoOTA.handle();

  checkWiFi();


  delay(10);
  yield();
  elapsed = millis() - loopTime;
}
