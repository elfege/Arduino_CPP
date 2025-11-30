void getPowerState()
{
  togglePower();
  String state = digitalRead(power) ? "KOBRA IS ON" : "KOBRA IS OFF";
  _server.send(200, "text/plain", state);
}
void getChargeState()
{
  toggleCharge();
  String state = digitalRead(CHARGER) ? "KOBRA IS NOT CHARGING" : "KOBRA IS CHARGING";
  _server.send(200, "text/plain", state);
}
void getChargeStateOnly()
{
  String state = digitalRead(CHARGER) ? "KOBRA IS NOT CHARGING" : "KOBRA IS CHARGING";
  _server.send(200, "text/plain", state);
}
void getPowerStateOnly()
{
  String state = digitalRead(power) ? "KOBRA IS ON" : "KOBRA IS OFF";
  _server.send(200, "text/plain", state);
}


void togglePower()
{
  digitalWrite(power, !digitalRead(power));
  delay(1000);
  remoteReset();
}

void poweron()
{
  On();
}
void poweroff()
{
  Off();
  camoff(); // always when turning off, otherwise battery might not charge properly
}

void camon()
{
  digitalWrite(CAM, 1); // high for 1 second, activate PNP transistor
  unsigned long strt = millis();
  while (millis() - strt < 1000)
  {
    loop();
  }
  digitalWrite(CAM, 0); // release PNP transistor
  camIsOn = true;
}
void camoff()
{
  digitalWrite(CAM, 1); // high for 4 seconds, activate PNP transistor
  unsigned long strt = millis();
  while (millis() - strt < 4500)
  {
    loop();
  }
  digitalWrite(CAM, 0); // release PNP transistor
  camIsOn = false;
}

void toggleCharge()
{
  digitalRead(CHARGER) ? chargeon() : chargeoff();
}

void initXMLhttp() {
  _server.on("/", []()
  {
    _server.send_P(200, "text/html", MAIN_page);

  });

  _server.on("/power", getPowerState);// toggles
  _server.on("/poweron", poweron);// for ESP32 controller only
  _server.on("/poweroff", poweroff);// for ESP32 controller only
  _server.on("/camon", camon);// to relay
  _server.on("/camoff", camoff);// to relay
  _server.on("/getpowerstate", getPowerStateOnly); // doesn't toggle, just refreshes.
  _server.on("/charge", getChargeState);// toggles
  _server.on("/getchargestate", getChargeStateOnly); // doesn't toggle, just refreshes.
  _server.on("/chargeoff", []()
  {
    Serial.println("Received charge OFF cmd from http");
    chargeoff();
  });
  _server.on("/chargeon", []()
  {
    Serial.println("Received charge ON cmd from http");
    chargeon();
  });

  _server.on("/reset", Reset);
  _server.on("/remotereset", remoteReset);

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
