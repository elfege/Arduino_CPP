void sendNTPpacket(IPAddress& address); // prototype required to avoid havint the void declaration error message...

void loop() {

  loopTime = millis();
  // ESP.wdtDisable();

  if (!StopAll)
  {

    boolean shortDelay = millis() - previousMillis > DELAY_SHORT * 1000;
    boolean longDelay = millis() - previousMillis > DELAY_PIR * 1000;

    if (motion() && !active) {
      c++;
      active = true;
      inactive = false;
      Serial.println("MOTION ACTIVE");
      previousMillis = millis();

      String var = "motion active";
      buildDebug(var);
      //Blink(1, 100);
    }
    else if (shortDelay && !inactive && !motion())
      /**** report inactive only if:
        -there's  been no motion for the past couple minutes, refresh the HUB // DEPRECATED! Messes with eventSince() in groovy
        -No motion for the past N seconds after last active motion event
      ****/
    {
      c = 0;
      inactive = true;
      active = false;
      Serial.println("MOTION INACTIVE");
      previousMillis = millis();
      String var = "motion inactive";
      buildDebug(var);
      //Blink(1, 100);
    }

    smartthing.run();
    _server.handleClient();
  }
  else
  {
    ArduinoOTA.handle(); // already managed by st lib but not when StopAll = true here...
  }
  if (glowDelay < 1)
  {
    delay(10);
  }
  else
  {
    glow(glowDelay); // light from this LED must be prevented from reaching the sensor otherwise we get false positives
  }
  if (justbooted)
  {
    preserved = "LASTBOOT: " + String(ntpRequest());
    buildDebug(preserved);
    justbooted = false;
  }
  //ESP.wdtEnable(1000); // restart software watchdog // Crashes everything when smartthings is unresponsive
  elapsed = millis() - loopTime;
}


String ntpRequest()
{
  timeClient.update();

  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());

  String var = "";
  var.concat(String(daysOfTheWeek[timeClient.getDay()]) + " at ");
  var.concat(String(timeClient.getHours()) + ":");
  var.concat(String(timeClient.getMinutes()) + ":");
  var.concat(String(timeClient.getSeconds()));


  if (var.length() > 1)
  {
    return var;
  }
  else
  {
    var = String(TimingInfos());
    return var;
  }
}
