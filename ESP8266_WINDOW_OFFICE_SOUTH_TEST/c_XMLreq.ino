

void initXMLhttp()
{
  _server.on("/", []()
  {
    _server.send_P(200, "text/html", MAIN_page);

  });
  _server.on("/index2", []()
  {
    _server.send_P(200, "text/html", simplifiedPage);

  });

  _server.on("/open", Open);
  _server.on("/close", Close);

  _server.on("/clear", []()
  {
    debugData = "";
    previousDebugData = "";
    previousDebugDataCmd = "";
    previousDebugDataCmdBits = "";
    previousDebugDataCmdLevel = "";

  });
  _server.on("/getstate", []()
  {
    String state = isOpen() ? "window is open" : "window is closed";
    _server.send(200, "text/html", state);
    if (isOpen())
    {
      String state = isFullyOpen() ? "WINDOW IS FULLY OPEN" : "WINDOW IS PARTIALLY OPEN";
      _server.send(200, "text/html", state);
    }
  });

  _server.on("/getClock", []()
  {
    String clockInfo = timeClient.getFormattedTime();
    _server.send(200, "text/html", clockInfo);
  });
  _server.on("/calibrate", []()
  {
    calibrate();
  });

  _server.on("/logs", []()
  {
    logs = !logs;
    if (!logs)
    {
      debugData = "";
      buildDebug("DATA RESET REQUESTED");
    }
  });

  _server.on("/stop", []()
  {
    //    stoppedRequestedByUser = true;
    Stop();
  });
  _server.on("/refresh", Refresh);
  _server.on("/getMac", getMac);
  _server.on("/getIP", getIP);
  _server.on("/getSSID", getSSID);
  _server.on("/getRSSI", getRSSI);
  _server.on("/speed", getSpeed);
  _server.on("/speed", getSpeed);

  _server.on("/debug", getDataDebug);
  _server.on("/getTimeInfo", []()
  {
    String data = TimeInfos();
    _server.send(200, "text/html", data);

  });

  _server.on("/reset", Reset);
  _server.on("/hardreset", hardReset);

  _server.on("/clear", []()
  {
    debugData = "";

  });

  // speed range from slider
  int i = 0;
  int s = 100; // max speed value
  for (1; i <= s; i++) {
    //  term.println(i);
    String iString = String(i);
    String str1 = "/speedRange";
    String str2 = str1 + "=" + iString;
    _server.on(str2, [i, iString]() {
      term.print("NEW SPEED: "); term.println(i);
      // update the global variable
      speedVal = map(i, 0, 100, ULTRASLOW_RESTORE, FAST);
      lastSpeedVal = speedVal;
      modifRequested = true;

      threshold = map(speedVal, ULTRASLOW_RESTORE, FAST,  300, 800);
      startThresHold = map(speedVal, ULTRASLOW_RESTORE, FAST,  1000, 3000);

      analogWrite(pwm, speedVal);


      if (speedVal == FAST)
      {
        emergencySpeed = true;
        emergencySpeedMillis = millis();
      }

      _server.send(200, "text/html", "confirmed SPEED req: " + iString);
    });
  }
}
