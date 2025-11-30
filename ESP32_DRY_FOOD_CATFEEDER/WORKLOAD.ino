void workLoad (void) {

  if (millis() > 15000)
  {

    if (!watchdogInitOk)
    {
      term.println("Time since last boot = " + String(millis()) + " milliseconds");
      term.println("WATCHDOG INIT SUCCESSFUL *************************************");
      watchdogInitOk = true;
    }
    unsigned long start = millis();
    int thisElapse = 0;

    while (wtdpin == 0 && thisElapse < 60000)
    {
      thisElapse = millis() - start;
      //      term.print("PIN STATE = ");term.println(pinstate);
      //      term.print("thisElapse = ");term.println(thisElapse);
      //      delay(1);
    }
    if (wtdpin == 1 || stopAll)
    {
      //      term.println("watchdog ok");
    }
    else
    {
      term.println("***************************************************");
      term.println("      ELAPSED TIME = " + String(thisElapse));
      term.println("***************************************************");
      term.println("                  WATCHDOG ALERT                  ");
      term.println("***************************************************");
      Reset();
    }
  }
}
