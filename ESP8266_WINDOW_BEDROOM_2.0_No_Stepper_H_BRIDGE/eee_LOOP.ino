/* IDENTICAL TO THE OTHER WINDOW USING SAME THREADED ROD BASED ACTUATOR */
void loop() {
  //loopTime = millis();

  if (millis() - millisBoot < 30000 && !bootRestore)
  {
    bootRestore = true;
    term.println("Restoring last recorded operation lastOperation = " + String(lastOperation == 1 ? "opening" : lastOperation == 2 ? "closing" : "stop"));
    if (lastOperation == 1)
    {
      Open();
    }
    else if (lastOperation == 2)
    {
      Close();
    }
  }

  //term.print("Analog Read: ");term.println(analogRead(A0));
  if (!StopAll)
  {
    if (millis() - lastNTP > NTP_UPDATE_DELAY || ntpFailed && !stopNtp)
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
    }
  }
  //term.println(String(loops++));
  //  elapsed = millis() - loopTime;
  term.handleClient();// WiFi terminal
  yield();
  delay(1);
}

void master() {
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
  _server.handleClient(); // JS webpage and wifi terminal
  //  term.println("term clt hdl");
  term.handleClient();// WiFi terminal (second call, necessary)
  smartthing.run();
  safety();
}

void safety() {

  /// SPECIFIC TO THIS BOARD...
  if (opening || closing)
  {
    digitalWrite(FAN, 1);
  }
  else
  {
    digitalWrite(FAN, 0);
  }

  boolean needAdjustment = !speedAdjusted ? millis() - previousMillisMotors > lastMaxOpDuration : millis() - lastSpeedAdjustment > (shortDelayAdjust);


  // this operation must happen only 3 times after reset
  if ((opening || closing) && needAdjustment)// if still running after x minutes, increase speed
  {
    if (state.loadFromRTC() && speedAdjustmentOccurences == 0) // if values are stored and it's the first attempt
    {
      term.println("APPLYING LAST RTC CLOSING SPEED " + String(FINALSPEED));
      analogWrite(pwm, FINALSPEED);
      unsigned long Start = millis();
      unsigned cc = millis();
      while (isOpen() && millis() - Start < 30000 && !stopped) // wait 30000 secs max
      {
        if (millis() - cc >= 5000)
        {
          FINALSPEED += 50; // if still open means that FINALSPEED was too slow, so learn from it.
          term.println("INCREASING NEW CLOSING SPEED " + String(FINALSPEED));
          analogWrite(pwm, FINALSPEED);
          cc = millis();
        }
        yield();
      }
      if (isOpen())
      {
        Stop(); // terminate current operation
        term.println("THERE'S A PROBLEM...");
        Stop();
        return;
      }
    }
    else 
    {
      previousMillisMotors = millis();
      int previousVal = FINALSPEED;
      FINALSPEED += speedIncrement;
      FINALSPEED = FINALSPEED > 1023 ? 1023 : FINALSPEED;
      analogWrite(pwm, FINALSPEED);
      buildDebug("TOO SLOW ADAPTING FINALSPEED from " + String(previousVal) + " to " + String(FINALSPEED));
      lastSpeedAdjustment = millis(); // only place where this value is defined, beside boot.

    }
    speedAdjusted = true;
    speedAdjustmentOccurences++;
  }
  else if (!opening && !closing && speedAdjustmentOccurences != 0)
  {
    speedAdjusted = false;
    speedAdjustmentOccurences = 0; // now reset this value for next time (and also to stop this scope)
    doneLearning = 1;
  }
  if ((opening || closing) && millis() - previousMillisMotors > (6 * 60 * 1000))
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
  state.saveToRTC(); // there are no parameters because it only needs the vars' definiton once
}

String TimeInfos()
{
  unsigned long totalTimeSeconds = millis() / 1000;
  unsigned long totalTimeMinutes = millis() / 1000 / 60;
  unsigned long TotalTimeHours = millis() / 1000 / 60 / 60;
  unsigned long TotalTimeDays = millis() / 1000 / 60 / 60 / 24;

  unsigned long displaySeconds = (millis() - (totalTimeMinutes * 60 * 1000)) / 1000;
  unsigned long displayMinutes = (millis() - (TotalTimeHours * 1000 * 60 * 60)) / 1000 / 60;
  unsigned long displayHours = (millis() - (TotalTimeDays * 1000 * 60 * 60 * 24)) / 1000 / 60 / 60;
  unsigned long dispalyDays = TotalTimeDays;

  //term.println("LOOP time = " + String(elapsed) + "ms");
  String result = "Time since last boot: ";
  timerResult = "";

  if (dispalyDays == 1)
  {
    result = result + dispalyDays + " day";
    timerResult = dispalyDays + " day";
    //term.print(result);
  }
  else if (dispalyDays > 1)
  {
    result = result + dispalyDays + " days";
    timerResult = dispalyDays + " days";
    //term.print(result);
  }
  if (displayHours == 1)
  {
    result = result + displayHours + " hour ";
    timerResult = displayHours + " hour";
    //term.print(result);
  }
  else if (displayHours > 1)
  {
    result = result + displayHours + " hours ";
    timerResult = displayHours + " hours";
    //term.print(result);
  }
  if (displayMinutes == 1)
  {
    result = result + displayMinutes + " minute ";
    timerResult = displayMinutes + " minute";
    //term.print(result);
  }
  else if (displayMinutes > 1)
  {
    result = result + displayMinutes + " minutes ";
    timerResult = displayMinutes + " minutes";
    //term.print(result);
  }
  if (displaySeconds == 1)
  {
    result = result + displaySeconds + " second ";
    timerResult = displaySeconds + " second";
    //term.print(result);
  }
  else if (displaySeconds > 1)
  {
    result = result + displaySeconds + " seconds ";
    timerResult = displaySeconds + " seconds";
    //term.print(result);
  }

  result += " " + lastBootDate; // add the boot date stamp
  return result;
}
