void loop() {

  _server.handleClient();
  delay(1);
  smartthing.run();
  delay(1);
  ArduinoOTA.handle();

  if (digitalRead(power))
  {
    // if power is on, sync CHARGER state (from ESP32) to CHARGEin input state on this ESP8266
    digitalWrite(CHARGER, digitalRead(CHARGERin)); // charger has reverse logic
  }


  if (!digitalRead(power) && digitalRead(CHARGER)) // charger has reverse logic
  {
    if (millis() - previousMillisChargeCycle > DELAY_24)
    {
      chargeoff();
      delay(5000);
      previousMillisChargeCycle = millis();
    }
    // if power is off, turn on charger
    if (millis() - previousMillisCharge > DELAY_SEC * 1000 && digitalRead(CHARGER)) // 1 means off
    {
      chargeon();
    }
  }
  glow();

  delay(1);
}
