

void initXMLhttp()
{
  _server.on("/", []()
  {
    _server.send_P(200, "text/html", MAIN_page);

  });

  _server.on("/on", []() {
    On();
    onRequestedByUser = true;
    
    
  });
  _server.on("/off", Off);

  _server.on("/nonStopPurge", nonStopPurge);

  _server.on("/refresh", Refresh);
  _server.on("/getMac", getMac);
  _server.on("/getIP", getIP);
  _server.on("/getSSID", getSSID);
  _server.on("/getRSSI", getRSSI);
  _server.on("/debug", getDataDebug);
  _server.on("/getTimeInfo", TimeInfos);
  _server.on("/getwetval", []()
  {
    lastHwetv = analogRead(A0);
    String strval = String(lastHwetv);
    _server.send(200, "text/html", strval);
  });

   _server.on("/getClock", []()
  {
    String clockInfo = timeClient.getFormattedTime();
    _server.send(200, "text/html", clockInfo);
  });

  _server.on("/getstate", []()
  {
    _server.send(200, "text/html", wetState); // update wet/dry status based on last read (done by loop() calling wet())
  });
  _server.on("/reset", Reset);

  _server.on("/clear", []()
  {
    debugData = "";

  });


}
