void workLoad (void) ;                // prototype is required
void watchdog (void) ;                // prototype is required


void CPU_0_task1( void * parameter )
{
  for (;;) {
    //term.println("encoders");

    while (mvt && noturn || makeEncHappen) // only if no mvt, and this is not a turn request, unless otherwise requested (makeEncHappen)
    {

      AFL = digitalRead(LEFT);
      AFR = digitalRead(RIGHT);
      delayMicroseconds(500);
      B4L = digitalRead(LEFT);
      B4R = digitalRead(RIGHT);

      if (B4L != AFL)
      {
        cl++;
        //        term.println("cl++");
      }
      if (B4R != AFR)
      {
        cr++;
        //        term.println("cr++");
      }


      workLoad();
    }
    watchdog();
    delay(50);
  }
}

void watchdog(void)
{
  //term.println("watchdog");
  if (millis() > 15000)
  {
    if (!watchdogInitOk)
    {
      term.println("Time since last boot = " + String(millis()) + " milliseconds");
      term.println("************************************************");
      term.println("              WATCHDOG INITIALIZED              ");
      term.println("************************************************");
      watchdogInitOk = true;
    }

    unsigned long start = millis();
    int thisElapse = 0;
    while (digitalRead(watchDogPin) == 0 && thisElapse < 5000)
    {
      thisElapse = millis() - start;
      //term.println(thisElapse);
    }
    if (digitalRead(watchDogPin) == 1 || stopAll)
    {
      //term.println("watchdog ok");
    }
    else
    {
      term.println("***************************************************");
      term.println("                  WATCHDOG ALERT                   ");
      term.println("***************************************************");
      //esp_restart();
    }
    if(millis() - loopTime > 60000 && !stopAll)
    {
      term.println("***************************************************");
      term.println("                  WATCHDOG TIMER ALERT             ");
      term.println("***************************************************");
      esp_restart();
    }
  }
}
