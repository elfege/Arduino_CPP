void loop() {

  if (!stopAll)
  {
    master();
    if (millis() - ledMillis >= 1000)
    {
      digitalWrite(LED, !digitalRead(LED));
      ledMillis = millis();
    }
    if (millis() - debugMillis >= 5000)
    {
      logs();
      debugMillis = millis();
    }
  }
  ArduinoOTA.handle();

  delay(1);
}

void master()
{

  _server.handleClient();
  st::Everything::run();
  term.handleClient();

term.println("analogRead(cooling) = "+String(analogRead(cooling)));
  delay(100);

}

void logs()
{
  if (debug || oneDebug)
  {
    term.println("*********************************************");
    term.println("analogRead(cooling) = " + String(analogRead(cooling)));
    term.println("analogRead(heating) = " + String(analogRead(heating)));
    term.println("analogRead(dehumid) = " + String(analogRead(dehumid)));
    term.println("analogRead(fanonly) = " + String(analogRead(fanonly)));
    term.println("analogRead(fanlow) = " + String(analogRead(fanlow)));
    term.println("analogRead(fanmed) = " + String(analogRead(fanmed)));
    term.println("analogRead(fanhigh) = " + String(analogRead(fanhigh)));
    term.println("analogRead(fanauto) = " + String(analogRead(fanauto)));

    term.println("----------------------------------------------");
    term.println("digitalRead(cooling) = " + String(digitalRead(cooling)));
    term.println("digitalRead(heating) = " + String(digitalRead(heating)));
    term.println("digitalRead(dehumid) = " + String(digitalRead(dehumid)));
    term.println("digitalRead(fanonly) = " + String(digitalRead(fanonly)));
    term.println("digitalRead(fanlow) = " + String(digitalRead(fanlow)));
    term.println("digitalRead(fanmed) = " + String(digitalRead(fanmed)));
    term.println("digitalRead(fanhigh) = " + String(digitalRead(fanhigh)));
    term.println("digitalRead(fanauto) = " + String(digitalRead(fanauto)));
  }
  oneDebug = false;
}
