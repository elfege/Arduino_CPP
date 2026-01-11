void loop() {

  //term.print("Analog Read: ");term.println(analogRead(A0));
  //term.println("inFailSafeMode ? "+String(inFailSafeMode));
  if (!StopAll && !inFailSafeMode) {
    if (millis() - millisBoot < 30000 && !bootRestore) {
      bootRestore = true;
      term.println("Restoring last recorded operation lastOperation = " + String(lastOperation == 1 ? "opening" : lastOperation == 2 ? "closing"
                                                                                                                                     : "stop"));
      if (lastOperation == 1) {
        Open();
      } else if (lastOperation == 2) {
        Close();
      }
    }
    if (millis() - millisBoot > 1500 && !updateRTCdone)  // this is just for wifiTerm debug purpose after fresh boot
    {
      updateFromRTC();
      updateRTCdone = true;
    }
    if (millis() - lastNTP > NTP_UPDATE_DELAY || ntpFailed && !stopNtp) {
      updateNtpTime();
      lastNTP = millis();
    } else if (millis() - lastNTP > 10000 && stopNtp) {
      term.println("NTP FAILED - USER MUST RESET ESP");
      // don't automate reset for this; if DNS are down this controller would not stop resetting...
    }
    if (!opening && !closing && donotAcceptCmdsFromHubs == 1) {
      donotAcceptCmdsFromHubs = 0;
    }
    master();

  } else {
    ArduinoOTA.handle();  // already managed by st lib but not when StopAll = true here...
    if (inFailSafeMode && millis() - LedOn > 1000) {
      digitalWrite(LED, !digitalRead(LED));  // double blink in fail safe mode
      delay(300);
      digitalWrite(LED, !digitalRead(LED));
      term.println("FAIL SAFE MODE");
      LedOn = millis();
    }
    if (inFailSafeMode) {
      _server.handleClient();  // JS webpage and wifi terminal
      term.handleClient();
      state.saveToRTC();  // FailSafe mode must update RTC failSafeBoot var
    }
  }
  yield();
  delay(10);
}


void master() {

  serverAndHub();

  fanmanagement(); 

  if (!wasclosed && contactClosed) {
    wasclosed = true;
    windowLastState = isOpen() ? "window is open" : "window is closed";
    sendHub("contact closed");
    sendHub("window closed");
    sendHub("switch off");
  }
  if (wasclosed && !contactClosed) {
    wasclosed = false;
    windowLastState = isOpen() ? "window is open" : "window is closed";
    sendHub("contact closed");
    sendHub("window closed");
    sendHub("switch off");
  }

  if (millis() - previousMillisBlink > 10000) {
    digitalWrite(LED, !digitalRead(LED));
    delay(50);
    digitalWrite(LED, !digitalRead(LED));
    previousMillisBlink = millis();
  }
  if (millis() - lastSpeedRequestMillis > RequestDelay && userSpeedInputRequested) {
    userSpeedInputRequested = false;  // reset user input boolean after 30 seconds
  }

  if (logs && millis() - lastLogMillis > 3000) {
    log();
    lastLogMillis = millis();
  }


  //  maintenance(); // DEPRECATED ISR instead
  state.saveToRTC();  // there are no parameters because it only needs the vars' definiton once
  delay(10);
  yield();
}
void serverAndHub() {  // IN THIS PROGRAM THIS FUNCTION IS CALLED ONLY FROM termSteps. MUST NOT BE CALLED FROM OPEN OR CLOSE() (unlike other windows sketches)
  smartthing.run();
  _server.handleClient();  // JS webpage and wifi terminal
  term.handleClient();     // WiFi terminal
  termSteps();
  yield();
}
void termSteps() {
  char c;
  String cmd = "";

  if (term.available()) {
    while (term.available()) {
      c = term.read();
      cmd += c;
    }
    term.print("Ok I received : ");
    term.print(cmd);
    term.println();
    if (cmd == "reset") {
      term.println("RESETING ESP NOW");
      // give time for the terminal to display this message
      unsigned long s = millis();
      while (millis() - s < 1000) {
        serverAndHub();
      }
      Reset();
    }
  }
}


String upTime() {
  unsigned long totalTimeSeconds = (millis() - millisBoot) / 1000;  // we want the time at the end of setup, not since last reset
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

  if (dispalyDays >= 1) {
    result = result + dispalyDays + " d";
    timerResult = dispalyDays + " d";
    //term.print(result);
  }
  if (displayHours >= 1) {
    result = result + displayHours + " h ";
    timerResult = displayHours + " h";
    //term.print(result);
  }
  if (displayMinutes >= 1) {
    result = result + displayMinutes + " m ";
    timerResult = displayMinutes + " m";
    //term.print(result);
  }
  if (displaySeconds >= 1) {
    result = result + displaySeconds + " s ";
    timerResult = displaySeconds + " s";
    //term.print(result);
  }

  result += " " + bootStartDate;  // add the boot date stamp


  return result;
}

void fanmanagement() {

  if (digitalRead(FAN) && !opening && !closing) {
    digitalWrite(FAN, 0);
  }
  
}
