void loop()
{
  // loopTime = millis();

  if (!StopAll)
  {
    master();
  }
  else
  {
    // user requested interruption of all activity
    ArduinoOTA.handle();
  }
  if (millis() - stopAllRequest > 60000 && StopAll && tempRequest) // reset the temporary hold
  {
    StopAll = false;
    tempRequest = false;
  }
  yield();
  delay(10);
}

void master()
{
  restoreSettingsAfterBoot();
  serverAndHub();
  getOutdoorTemperature();

  if (millis() - lastNTP > 1 * 60 * 60 * 1000 || ntpFailed && !stopNtp)
  {
    updateNtpTime();
    lastNTP = millis();
  }
  else if (millis() - lastNTP > 1 * 60 * 60 * 1000 && stopNtp)
  {
    term.println("NTP FAILED - USER MUST RESET ESP");
    lastNTP = millis();
    stopNtp = true;
    // don't automate reset for this; if DNS are down this controller would not stop resetting...
  }
  if (logs)
  {
    log();
  }

  if (millis() - previousMillis > 600000)
  {
    Refresh();
    previousMillis = millis();
  }
  if (lastMode != currentMode)
  {
    term.println("new mode:" + String(currentMode));
    lastMode = currentMode;
    Refresh();
  } /******/

  // make sure boost temp is set, hubs' driver might have messed that up...
  boolean boostTempNeeded = millis() - lastModeRequest > 30000 && lastSetpointRequested != requiredSetpointForBoost && currentMode != "AUTO" && currentMode != "OFF";
  // must be long delay to prevent overlapp which, otherwise, triggers an exception 28:Exception 28: LoadProhibited: A load referenced a page mapped with an attribute that does not permit loads
  /*********************************DEPRECATED SINCE PURECOOL/PUREHEAT********************************/
  /*********************************DEPRECATED SINCE PURECOOL/PUREHEAT********************************/
  /*********************************DEPRECATED SINCE PURECOOL/PUREHEAT********************************/
  /*if (boostTempNeeded)
    {
    term.println("boostTempNeeded = " + String(boostTempNeeded ? "true" : "false"));
    lastSetpointRequested = requiredSetpointForBoost;
    indexArray = getIndex(requiredSetpointForBoost); // set to a lower default value
    newTemp = TempArray[indexArray - 1][99];
    term.println(String(requiredSetpointForBoost) +" SENT FROM LOOP");
    SetTemp(newTemp, "");// this won't send a coolingSetpoint event
    }*/

  if (millis() - requestTurboMillis > 3000 && !turboIsOn && currentMode != "OFF")
  {
    term.println("turbo request");
    turbo();
  }
}
void serverAndHub()
{

  if (millis() - previousMillisBlink >= 10000)
  {
    previousMillisBlink = millis();
    digitalWrite(LED, 0);
    delay(10);
    digitalWrite(LED, 1);
  }
  smartthing.run();
  _server.handleClient();
  term.handleClient(); // WiFi terminal
  //  termSteps();
  state.saveToRTC(); // there are no parameters because it only needs the vars' definiton once
  yield();
}
void restoreSettingsAfterBoot()
{

  if (millis() - millisBoot < 30000 && !bootRestore)
  {
    bootRestore = true;
    term.println("lastOperation 3 seconds after reboot = " + (String)lastOperation);
    String lastOpStr = lastOperation == 1   ? "cool"
                       : lastOperation == 2 ? "heat"
                       : lastOperation == 3 ? "auto"
                       : lastOperation == 4 ? "undefined"
                       : lastOperation == 5 ? "fanOnly"
                                            : "undefined";
    term.println("lastOpStr = " + lastOpStr);
    term.println("Restoring last recorded operation lastOperation = " + String(lastOpStr));
    if (lastOperation == 0)
    {
      if (currentMode != "OFF")
      {
        offRequestMillis = millis(); // allow fan to circulate for X minutes after turning off to prevent mold formation
      }
      acOff();
    }
    else if (lastOperation == 1)
    {
      cool();
    }
    else if (lastOperation == 2)
    {
      heat();
    }
    else if (lastOperation == 3)
    {
      // Auto(); // auto is used as override in thermostat hub app so don't send the command so as to not change user's pref (which what the override is meant for...).
      send_to_hub("thermostatMode auto"); // only update the current thermostat mode.
    }
    else if (lastOperation == 4)
    {
      currentMode = "undefined";
      send_to_hub("thermostatMode undefined");
    }
    else if (lastOperation == 5)
    {
      fanOn();
    }
  }
}

void termSteps()
{
  char c;
  String cmd = "";

  if (term.available())
  {
    while (term.available())
    {
      c = term.read();
      cmd += c;
    }
    term.print("Ok I received : ");
    term.print(cmd);
    term.println();
    if (cmd == "reset")
    {
      term.println("RESETING ESP NOW");
      // give time for the terminal to display this message
      unsigned long s = millis();
      while (millis() - s < 1000)
      {
        serverAndHub();
      }
      Reset();
    }
  }
}
