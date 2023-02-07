
void initXMLhttp()
{
  _server.on("/", []()
  {
    term.println("loading page...");
    _server.send_P(200, "text/html", MAIN_page);
  });
  _server.on("/index2", []()
  {
    _server.send_P(200, "text/html", simplifiedPage);
  });
  _server.on("/getstate", []()
  {
    _server.send(200, "text/html", currentMode);
  });
  _server.on("/getClock", []()
  {
    String clockInfo = timeClient.getFormattedTime();
    _server.send(200, "text/html", clockInfo);
  });

  _server.on("/cool", cool);
  _server.on("/heat", heat);
  _server.on("/off", acOff);

  _server.on("/logs", []()
  {
    logs = !logs;
  });
  _server.on("/stopAll", []()
  {
    StopAll = true; // irreversible needs manual reset or succesfull OTA upload
    stopAllRequest = millis(); // so make it a temporary 1 minute request
    tempRequest = true;
  });

  _server.on("/refresh", Refresh);
  _server.on("/getMac", getMac);
   _server.on("/getTimeInfo", []()
  {
    String data = TimeInfos();
    _server.send(200, "text/html", data);

  });
  
  _server.on("/temp", getTemp);


  _server.on("/turbo", []()
  {
    turboIsOn = false;
    turbo();

  });

  _server.on("/reset", Reset);

  // temp range from slider
  int i = 0;
  int s = 90; // max speed value
  for (1; i <= s; i++) {
    //  term.println(i);
    String iString = String(i);
    lastSetPoint = iString;
    String str1 = "/tempRange";
    String str2 = str1 + "=" + iString;
    _server.on(str2, [i, iString]() {
      term.print("NEW TEMPERATURE: "); term.println(i);
      // update the global variable
      indexArray = getIndex(iString);
      term.println("indexArray = " + String(indexArray));
      newTemp = TempArray[indexArray - 1][99]; // now use this index to find the proper IR command to send
      term.println("newTemp = " + iString);
      SetTemp(newTemp, cmd);
      buildDebug("New Set Point: " + iString);
    });
  }

}
