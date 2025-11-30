void loop() {
  //  loopTime = millis();

  server.handleClient();
  ArduinoOTA.handle();

  //SAFETY OPERATIONS
  safetyOp();

  if (!docking && !mvt)
  {
    othertasks();
  }

  yield();
  //  elapsed = millis() - loopTime;
}
void safetyOp() {
  if (millis() - tracksOnmillis > 1000 && tracksAreOn())
  {
    trackstop();
  }
  if (millis() - motorsOnMillis > 500 && motorsAreOn())
  {
    STOP();
  }
}
void log() {
  if (millis() - lastMillis > 100) {
    //Blink(5, 30);
    lastMillis = millis();
    //    Serial.println("LEFT = " + String(SeeLineLeftFront()));
    //    Serial.println("");
    //    Serial.println("RIGHT = " + String(SeeLineRightFront()));
    //    Serial.println("");
    //    Serial.println("LEFT VAL = " + String(analogRead(LineLeftFront)));
    //    Serial.println("");
    //    Serial.println("RIGHT VAL = " + String(analogRead(LineRightFront)));
    //    Serial.println("");
    //    Serial.println("HAS POWER = " + String(haspower()));
    //    Serial.println("");
    //    Serial.println("Current: " + String(getvoltsonly()));
    //    Serial.println("analog Value = "+ String(analogRead(PW)));
    //    Serial.println("-------------------------------");
  }
}


void othertasks()
{
  if (logs)
  {
    log();
  }
  if (millis() - previousMillisBlink > 1000)
  {
    // normal operation without any client connected or client is getting slow (no update from JS for more than 3 seconds)
    if (!clientisconnected)
    {
      Blink(2, 20);
    }
    else
    {
      //Serial.println("loop time = " + String(elapsed) + " millisecs");
      digitalWrite(LED, !digitalRead(LED));

    }
    previousMillisBlink = millis();
    webconnectstatus();
  }


  //CONNECTION WATCHDOG
  if (millis() - lastWifiConnection > 5000)
  {
    if (WiFi.status() != WL_CONNECTED )
    {
     // connection();
    }
  }
}

void TimeInfos()
{
  unsigned long totalTimeSeconds = previousMillisBlink / 1000;
  unsigned long totalTimeMinutes = previousMillisBlink / 1000 / 60;
  unsigned long TotalTimeHours = previousMillisBlink / 1000 / 60 / 60;
  unsigned long TotalTimeDays = previousMillisBlink / 1000 / 60 / 60 / 24;

  unsigned long displaySeconds = (previousMillisBlink - (totalTimeMinutes * 60 * 1000)) / 1000;
  unsigned long displayMinutes = (previousMillisBlink - (TotalTimeHours * 1000 * 60 * 60)) / 1000 / 60;
  unsigned long displayHours = (previousMillisBlink - (TotalTimeDays * 1000 * 60 * 60 * 24)) / 1000 / 60 / 60;
  unsigned long dispalyDays = TotalTimeDays;


  Serial.println("LOOP time = " + String(elapsed) + "ms");
  Serial.print("Time since last boot = ");
  if (dispalyDays == 1)
  {
    Serial.print(dispalyDays); Serial.print(" day ");
  }
  else if (dispalyDays > 1)
  {
    Serial.print(dispalyDays); Serial.print(" days ");
  }
  if (displayHours == 1)
  {
    Serial.print(displayHours); Serial.print(" hour " );
  }
  else if (displayHours > 1)
  {
    Serial.print(displayHours); Serial.print(" hours " );
  }
  if (displayMinutes == 1)
  {
    Serial.print(displayMinutes); Serial.print(" minute " );
  }
  else if (displayMinutes > 1)
  {
    Serial.print(displayMinutes); Serial.print(" minutes " );
  }
  if (displaySeconds == 1)
  {
    Serial.print(displaySeconds); Serial.println(" second");
  }
  else if (displaySeconds > 1)
  {
    Serial.print(displaySeconds); Serial.println(" seconds");
  }
}
