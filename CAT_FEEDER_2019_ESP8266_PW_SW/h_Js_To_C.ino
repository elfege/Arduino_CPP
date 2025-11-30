void getPowerState()
{
  togglePower();
  String state = !digitalRead(power) ? "CATFEEDER IS ON" : "CATFEEDER IS OFF";
  String stateSendHttp = !digitalRead(power) ? "deviceon" : "deviceoff";
  _server.send(200, "text/plain", state);
  delay(100);
  sendGET(stateSendHttp);// infor Atmega of status}
}

void getPowerStateOnly()
{
  String state = !digitalRead(power) ? "CATFEEDER IS ON" : "CATFEEDER IS OFF";
  //  String stateSendHttp = !digitalRead(power) ? "deviceon" : "deviceoff";
  _server.send(200, "text/plain", state);
  //  delay(100);
  //  sendGET(stateSendHttp);// infor Atmega of status}
}


void togglePower()
{
  if (digitalRead(power) == HIGH) // is off so it's turning on, so record this event's time so offTimer can be triggered
  {
    lastOnMillis = millis();
  }
  digitalWrite(power, !digitalRead(power));
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

  _server.on("/off", Off);
  _server.on("/on", On);
  _server.on("/reboot", Reboot);



  _server.on("/getMac", getMac);
  _server.on("/debug", getDataDebug);
  _server.on("/getTimeInfo", TimeInfos);
  _server.on("/clear", []()
  {
    debugData = "";

  });

}
