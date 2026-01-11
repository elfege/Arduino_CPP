void loop() {
  ArduinoOTA.handle();
  if (!StopAll) {
    master();
    WiFiHandler();
    httpHandler();                // also called from the wait() function
    digitalWrite(TALK, RELEASE);  // ensure it's always eventually released.
  }
  yield();
}

void master() {
  // process sensor monitoring and other and handlers when operation is on due to positive signal
  if (!got_signal && !runsim) {
    monitor_signal();
    ledBlink();
    if (millis() - lastRefresh > 5 * 60 * 1000) {
      Refresh();
    }
  } else if (!switch_door_operation_is_running) {  // if operation hasn't been triggered already
    mainOp();
  }
}

// function to capture a positive signal and hold on to it until operation is executed, to prevent iterative false negatives
void monitor_signal() {
  wait(50);  // services HTTP while waiting// absolutely required! too many analogRead()'s crashes the wifi - in ESP8266 architechture delay() function makes a quick call to background functions.
  sensorValue = analogRead(A0);
  got_signal = sensorValue >= threshold;
  if (millis() - lastSignalLog > 1000) {
    term.println("signal: " + String(sensorValue));
    lastSignalLog = millis();
  }
}

void mainOp() {
  unsigned long messageSendStart = millis();
  if (runsim || got_signal)  // bell's ringing or running a test
  {
    if (got_signal) term.println("got_signal! Value: " + String(sensorValue));
    if (runsim) term.println("RUNNING TEST");

    if (!switch_door_operation_is_running)  // !switch_door_operation_is_running prevents operation from runing over and over while sensorValue returns a value above threshold
    {
      // if (!runsim) EXECUTE_MAIN_OPERATION();  // run only if it's not a test
      EXECUTE_MAIN_OPERATION();
      got_signal = false;  // once operation is done running, reset the got_signal bool to resume normal analogRead() monitoring
      // if(!runsim) terminate(true);         // terminate the communication in the while loop below

      runsim = false;
    }
  } else {
    PowerOff();  // conditioned to I/O states.
  }

  // term.println("awaiting send_event()...");
  // send_event("switch on");
  // messageSendStart = millis();
  // while (millis() - messageSendStart < 10000 && !sent) {
  //   wait(100);  // calls httpHandler()
  // }
}
