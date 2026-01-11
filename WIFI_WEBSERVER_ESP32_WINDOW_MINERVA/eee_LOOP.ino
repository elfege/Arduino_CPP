void loop() {
  // run smartthings routine

  ArduinoOTA.handle();

  st::Everything::run();


  // refresh statuses every x minutes
  if (millis() - previousMillis > DELAY_SEC * 5000)
  {
    previousMillis = millis();
    Refresh();
    Blink(5, 50);
    Serial.println("TEST");

    Serial.println("CONNECTION TEST");
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("WIFI DISCONNECTED. Waiting to see if library can fix it..");
      delay(500); // wait for connection:
      if (WiFi.status() != WL_CONNECTED) {
        Serial.print("WIFI STILL DISCONNECTED. Hard Reseting board");
        delay(500);
        Reset(); // this is a hard reset on RST port.
      }
      else {
        Serial.print("WIFI connection restored..");
      }
    }
    else {
      Serial.print("Connection is healthy");
    }

  }
  if (millis() - previousMillisBlink > 1000)
  {
    previousMillisBlink = millis();
    Blink(2, 200);

  }

  //webserver();
  //Serial.println("loop");

}

