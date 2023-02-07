void workLoad (void) ;                // prototype is required
void watchdog (void) ;                // prototype is required


void CPU_0_task1( void * parameter )
{
  for (;;) {
    //Serial.println("encoders");

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
        //        Serial.println("cl++");
      }
      if (B4R != AFR)
      {
        cr++;
        //        Serial.println("cr++");
      }


      workLoad();
    }
    //watchdog();
    delay(50);
  }
}

void watchdog(void)
{
  //Serial.println("watchdog");
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
    while (digitalRead(watchDogPin) == 0 && thisElapse < 5000)
    {
      thisElapse = millis() - start;
      //Serial.println(thisElapse);
    }
    if (digitalRead(watchDogPin) == 1 || stopAll)
    {
      //Serial.println("watchdog ok");
    }
    else
    {
      Serial.println("***************************************************");
      Serial.println("                  WATCHDOG ALERT                  ");
      Serial.println("***************************************************");
      esp_restart();
    }
  }
}
