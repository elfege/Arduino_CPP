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
  delay(1);
}

void master()
{
  if (!isOpen() || isFullyOpen())
  {
    if (millis() - previousMillisStop >= 1000)
    {
//      Serial.println("fullyOpen result = " + String(fullyOpen) + " & digital READ = " + String(digitalRead(contact_sensor_fullyOpen)));
//      String cause = !OPEN && fullyOpen ? "isOpen returned false and isfullyOpen returned true" : !OPEN ? "isOpen returned false" : fullyOpen ? "isfullyOpen returned true" : "UNKNOWN!!!";
      Serial.println("******************STOP!****************");
      previousMillisStop = millis();
      Stop();
    }
  }
  if (millis() - previousMillisBlink >= 1000)
  {
    previousMillisBlink = millis();
    digitalWrite(LED, !digitalRead(LED));
  }
  _server.handleClient();
  smartthing.run();
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
