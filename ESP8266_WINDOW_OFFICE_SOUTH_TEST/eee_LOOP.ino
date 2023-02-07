int loops = 0;

void loop() {
  //loopTime = millis();

  //term.print("Analog Read: ");term.println(analogRead(A0));
  if (!StopAll)
  {
    if (millis() - lastNTP > 10000 || ntpFailed && !stopNtp)
    {
      updateNtpTime();
      lastNTP = millis();
    }
    else if (millis() - lastNTP > 10000 && stopNtp)
    {
      term.println("NTP FAILED - USER MUST RESET ESP");
      // don't automate reset for this; if DNS are down this controller would not stop resetting...
    }
    master();
  }
  else
  {
    ArduinoOTA.handle(); // already managed by st lib but not when StopAll = true here...
  }
  if (millis() - LedOn > 500)
  {
    digitalWrite(LED, !digitalRead(LED));
    if (logs && millis() - lastLogMillis > 3000)
    {
      log();
      lastLogMillis = millis();
    }
    LedOn = millis();
  }
  //term.println(String(loops++));
  //  elapsed = millis() - loopTime;
  yield();
  delay(1);
}

void master()
{
  if (!isOpen() || isFullyOpen() && (digitalRead(C) == 1 || digitalRead(D) == 1))
  {
    if (millis() - previousMillisStop >= 1000)
    {
      //      term.println("fullyOpen result = " + String(fullyOpen) + " & digital READ = " + String(digitalRead(contact_sensor_fullyOpen)));
      //      String cause = !OPEN && fullyOpen ? "isOpen returned false and isfullyOpen returned true" : !OPEN ? "isOpen returned false" : fullyOpen ? "isfullyOpen returned true" : "UNKNOWN!!!";
      //term.println("******************STOP!****************");
      previousMillisStop = millis();
      analogWrite(pwm, 0);
      digitalWrite(C, 0);
      digitalWrite(D, 0);
    }
  }

  if (!isOpen() && !opening)
  {
    previousMillisMotors = millis();// reset the 5 minutes opening timer
    // to prevent it from stopping a closing procedure
  }
  if (opening && (!openingUpdateDone || lastOpeningReportWindowWasStillClosed) && millis() - timeLastOpeningUpdate > 5000 && events < 3)
  {
    String var = !isOpen() ? "contact closed" : "contact open";
    buildDebug(var);
    var = !isOpen() ? "window Closed&Opening" : "window Open&Opening";
    windowLastState = !isOpen() ? "window is closed but opening" : "window is open and opening";
    buildDebug(var);
    lastOpeningReportWindowWasStillClosed = !isOpen() ? true : false;
    timeLastOpeningUpdate = millis();
    openingUpdateDone = true;
    events++;
  }
  else if (closing && (!closingUpdateDone || lastClosingReportWindowWasStillOpen) && millis() - timeLastClosingUpdate > 5000 && events < 3)
  {
    String var = !isOpen() ? "contact closed" : "contact open";
    buildDebug(var);
    var = !isOpen() ? "window closed" : "window Open&Closing";
    windowLastState = !isOpen() ? "window is closed" : "window is open and closing";
    buildDebug(var);
    lastClosingReportWindowWasStillOpen = isOpen() ? true : false; // allows future update as soon as state changed from open to closed
    timeLastClosingUpdate = millis(); // necessary to avoid overflow while window is still open and closing
    closingUpdateDone = true;
    events++;
  }
  else if (wasclosed && isOpen() && !opening && !closing)
  {
    previousMillisContactOpen = millis();
    wasclosed = false;
    String var = "contact open";
    term.println(var);
    delay(200);
    buildDebug(var);
    var = "window open";
    windowLastState = "window is open";
    term.println(var);
    delay(500);
    buildDebug(var);
    var = "switch on";
    term.println(var);
    delay(500);
    smartthing.send(var);

  }
  else if (!wasclosed && !isOpen() && !opening && !closing)
  {
    previousMillisContactClosed = millis();
    wasclosed = true;
    String var = "contact closed";
    term.println(var);
    delay(500);
    buildDebug(var);
    var = "window closed";
    windowLastState = "window is closed";
    term.println(var);
    delay(500);
    buildDebug(var);
    var = "switch off";
    term.println(var);
    delay(500);
    smartthing.send(var);

  }
  serverAndHub(); // we want to be able to call for these without throwing any event yet in the mvt functions
}

void serverAndHub() {
  _server.handleClient();
  term.handleClient();// WiFi terminal
  termSteps(); // dunno what this is, took it from the example...
  smartthing.run();

  safety();

}

void termSteps() {
  char c;
  String cmd = "";

  if (term.available())
  {
    while (term.available())
    {
      c = term.read();
      cmd += c;
    }
    term.print("Ok I received : "); term.print(cmd);
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

void safety() {

  //stallCheck();

  boolean needAdjustment = !speedAdjusted ? millis() - previousMillisMotors > (3 * 60 * 1000) : millis() - lastSpeedAdjustment > (1 * 60 * 1000);

  if ((opening || closing) && needAdjustment)// if still running after x minutes, increase speed
  {
    previousMillisMotors = millis();
    int previousVal = ULTRASLOW;
    ULTRASLOW += 20;
    analogWrite(pwm, ULTRASLOW);
    buildDebug("TOO SLOW ADAPTING SPEED from " + String(previousVal) + " to " + String(ULTRASLOW));
    lastSpeedAdjustment = millis(); // only place where this value is defined, beside boot.
    speedAdjusted = true;
  }
  else if (!opening && !closing && ULTRASLOW != ULTRASLOW_RESTORE)
  {
    ULTRASLOW = ULTRASLOW_RESTORE; // restore default value for lowest speed
    speedAdjusted = false;
  }
  if ((opening || closing) && millis() - previousMillisMotors > (3 * 60 * 1000))
  {
    // after x minutes since last open or close execution, stop the motor controller
    opening = false; // reset this boolean
    previousMillisMotors = millis();
    Stop(); // stop motor controller
    buildDebug("SAFETY TIMEOUT");
    term.println("SAFETY TIMEOUT");
  }
  if (millis() - previousMillisBlink >= 1000)
  {
    previousMillisBlink = millis();
    digitalWrite(LED, !digitalRead(LED));
  }
}

void stallCheck()
{
  threshold = map(speedVal, FAST, ULTRASLOW_RESTORE, 300, 800);
  startThresHold = map(speedVal, FAST, ULTRASLOW_RESTORE, 1000, 3000);
  //  term.println("threshold = "+String(threshold));
  //  term.println("startThresHold = "+String(startThresHold));
  if (digitalRead(rotor) != readM)
  {
    readM = digitalRead(rotor);
    lastReadMillis = millis();
    //term.println("TURNING");
  }
  else if (millis() - lastReadMillis > threshold && millis() - previousMillisMotors > startThresHold) // check every XXX millis() once it had enough time to start rotating
  {
    //term.println("NOT TURNING");
    if (opening || closing)
    {
      term.println("STALL!!!!!!!!!");
      if (isOpen() && !isFullyOpen())
      {
        reverse();
      }
      Stop();
      buildDebug("window STALL");
    }
  }
}


String TimeInfos()
{
  unsigned long totalTimeSeconds = (millis() - millisBoot) / 1000; // we want the time at the end of setup, not since last reset
  unsigned long totalTimeMinutes = (millis() - millisBoot) / 1000 / 60;
  unsigned long TotalTimeHours = (millis() - millisBoot) / 1000 / 60 / 60;
  unsigned long TotalTimeDays = (millis() - millisBoot) / 1000 / 60 / 60 / 24;

  unsigned long displaySeconds = ((millis() - millisBoot) - (totalTimeMinutes * 60 * 1000)) / 1000;
  unsigned long displayMinutes = ((millis() - millisBoot) - (TotalTimeHours * 1000 * 60 * 60)) / 1000 / 60;
  unsigned long displayHours = ((millis() - millisBoot) - (TotalTimeDays * 1000 * 60 * 60 * 24)) / 1000 / 60 / 60;
  unsigned long dispalyDays = TotalTimeDays;


  //term.println("LOOP time = " + String(elapsed) + "ms");
  String result = "Time since last boot: ";
  timerResult = "";

  if (dispalyDays >= 1)
  {
    result = result + dispalyDays + " d";
    timerResult = dispalyDays + " d";
    //term.print(result);
  }
  if (displayHours >= 1)
  {
    result = result + displayHours + " h ";
    timerResult = displayHours + " h";
    //term.print(result);
  }
  if (displayMinutes >= 1)
  {
    result = result + displayMinutes + " m ";
    timerResult = displayMinutes + " m";
    //term.print(result);
  }
  if (displaySeconds >= 1)
  {
    result = result + displaySeconds + " s ";
    timerResult = displaySeconds + " s";
    //term.print(result);
  }

  result += " " + lastBootDate; // add the boot date stamp


  return result;
}
