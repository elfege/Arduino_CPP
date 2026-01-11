void loop() {

  if (!StopAll)
  {
    //ESP.wdtDisable();
    
    val = analogRead(ASensor);
    Serial.println(val);

    if (val > 400 && !active) {
      // there's motion
      active = true;
      inactive = false;
      //Serial.println("MOTION ACTIVE");
      previousMillis = millis(); // count time as of active to declare inactive later
      smartthing.send("motion active");
      Blink(1, 50);
      //String var = "val " + String(val);
      //smartthing.send(var);
    }
    else if(!inactive && millis() - previousMillis > DELAY_PIR * 1000) // 30 seconds
    {
      inactive = true;
      active = false;
      Serial.println("MOTION ACTIVE");
      smartthing.send("motion inactive");
      delay(500);
      //String var = "val " + String(val);
      //smartthing.send(var);
      Blink(4, 50);
    }

    if (millis() - previousMillisRefresh > DELAY_SEC * 1000)
    {
      Refresh();
      Blink(2, 50);
      previousMillisRefresh = millis();
    }

    /*if (millis() - previousMillisBlink > 5000)
    {
      Blink(1, 50);
      previousMillisBlink = millis();
    }
    */
    smartthing.run();
    //ESP.wdtEnable(1000); // restart software watchdog
  }
  else
  {
    ArduinoOTA.handle(); // already managed by st lib but not when StopAll = true here...
  }
  delay(10);
}
