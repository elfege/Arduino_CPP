void loop() {

  ArduinoOTA.handle();  // also handled by smartthings ESP8266 library but it doesn't run when OTA is called due to StopAll = true

  master();
  if (pumping && !clocked)  // always clock the purge to override any soft fault and make sure to shut it down after a max timeout
  {
    term.println("clocked true");
    clocked = true;
    maxTimeout = millis();
  }
  if (clocked && millis() - maxTimeout > 3600000)  // one hour strict max
  {
    Off();
    term.println("clocked false");
    clocked = false;
  }
  if (onRequestedByUser && pumping && millis() - previousMillisRun > MAX_RUN_AFTER_USER_REQUEST && !nonStopPurging)  // user (or accelerometer from hubitat) request, 5 seconds max
  {
    term.println(String(MAX_RUN_AFTER_USER_REQUEST/1000) + " SECONDS STOP AFTER USER PURGE REQUEST");
    onRequestedByUser = false;
    Off();
  }

  // purge the water tank/ activate pump on a schedule (MUST NOT RUN FROM MASTER)
  if (millis() - previousMillisPURGE > schedule && schedule != 0 && !scheduledRunning) {
    scheduledRunning=true;
    previousMillisPURGE = millis();
    On("scheduled purge");
  }


  if (nonStopPurging && millis() - nonStopPurgeMillis > 15 * 60 * 1000)  // no more than 15 minutes
  {
    nonStopPurgeMillis = millis();
    nonStopPurging = false;
    term.println("non stop time out");
    Off();
  }
  delay(10);
  yield();
}
void serverAndHub() {
  smartthing.run();
  _server.handleClient();
  term.handleClient();  // WiFi terminal
}
void master() {
  if (!StopAll) {
    serverAndHub();

    if (millis() - lastNTP > 10000 || ntpFailed && !stopNtp) {
      updateNtpTime();
      lastNTP = millis();
    } else if (millis() - lastNTP > 10000 && stopNtp) {
      term.println("NTP FAILED - USER MUST RESET ESP");
      // Don't automate reset for this; if DNS are down this controller would not stop resetting...
    }
    if (millis() - previousMillis > DELAY_SEC * 1000) {
      previousMillis = millis();
      Refresh();
    }
    if (millis() - previousMillisBlink > 1000) {
      digitalWrite(LED, !digitalRead(LED));
      previousMillisBlink = millis();
    }
    if (scheduledRunning && millis() - previousMillisRun > regularPurgeDuration)  // n seconds purge
    {
      previousMillisRun = millis();
      scheduledRunning = false;
      digitalWrite(Signal, 0);
    }
    if (nonStopPurging && millis() - nonStopPurgeMillis > 60 * 60 * 1000)  // no more than 60 minutes
    {
      nonStopPurgeMillis = millis();
      nonStopPurging = false;
      term.println("non stop time out");
      Off();
    }
  }

  if (wet() && !wetTriggered) {
    wetTriggered = true;
    triggerMillis = millis();
    lastHwetv = analogRead(A0);
    term.println("wet val = " + String(wetv));
    digitalWrite(Signal, 1);  // don't call On() from as to not reset the timer
    maxTimeout = millis();
    pumping = true;
    buildDebug("switch on");
    buildDebug("water wet");
  } else if (!wet() && wetTriggered) {
    buildDebug("water dry");
    wetTriggered = false;
  }
  if (!wet() && millis() - previousMillisRun > 10000 && pumping && !nonStopPurgeMillis)  // once it's no longer wet, give it 10 extra seconds to run.
  {
    Off();
  }
}
