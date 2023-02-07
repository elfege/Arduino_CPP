int loops = 0;

void loop() {
  loopTime = millis();

  //Serial.print("Analog Read: ");Serial.println(analogRead(A0));
  if (!StopAll)
  {
    master();
    _server.handleClient();
    smartthing.run();

  }
  else
  {
    ArduinoOTA.handle(); // already managed by st lib but not when StopAll = true here...
  }
  //Serial.println(String(loops++));
  elapsed = millis() - loopTime;
}


void master()
{
  if (logs)
  {
    log();
  }
  if (emergencySpeed)
  {
    //Maintain speed value due to emergency request
    emergencySpeed = false;
    analogWrite(pwm, FAST);
    if (millis() - emergencySpeedMillis > 3000) // dealy to give time for req to be implmtd
    {
      emergencySpeed = false;
    }
  }
  if (!isOpen() && !opening)
  {
    previousMillisMotors = millis();// reset the 5 minutes opening timer
    // to prevent it from stopping a closing procedure
  }
  if (!isOpen() && closing)
  {
    buildDebug("contact closed");
    buildDebug("window closed");
    //    buildDebug("window stop");
    smartthing.send("switch off");
    closing = false;
    analogWrite(pwm, SpeedVal + PUSH);
  }
  if (touch() && !stopped && closing && isOpen())
  {
    simpleStop(); // MAJOR SAFETY !!!
    Serial.println("EMERGENCY STOP!");
    Serial.print("TOUCH ="); Serial.println(TOUCH);
    buildDebug("WINDOW EMERGENCY_STOP");
    emergency = true;
    closing = false; //allows resending command
    buildDebug("window TOUCHED");
    forceOpen(5000);
  }
  else if (emergency)
  {
    //resume closing
    Close();// no simpleClose() here; we need variables to be updated
    analogWrite(pwm, lastSpeedVal); // reinstate last speed request
    emergency = false;
  }
  else if (opening && emergency)
  {
    //resume opening
    Open();
    emergency = false;
  }
  if (touch() && stopped && !NoTouchOpen)
  {
    forceOpen(2000);
  }

  if (closing && !touch() && !stopped)
  {
    //make sure it is closing
    simpleClose();
  }
  if (millis() - closingcmdMillis > 5 * 60 * 1000 && lastCmd == "close" && isOpen())
  {
    // has been running for too long without detecting closed contact
    // after 5 minutes runtime, forceclose it
    NoTouchOpen = true; // so touch doesn't reopen
    buildDebug("FORCE CLOSING!");
    lastSpeedVal = SpeedVal + 200;
    simpleClose();
    buildDebug("contact closed");// declare as closed to ST in any case
    closingcmdMillis = millis();
  }

  maintenance();

  delay(10);
}

void maintenance()
{
  if (modifRequested) {
    // adjust speed to request
    String var = "window newSpeedOk";
    analogWrite(pwm, SpeedVal);
    modifRequested = false; // reset this value
    buildDebug(var);
    //buildDebug("10 bits speed value = " + String(SpeedVal));
  }


  if (wasclosed && isOpen())
  {
    evtscount++;
    previousMillisContactOpen = millis();
    wasclosed = false;
    String var = "contact open";
    Serial.println(var);
    delay(200);
    buildDebug(var);
    var = "window open";
    Serial.println(var);
    delay(500);
    buildDebug(var);
    var = "switch on";
    Serial.println(var);
    delay(500);
    smartthing.send(var);

  }
  else if (!wasclosed && !isOpen())
  {
    evtscount++;
    previousMillisContactClosed = millis();
    wasclosed = true;
    String var = "contact closed";
    Serial.println(var);
    delay(500);
    buildDebug(var);
    var = "window closed";
    Serial.println(var);
    delay(500);
    buildDebug(var);
    var = "switch off";
    Serial.println(var);
    delay(500);
    smartthing.send(var);

  }

  if ((opening || closing) && millis() - previousMillisMotors > (30 * 60 * 1000))
  {
    // after x minutes since last open or close execution, stop the motor controller
    opening = false; // reset this boolean
    previousMillisMotors = millis();
    Stop(); // stop motor controller
    //buildDebug("STOP AFTER 5 MINUTES");
  }

  // refresh statuses every x minutes
  if (millis() - previousMillis > DELAY_SEC * 1000)
  {
    previousMillis = millis();
    Serial.println("REFRESH --------------");
    //buildDebug("CALLING REFRESH");
    Refresh();


    //Blink(5, 50);
  }


  if (millis() - previousMillisBlink >= 1000)
  {

    LedOn = millis();
    previousMillisBlink = millis();
    if (millis() - lastTimeDisplay > 10000)
    {
      lastTimeDisplay = millis();
      TimeInfos();
    }
  }
  if (millis() - LedOn > 500)
  {
    digitalWrite(LED, !digitalRead(LED));
    if (logs)
    {
      log();
    }
  }

  /*if (millis() - previousMillisReset > ResetTime && stopped)
    {
    Reset();
    esp_restart();
    }*/
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


  //Serial.println("LOOP time = " + String(elapsed) + "ms");
  String result = "Time since last boot: ";
  timerResult = "";

  if (dispalyDays == 1)
  {
    result = result + dispalyDays + " day";
    timerResult = dispalyDays + " day";
    //Serial.print(result);
  }
  else if (dispalyDays > 1)
  {
    result = result + dispalyDays + " days";
    timerResult = dispalyDays + " days";
    //Serial.print(result);
  }
  if (displayHours == 1)
  {
    result = result + displayHours + " hour ";
    timerResult = displayHours + " hour";
    //Serial.print(result);
  }
  else if (displayHours > 1)
  {
    result = result + displayHours + " hours ";
    timerResult = displayHours + " hours";
    //Serial.print(result);
  }
  if (displayMinutes == 1)
  {
    result = result + displayMinutes + " minute ";
    timerResult = displayMinutes + " minute";
    //Serial.print(result);
  }
  else if (displayMinutes > 1)
  {
    result = result + displayMinutes + " minutes ";
    timerResult = displayMinutes + " minutes";
    //Serial.print(result);
  }
  if (displaySeconds == 1)
  {
    result = result + displaySeconds + " second ";
    timerResult = displaySeconds + " second";
    //Serial.print(result);
  }
  else if (displaySeconds > 1)
  {
    result = result + displaySeconds + " seconds ";
    timerResult = displaySeconds + " seconds";
    //Serial.print(result);
  }

  _server.send(200, "text/html", result);
  return result;
}

String TimingInfos()
{
  unsigned long totalTimeSeconds = millis() / 1000;
  unsigned long totalTimeMinutes = millis() / 1000 / 60;
  unsigned long TotalTimeHours = millis() / 1000 / 60 / 60;
  unsigned long TotalTimeDays = millis() / 1000 / 60 / 60 / 24;

  unsigned long displaySeconds = (millis() - (totalTimeMinutes * 60 * 1000)) / 1000;
  unsigned long displayMinutes = (millis() - (TotalTimeHours * 1000 * 60 * 60)) / 1000 / 60;
  unsigned long displayHours = (millis() - (TotalTimeDays * 1000 * 60 * 60 * 24)) / 1000 / 60 / 60;
  unsigned long dispalyDays = TotalTimeDays;


  //Serial.println("LOOP time = " + String(elapsed) + "ms");
  String result = "Time since last boot: ";
  timerResult = "";

  if (dispalyDays == 1)
  {
    timerResult = timerResult + dispalyDays + " day";
    //Serial.print(result);
  }
  else if (dispalyDays > 1)
  {
    timerResult = timerResult + dispalyDays + " days";
    //Serial.print(result);
  }
  if (displayHours == 1)
  {
    timerResult = timerResult + displayHours + " hour ";
    //Serial.print(result);
  }
  else if (displayHours > 1)
  {
    timerResult = timerResult + displayHours + " hours ";
    //Serial.print(result);
  }
  if (displayMinutes == 1)
  {
    timerResult = timerResult + displayMinutes + " minute ";
    //Serial.print(result);
  }
  else if (displayMinutes > 1)
  {
    timerResult = timerResult + displayMinutes + " minutes ";
    //Serial.print(result);
  }
  if (displaySeconds == 1)
  {
    timerResult = timerResult + displaySeconds + " second ";
    //Serial.print(result);
  }
  else if (displaySeconds > 1)
  {
    timerResult = timerResult + displaySeconds + " seconds ";
    //Serial.print(result);
  }

  return timerResult;
}
