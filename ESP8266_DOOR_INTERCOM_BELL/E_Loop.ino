void loop() {
  ArduinoOTA.handle();
  if (!StopAll) {
    master();
  }
  yield();
  delay(10);  // absolutely required! too many analogRead()'s crashes the wifi - in ESP8266 architechture delay() function makes a quick call to background functions.
}

void master() {
  // process sensor monitoring and other and handlers when operation is on due to positive signal
  if (!got_signal && !runsim) {
    monitor_signal();
    WiFiHandler();
    ledBlink();
    httpHandler();  // runs from the wait() function
    if (millis() - lastRefresh > 5 * 60 * 1000) {
      Refresh();
    }
  } else if (!switch_door_operation_is_running) {  // if operation hasn't been triggered already
    mainOp();
  }
}

// function to capture a positive signal and hold on to it until operation is executed, to prevent iterative false negatives
void monitor_signal() {
  sensorValue = analogRead(A0);
  got_signal = sensorValue >= threshold;
}

void mainOp() {
  unsigned long messageSendStart = millis();
  if (runsim || got_signal)  // bell's ringing or running a test
  {
    if(got_signal) term.println("got_signal! Value: " + String(sensorValue));
    if (runsim) term.println("TEST START");

    term.println("awaiting send_event()...");
    send_event("switch on");
    messageSendStart = millis();
    while (millis() - messageSendStart < 10000 && !sent) {
      wait(100);  // calls httpHandler()
    }

    if (!switch_door_operation_is_running)  // !switch_door_operation_is_running prevents operation from runing over and over while sensorValue returns a value above threshold
    {
      if (!runsim) PowerOn();  // run only if it's not a test
      got_signal = false;      // once operation is done running, reset the got_signal bool to resume normal analogRead() monitoring
      if(!runsim) terminate(true);         // terminate the communication in the while loop below
      
      term.println("awaiting send_event()");
      send_event("switch off");
      messageSendStart = millis();
      while (millis() - messageSendStart < 10000 && !sent) {
        wait(100);  // calls httpHandler()
      }
      runsim = false;
    }
  } else {
    PowerOff();  // conditioned to I/O states.
  }
}

void terminate(bool termination_needed) {
  // Making sure com is terminated once operation is done running (don't use got signal here, we read the actual remaining signal, which comes from the video feed power pin of the existing intercom's system)
  while (termination_needed && analogRead(A0) >= threshold && lastloop <= 4 && !switch_door_operation_is_running) {
    digitalWrite(TALK, PRESS);  // press talk button to terminate communication
    wait(500);
    digitalWrite(TALK, RELEASE);  // release talk button
    wait(500);
    lastloop++;
    httpHandler();
  }
  lastloop = 0;  // reset this value
}