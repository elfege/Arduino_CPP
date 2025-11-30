

void initXMLhttp()
{
  _server.on("/", []()
  {
    _server.send_P(200, "text/html", MAIN_page);

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
    String state = isOpen() ? "WINDOW IS OPEN" : "WINDOW IS CLOSED";
    _server.send(200, "text/html", state);
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
  _server.on("/speed", getSpeed);

  _server.on("/debug", getDataDebug);
  _server.on("/getTimeInfo", TimeInfos);

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
    //  Serial.println(i);
    String iString = String(i);
    String str1 = "/speedRange";
    String str2 = str1 + "=" + iString;
    _server.on(str2, [i, iString]() {
      Serial.print("NEW SPEED: "); Serial.println(i);
      // update the global variable
      speedVal = map(i, 0, 100, ULTRASLOW, FAST);
      lastSpeedVal = speedVal;
      modifRequested = true;

      threshold = map(speedVal, FAST, ULTRASLOW, 300, 800);
      startThresHold = map(speedVal, FAST, ULTRASLOW, 1000, 3000);

      //analogWrite(pwm, speedVal);


      if (speedVal == FAST)
      {
        emergencySpeed = true;
        emergencySpeedMillis = millis();
      }

      _server.send(200, "text/html", "confirmed SPEED req: " + iString);
    });
  }
}
