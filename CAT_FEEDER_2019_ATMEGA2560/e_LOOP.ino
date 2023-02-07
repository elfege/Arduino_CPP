
//******************************************************************************************
//Arduino Loop() routine
//******************************************************************************************
void loop()
{
  master();
}

void master() {

 if (debug && millis() - lastDebugMillis > 500)
  {
    canLoaded();
    C1empty();
    lastDebugMillis = millis();
  }
  webserver();
  smartthing.run();

  if (millis() - lastCheck > 1000)
  {
    if (pwstate != lastState)
    {
      smartthing.send("switch " + pwstate);
      lastState = pwstate;
    }
  }
  if (millis() - lastOnMillis > offTimer * 60 * 1000 && !digitalRead(power))
  {
    Serial.println("time is up, turning off the cat feeder"); // IMPORTANT SAFETY
    Off();
  }
  if (debug)
  {
    log();
  }
  if (STOP)
  {
    stopOpener();
    if ( millis() - millisStop > 5000)
    {
      // after X secs cancel STOP boolean
      STOP = false;
    }
  }

  if (millis() - millisRefresh > 240000)
  {
    millisRefresh = millis();
    Refresh();
  }
}

void log()
{

  Serial.println("pressed = " + String(digitalRead(pressed)));
   delay(500);
}
