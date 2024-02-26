
void initXMLhttp()
{
  _server.on("/", []()
  {
    Serial.println("loading page...");
    _server.send_P(200, "text/html", MAIN_page);


  });
  
  _server.on("/toggle", getPowerState);// toggles
  _server.on("/getpowerstate", getPowerStateOnly); // doesn't toggle, just refreshes.
  _server.on("/select", Select);
  _server.on("/appletvoff", appleTvOff);
  _server.on("/appletvmenu", appleTvMainMenu);
  _server.on("/appletvplaypause", appletvplaypause);
  _server.on("/down", down);
  _server.on("/up", up);
  _server.on("/left", left);
  _server.on("/right", right);
  


  _server.on("/logs", []()
  {
    logs = !logs;
  });


  _server.on("/refresh", Refresh);
  _server.on("/getMac", getMac);
  _server.on("/getTimeInfo", TimeInfos);
  _server.on("/clear", []()
  {
    debugData = "";

  });

  _server.on("/reset", Reset);

}
