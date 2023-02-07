void loop() {
  
  

  if (!OTArequest) {
    loopTime = millis();
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("*********************************************************************************");
      Serial.println("***************************** CONNECTION LOST ***********************************");
      Serial.println("*********************************************************************************");
      connection();
    }
    if (millis() - previousMillisBlink > 1000)
    {
      //Serial.println("last loop took "+String(elapsed)+" millisecs"); 
      Blink(1, 100);
      previousMillisBlink = millis();
    }
    safetyfunctions();
    server.handleClient();
  }

  
  ArduinoOTA.handle();


  delay(30); // necessary for servo control and overall stability
  if (!OTArequest) {
  elapsed = millis() - loopTime;
  }

}

void log() {
  if (millis() - lastMillis > 1000) {
    Blink(5, 30);
    lastMillis = millis();


  }
}

void safetyfunctions() {

  if (millis() - tracksonmillis > 3000)
  {
    trasckstop();
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
