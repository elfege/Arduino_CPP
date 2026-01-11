
void initXMLhttp()
{
  _server.on("/", []()
  {
    Serial.println("loading page...");
    _server.send_P(200, "text/html", MAIN_page);
    

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
    stopAllRequest = millis(); // so make it a temporary request of 1 minute
    tempRequest = true;
  });

  _server.on("/refresh", Refresh);
  _server.on("/getMac", getMac);
  _server.on("/debug", getDataDebug);
  _server.on("/getTimeInfo", TimeInfos);
  _server.on("/temp", getTemp);
  _server.on("/clear", []()
  {
    debugData = "";
    
  });

  _server.on("/reset", Reset);

  // temp range from slider
  int i = 0;
  int s = 90; // max speed value
  for (1; i <= s; i++) {
    //  Serial.println(i);
    String iString = String(i);
    String str1 = "/tempRange";
    String str2 = str1 + "=" + iString;
    _server.on(str2, [i, iString]() {
      Serial.print("NEW TEMPERATURE: "); Serial.println(i);
      // update the global variable
      indexArray = getIndex(iString);
      Serial.println("indexArray = " + String(indexArray));
      newTemp = TempArray[indexArray - 1][99]; // now use this index to find the proper IR command to send
      Serial.println("newTemp = " + iString);
      SetTemp(newTemp, cmd);
      buildDebug("New Set Point: " + iString);
    });
  }

}
