

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
  _server.on("/failsafe", []()
  {
    failSafeBoot = failSafeBoot == 1 ? 0 : 1;

    unsigned long s = millis();
    unsigned long c = millis();
    if (failSafeBoot == 1)
    {
      term.println("FAIL SAFE BOOT...");
    }
    else {
      term.println("REBOOT IN NORMAL MODE");
    }
    while (millis() - s < 3000)
    {
      if (millis() - c >= 200)
      {
        term.print(".");
        c = millis();
      }
      _server.handleClient(); // JS webpage and wifi terminal
      term.handleClient();// WiFi terminal (second call, necessary)
      state.saveToRTC();
    }

    ESP.reset(); // esp.reset is hard reset (don't call hardReset() from here! It'd reset the failSafeBoot RTC var to 0!

  });

  _server.on("/close", Close);
  _server.on("/forcestop", simpleStop);

  _server.on("/checkreload", []()
  {
    String var = reloadPage ? "reloadthepage" : "";
    _server.send(200, "text/html", var);
    reloadPage = false; // only once after fresh boot
  });

  _server.on("/learnplus", []()
  {
    int prev = ULTRASLOW;
    ULTRASLOW += 1;
    analogWrite(pwm, ULTRASLOW);
    term.println("ULTRASLOW INCREASED FROM " + String(prev) + " TO " + String(ULTRASLOW));

  });
  _server.on("/learnminus", []()
  {
    int prev = ULTRASLOW;
    ULTRASLOW -= 1;
    analogWrite(pwm, ULTRASLOW);
    term.println("ULTRASLOW DECREASED FROM " + String(prev) + " TO " + String(ULTRASLOW));

  });

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
    _server.send(200, "text/html", windowLastState);
  });
  _server.on("/getClock", []()
  {
    String clockInfo = timeClient.getFormattedTime();
    _server.send(200, "text/html", clockInfo);
  });

  _server.on("/forceclose", []()
  {
    forceClose(0);
  });
  _server.on("/forceopen", []()
  {
    forceOpen(0);
  });
  _server.on("/logs", []()
  {
    logs = !logs;
    if (!logs)
    {
      debugData = "";
      sendHub("DATA RESET REQUESTED");
    }
  });

  _server.on("/stop",  []()
  {
    Stop("HTTP request");
  });

  _server.on("/refresh", Refresh);
  _server.on("/getMac", getMac);
  _server.on("/getIP", getIP);
  _server.on("/getSSID", getSSID);
  _server.on("/getRSSI", getRSSI);
  _server.on("/speed", getSpeed);

  _server.on("/getTimeInfo", []()
  {
    String data = upTime();
    _server.send(200, "text/html", data);

  });

  _server.on("/reset", Reset);
  _server.on("/hardreset", hardReset);
  _server.on("/rtcreset", resetRTCVars);

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
      // update the global variable
      speedVal = map(i, 0, 100, ULTRASLOW_RESTORE, FAST);
      term.print("NEW SPEED: "); term.println(i);
      term.print("NEW SPEED (bits): "); term.println(speedVal);
      lastSpeedVal = speedVal;
      speedOverride = true;

//      if (closing || opening)
//      {
        term.println("analogWrite(pwm, " + String(speedVal) + ");");
        analogWrite(pwm, speedVal);
//      }

      modifRequested = true;
      term.println("donotAcceptCmdsFromHubs set to FALSE by HTTP request");
      donotAcceptCmdsFromHubs = 0;

      _server.send(200, "text/html", "confirmed SPEED req: " + iString);
    });
  }
}
