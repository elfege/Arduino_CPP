

void initXMLhttp()
{
  _server.on("/", []()
  {
    term.println("Loading main page");
    _server.send_P(200, "text/html", MAIN_page);

  });

  _server.on("/power", []()
  {
    term.println("POWER CMD RECEIVED");
    onOff();
  });
  _server.on("/logs", []()
  {
    debug = !debug;
    String message = debug ? "DEBUG ACTIVE" : "DEBUG INACTIVE";
    term.println(message);
  });
  _server.on("/modeswitch", nextMode);
  _server.on("/cool", cool);

}
