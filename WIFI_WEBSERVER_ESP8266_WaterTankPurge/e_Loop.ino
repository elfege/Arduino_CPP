void loop() {
  loopTime = millis();
  if (!StopAll)
  {
    ESP.wdtDisable();
    smartthing.run();
    _server.handleClient();
    
    if (millis() - previousMillis > DELAY_SEC * 1000)
    {
      previousMillis = millis();
      Refresh();
    }
    
    // purge the water tank/ activate pump
    if (millis() - (unsigned long)previousMillisPURGE > (unsigned long)TimeLimit && TimeLimit != 0)
    {
      previousMillisPURGE = millis();
      On();
    }
    //glow();  
    ESP.wdtEnable(1000); // restart software watchdog
  }
  else
  {
    ArduinoOTA.handle(); // also handled by smartthings ESP8266 library but it doesn't run when OTA is called due to StopAll = true
  }
  
  if(wet() && !wetTriggered)
  {
    wetTriggered = true;
    triggerMillis = millis();
    lastHwetv = analogRead(A0);
    Serial.println("wet val = "+ String(wetv));
    digitalWrite(Signal, 1); // don't call On() from here due to loop() being called within it and also as to not reset the timer
    buildDebug("switch on");
    buildDebug("water wet");
  }
  else if(!wet() && wetTriggered && millis() - triggerMillis > 30000) // don't turn it off if it's on due to other events than wet and run for at least x secs
  {
    buildDebug("water dry");
    wetTriggered = false;
    Off();
  }

  delay(10);
  //Serial.println(analogRead(A0));
  elapsed = millis() - loopTime;

}
