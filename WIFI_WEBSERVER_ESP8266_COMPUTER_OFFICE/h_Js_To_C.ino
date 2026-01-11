void getPowerState()
{
  togglePower();
  String state = isOn() ? "COMPUTER IS ON" : "COMPUTER IS OFF";
  _server.send(200, "text/plain", state);
}

void getPowerStateOnly()
{
  String state = isOn() ? "COMPUTER IS ON" : "COMPUTER IS OFF";
  _server.send(200, "text/plain", state);
}


void togglePower()
{
  if (isOn())
  {
    off();
  }
  else
  {
    on();
  }
}

void hardreboot()
{
  digitalWrite(switchPin, 1);
  delay(5000);
  digitalWrite(switchPin, 0);
  delay(5000);
  digitalWrite(switchPin, 1);
  delay(500);
  digitalWrite(switchPin, 0);
}
void shutdown()
{
  digitalWrite(switchPin, 1);
  delay(5000);
  digitalWrite(switchPin, 0);
}

void initXMLhttp() {
  _server.on("/", []()
  {
    _server.send_P(200, "text/html", MAIN_page);

  });

  _server.on("/power", getPowerState);// toggles
  _server.on("/getpowerstate", getPowerStateOnly); // doesn't toggle, just refreshes. .
  _server.on("/reset",  []()
  {
    Serial.println("RESET CMD RECEIVED.. reseting in 1.5 second");
    delay(1500);
    Reset();

  });

  _server.on("/togglepower", togglePower);
  _server.on("/on", on);
  _server.on("/off", off);
  _server.on("/hardreboot", hardreboot);
  _server.on("/shutdown", shutdown);
  _server.on("/reset", Reset);



  _server.on("/getMac", getMac);
  _server.on("/debug", getDataDebug);
  _server.on("/getTimeInfo", TimeInfos);
  _server.on("/clear", []()
  {
    debugData = "";

  });

}
