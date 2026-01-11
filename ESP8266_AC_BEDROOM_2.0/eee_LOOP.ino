void loop() {
  //loopTime = millis();
  if (!StopAll)
  {
    serverAndHub();

    if (millis() - lastNTP > 10000 || ntpFailed && !stopNtp)
    {
      updateNtpTime();
      lastNTP = millis();
    }
    else if (millis() - lastNTP > 10000 && stopNtp)
    {
      term.println("NTP FAILED - USER MUST RESET ESP");
      // don't automate reset for this; if DNS are down this controller would not stop resetting...
    }
    if (logs)
    {
      log();
    }

    if (millis() - previousMillis > 600000)
    {
      Refresh();
      previousMillis = millis();
    }
    if(lastMode != currentMode)
    {
      term.println("new current mode value ("+String(currentMode)+")");
      lastMode = currentMode;
      Refresh();
    }
    glow();
  }
  else
  {
    ArduinoOTA.handle();
  }
  if (millis() - stopAllRequest > 60000 && StopAll && tempRequest) // reset the temporary hold
  {
    StopAll = false;
    tempRequest = false;
  }
  yield();
  delay(1);

}

void serverAndHub() {
  smartthing.run();
  _server.handleClient();
  term.handleClient();// WiFi terminal
  termSteps();
}

void termSteps() {
  char c;
  String cmd = "";

  if (term.available())
  {
    while (term.available())
    {
      c = term.read();
      cmd += c;
    }
    term.print("Ok I received : "); term.print(cmd);
    term.println();
    if (cmd == "reset")
    {
      term.println("RESETING ESP NOW");
      // give time for the terminal to display this message
      unsigned long s = millis();
      while (millis() - s < 1000)
      {
        serverAndHub();
      }
      Reset();
    }
  }
}
