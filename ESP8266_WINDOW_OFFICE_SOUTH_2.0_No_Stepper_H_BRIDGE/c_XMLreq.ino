/* IDENTICAL TO THE OTHER WINDOW USING SAME THREADED ROD BASED ACTUATOR */

void initXMLhttp()
{
  _server.on("/", []()
             { _server.send_P(200, "text/html", MAIN_page); });
  _server.on("/index2", []()
             { _server.send_P(200, "text/html", simplifiedPage); });

  _server.on("/open", Open);
  _server.on("/close", Close);

  _server.on("/clear", []()
             {
               debugData = "";
               previousDebugData = "";
               previousDebugDataCmd = "";
               previousDebugDataCmdBits = "";
               previousDebugDataCmdLevel = ""; });
  _server.on("/getstate", []()
             {
    String state = isOpen() ? "window is open" : "window is closed";
    _server.send(200, "text/html", state);
    if (isOpen())
    {
      String state = isFullyOpen() ? "WINDOW IS FULLY OPEN" : "WINDOW IS PARTIALLY OPEN";
      _server.send(200, "text/html", state);
    } });
  _server.on("/getClock", []()
             {
    String clockInfo = timeClient.getFormattedTime();
    _server.send(200, "text/html", clockInfo); });
  _server.on("/plusten", []()
             {
               term.println("+10");
               speedVal = speedVal + 10;
               ULTRASLOW = ULTRASLOW + 10;
               if (opening || closing)
               {
                 term.println("analogWrite(pwm, " + String(ULTRASLOW) + ")");
                 analogWrite(pwm, ULTRASLOW);
               } });
  _server.on("/minusten", []()
             {
    term.println("-10");
    speedVal = speedVal - 10;
    ULTRASLOW = ULTRASLOW - 10;
    if (opening || closing) {
      term.println("analogWrite(pwm, "+String(ULTRASLOW)+")");
      analogWrite(pwm, ULTRASLOW);
    } });

  _server.on("/calibrate", []()
             { calibrate(); });

  _server.on("/logs", []()
             {
    logs = !logs;
    if (!logs)
    {
      debugData = "";
      buildDebug("DATA RESET REQUESTED");
    } });

  _server.on("/stop", []()
             {
    //    stoppedRequestedByUser = true;
    Stop(); });
  _server.on("/refresh", Refresh);
  _server.on("/getMac", getMac);
  _server.on("/getIP", getIP);
  _server.on("/getSSID", getSSID);
  _server.on("/getRSSI", getRSSI);
  _server.on("/speed", getSpeed);
  _server.on("/resetRTCVars", resetRTCVars);

  _server.on("/debug", getDataDebug);
  _server.on("/getTimeInfo", []()
             {
               String data = TimeInfos();
               _server.send(200, "text/html", data); });
  _server.on("/reset", Reset);
  _server.on("/hardreset", hardReset);

  _server.on("/clear", []()
             { debugData = ""; });

  // speed range from slider
  int i = 0;
  int s = 100; // max speed value
  for (1; i <= s; i++)
  {
    //  term.println(i);
    String iString = String(i);
    String str1 = "/speedRange";
    String str2 = str1 + "=" + iString;
    _server.on(str2, [i, iString]()
               {
                 // update the global variable
                 speedVal = map(i, 1, 100, ULTRASLOW, FAST);
                 term.print("NEW SPEED: ");
                 term.println(i);
                 term.print("NEW SPEED (bits): ");
                 term.println(speedVal);
                 lastSpeedVal = speedVal;
                 modifRequested = true;

                 term.println("10 bits speed value = " + String(speedVal));

                 analogWrite(pwm, speedVal);
                 term.println("pwm set to " + String(speedVal));

                 speedOverride = true;
                 if (speedVal == FAST)
                 {
                   emergencySpeed = true;
                   emergencySpeedMillis = millis();
                 }

                 _server.send(200, "text/html", "confirmed SPEED req: " + iString); });
  }
}
