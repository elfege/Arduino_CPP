void loop() {

  smartthing.run();
  

  val = analogRead(ASensor);

  if (val > 800) {
    // there's motion

    previousMillis = millis(); // reset timer

    if (millis() - previousMillisSinceLastActive > 5000) { // don't send active more than once every 5 seconds
      Serial.println("MOTION ACTIVE");
      smartthing.send("motion active");
      Blink(2, 50);
      previousMillisSinceLastActive = millis();
    }


    active = true;
  }
  else {
    webserver(); // run only when motion is not active
  }

  if (millis() - previousMillis > DELAY_PIR * 1000 && val < 800)
  {
    Serial.println(val);
    // no motion, if it was active, send event

    Serial.println("motion inactive");
    if (active) { // make sure to send this only if currently active
      smartthing.send("motion inactive");
    }
    active = false;
    previousMillis = millis();
    //
  }

  if (millis() - previousMillisRefresh > DELAY_SEC * 1000)
  {
    Refresh();
    Blink(2, 50);
    previousMillisRefresh = millis();
  }

  if (millis() - previousMillisBlink > 5000)
  {
    Blink(1, 50);
    previousMillisBlink = millis();
  }

  yield();
  delay(100);
  webserver();
}





