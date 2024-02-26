void loop()
{
  if (!StopAll)
  {
    master();
  }
  else
  {
    ArduinoOTA.handle(); // also handled by smartthings ESP8266 library
  }
  if (millis() - millisHeartBit >= 1000)
  {
    //    term.println("state = "+last_state);
    millisHeartBit = millis();
    digitalWrite(LED, !digitalRead(LED));
  }
  yield();
  delay(10);
}

void master()
{
  servers();

  if (!is_on() && last_state == "on")
  {
    term.println("UPDATE 1");
    last_state = "off";
    send_data("switch off");
    send_data("computer sleeping");
    send_data("status sleeping");
  }
  if (is_on() && last_state == "off")
  {
    term.println("UPDATE 2");
    last_state = "on";
    send_data("switch on");
    send_data("computer running");
    send_data("status running");
  }

  // refresh statuses every x minutes
  if (millis() - previousMillis > DELAY_SEC * 1000)
  {
    previousMillis = millis();
    refresh();
    userOverride = false; // reset this value
    String var = "Override Inactive";
    _server.send(200, "text/html", var);
    send_data(var);
  }
  if (millis() - previousMillis_override_RESET > OTHER_DELAY * 1000)
  {
    previousMillis_override_RESET = millis();
    userOverride = false; // reset this value
    String var = "Override Inactive";
    _server.send(200, "text/html", var);
    send_data(var);
  }
}

void servers()
{
  _server.handleClient();
  smartthing.run();
  term.handleClient();
}
