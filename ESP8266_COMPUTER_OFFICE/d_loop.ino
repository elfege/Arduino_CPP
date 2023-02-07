void loop() {
  if (!StopAll)
  {
    master();
  }
  else
  {
    ArduinoOTA.handle(); // also handled by smartthings ESP8266 library
  }
  if(millis() - millisHeartBit >= 1000)
  {
//    Serial.println("state = "+lastState);
    millisHeartBit = millis();
    digitalWrite(LED, !digitalRead(LED)); 
  }
  yield();
  delay(10);
}

void master() {
  servers();
  
  if (!isOn() && lastState == "on")
  {
    Serial.println("UPDATE 1");
    lastState = "off";
    buildDebug("switch off");
    buildDebug("computer sleeping");
    buildDebug("status sleeping");
  }
  if (isOn() && lastState == "off")
  {
    Serial.println("UPDATE 2");
    lastState = "on";
    buildDebug("switch on");
    buildDebug("computer running");
    buildDebug("status running");
  }

  // refresh statuses every x minutes
  if (millis() - previousMillis > DELAY_SEC * 1000)
  {
    previousMillis = millis();
    refresh();
    userOverride = false; // reset this value
    String var = "Override Inactive";
    _server.send(200, "text/html", var);
    buildDebug(var);
  }
  if (millis() - previousMillis_override_RESET > OTHER_DELAY * 1000)
  {
    previousMillis_override_RESET = millis();
    userOverride = false; // reset this value
    String var = "Override Inactive";
    _server.send(200, "text/html", var);
    buildDebug(var);
  }
}

void servers()
{
  _server.handleClient();
  smartthing.run();
}
