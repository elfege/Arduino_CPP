int loops = 0;

void loop() {
  //loopTime = millis();

  //Serial.print("Analog Read: ");Serial.println(analogRead(A0));
  if (!StopAll)
  {
    master();

  }
  else
  {
    ArduinoOTA.handle(); // already managed by st lib but not when StopAll = true here...
  }
  if (logs)
  {
    log();
  }
  //Serial.println(String(loops++));
  //  elapsed = millis() - loopTime;
  yield();
  delay(1);
}


void master()
{
  if (!isOpen() || isFullyOpen() && (opening || closing))
  {
    if (millis() - previousMillisStop >= 1000)
    {
      //      Serial.println("fullyOpen result = " + String(fullyOpen) + " & digital READ = " + String(digitalRead(contact_sensor_fullyOpen)));
      //      String cause = !OPEN && fullyOpen ? "isOpen returned false and isfullyOpen returned true" : !OPEN ? "isOpen returned false" : fullyOpen ? "isfullyOpen returned true" : "UNKNOWN!!!";
      //Serial.println("******************STOP!****************");
      previousMillisStop = millis();
      Stop();
    }
  }

  if (!isOpen() && !opening)
  {
    previousMillisMotors = millis();// reset the 5 minutes opening timer
    // to prevent it from stopping a closing procedure
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



  serverAndHub(); // we want to be able to call for these without throwing any event yet in the mvt functions
}

void serverAndHub() {
  _server.handleClient();
  smartthing.run();

  safety();

}

void safety() {
  
  //stallCheck();

  if ((opening || closing) && millis() - previousMillisMotors > (60 * 1000))
  {
    // after x minutes since last open or close execution, stop the motor controller
    opening = false; // reset this boolean
    previousMillisMotors = millis();
    Stop(); // stop motor controller
    buildDebug("SAFETY TIMEOUT");
    Serial.println("SAFETY TIMEOUT");
  }
  if (millis() - previousMillisBlink >= 1000)
  {
    previousMillisBlink = millis();
    digitalWrite(LED, !digitalRead(LED));
  }
}

void stallCheck()
{
  threshold = map(speedVal, FAST, ULTRASLOW, 300, 800);
  startThresHold = map(speedVal, FAST, ULTRASLOW, 1000, 3000);
  //  Serial.println("threshold = "+String(threshold));
  //  Serial.println("startThresHold = "+String(startThresHold));
  if (digitalRead(rotor) != readM)
  {
    readM = digitalRead(rotor);
    lastReadMillis = millis();
    //Serial.println("TURNING");
  }
  else if (millis() - lastReadMillis > threshold && millis() - previousMillisMotors > startThresHold) // check every XXX millis() once it had enough time to start rotating
  {
    //Serial.println("NOT TURNING");
    if (opening || closing)
    {
      Serial.println("STALL!!!!!!!!!");
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
