void loop()
{

  // if (millis() - millisLogs >= 500)
  // {
  //   // term.println("A => " + String(analogRead(wheel)));
  //   // term.println("D => " + String(digitalRead(wheel)));
  //   // millisLogs = millis();
  // }
  mainHandler();
}

void mainHandler()
{
  ArduinoOTA.handle();
  // WiFiHandler();
  httpHandler();
  logs();
  blinker();
  motorsTimer();
  chasDeformHandler();
  pusherRetractCheck();
  cosinusHandler();
  st::Everything::run();
  wheelSpinning();
  hubRefreshHandler();
  problemSolver();
  yield();
}

void hubRefreshHandler()
{
  if (millis() - lastRefresh > 120000)
  {
    getStates(true);
  }
}
