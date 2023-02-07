void workLoad (void) {

  if (millis() > 15000)
  {

    if (!watchdogInitOk)
    {
      Serial.println("Time since last boot = " + String(millis()) + " milliseconds");
      Serial.println("WATCHDOG INIT SUCCESSFUL *************************************");
      watchdogInitOk = true;
    }
    unsigned long start = millis();
    int thisElapse = 0;

    while (wtdpin == 0 && thisElapse < 60000)
    {
      thisElapse = millis() - start;
      //      Serial.print("PIN STATE = ");Serial.println(pinstate);
      //      Serial.print("thisElapse = ");Serial.println(thisElapse);
      //      delay(1);
    }
    if (wtdpin == 1 || stopAll)
    {
      //      Serial.println("watchdog ok");
    }
    else
    {
      Serial.println("***************************************************");
      Serial.println("      ELAPSED TIME = " + String(thisElapse));
      Serial.println("***************************************************");
      Serial.println("                  WATCHDOG ALERT                  ");
      Serial.println("***************************************************");
      Reset();
    }
  }
}
