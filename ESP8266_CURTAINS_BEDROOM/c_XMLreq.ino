
void initXMLhttp()
{
  _server.on("/", []()
  {
    Serial.println("loading page...");
    _server.send_P(200, "text/html", MAIN_page);


  });

  _server.on("/open", On);
  _server.on("/close", Off);
  _server.on("/stop", Stop);

  _server.on("/logs", []()
  {
    logs = !logs;
  });

  _server.on("/refresh", Refresh);
  _server.on("/getMac", getMac);
  _server.on("/debug", getDataDebug);
  _server.on("/getTimeInfo", TimeInfos);
  _server.on("/clear", []()
  {
    debugData = "";

  });

  _server.on("/reset", Reset);
  _server.on("/hardreset", hardReset);


}
