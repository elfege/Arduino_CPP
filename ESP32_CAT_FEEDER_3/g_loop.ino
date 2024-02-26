void loop() {
  mainHandler();
}

void mainHandler() {
  if (!stopAll) {
    if (!readyOTA) {
      elapsed_OTA_ready = 0;
      httpHandler();
      logs();
      blinker();
      motorsTimer();
      chasDeformHandler();
      pusherRetractCheck();
      cosinusHandler();
      wheelSpinning();
      hubRefreshHandler();
      problemSolver();
    } else {
      if (millis() - waitingForOTAMillis > 1000) {
        elapsed_OTA_ready += 1;
        term.println("waiting for OTA UPDATE... remaining time:" + String((OTA_ready_timeout - (elapsed_OTA_ready * 1000)) / 1000) + " seconds");
        waitingForOTAMillis = millis();
      }
      term.handleClient();
    }
    if (readyOTA && millis() - readyOTAmillis > OTA_ready_timeout) {
      term.println("OTA AWAITING DONE! Resuming normal operations");
      // time is up, reset readyOTA
      readyOTA = false;
      stopAll = false;
      _server.send(200, "text/plain", "OTA");
      httpHandler();
      delay(100);
      Reset();
    }
  }
  ArduinoOTA.handle();
  yield();
}

void hubRefreshHandler() {
  if (millis() - lastRefresh > 120000) {
    getStates(true);
  }
}

void wait(int duration) {
  unsigned long Start = millis();
  while (millis() - Start < duration) {
    yield();
  }
}


