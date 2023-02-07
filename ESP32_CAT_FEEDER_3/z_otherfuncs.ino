void blinker()
{
  if (paused && millis() - previousMillisBlink > 1500)
  {
    Blink(3, 500);
    previousMillisBlink = millis();
    term.println("PAUSED");
  }
  else if ((problem || deformation) && millis() - previousMillisBlink > 50)
  {
    digitalWrite(LED, !digitalRead(LED));
    previousMillisBlink = millis();
  }
  else if (!feeding && millis() - previousMillisBlink > 5000)
  {
    digitalWrite(LED, 1);
    delay(50);
    digitalWrite(LED, 0);
    previousMillisBlink = millis();
  }

  /* HEARTBEAT */
  if (feeding && millis() - previousMillisBlink >= 500)
  {
    digitalWrite(LED, !digitalRead(LED));

    previousMillisBlink = millis();
  }
  if (feeding && millis() - heartbeat >= 700)
  {
    digitalWrite(LED, !digitalRead(LED));
    heartbeat = millis();
  }
  /* END HEARTBEAT */
}
void Blink(int times, int lapse)
{
  int c = 0;
  while (c < times)
  {
    if (!problem && !deformation)
      digitalWrite(LED, 1);
    delay(lapse);
    if (!problem && !deformation)
      digitalWrite(LED, 0);
    delay(lapse);
    c++;
  }
}

void cosinusHandler()
{
 
  if (cosinuspulling && millis() - cosinusMillis >= 3000)
  {
    cosinusPushStep(1, 30);
  }
}

// function that makes sure that LN298 never stays active more than 5 minutes at a time. 
void motorsTimer() 
{
  if (millis() - lastActionMillis > 5 * 60 * 1000 && somethingIsOn())
  {
    term.println("MOTOR TIMER SHUT DOWN");
    stop();
    lastActionMillis = millis();
  }
}

bool somethingIsOn()
{

  return digitalRead(vertPush_A) || digitalRead(vertPush_B) || digitalRead(ACTUATOR_A) || digitalRead(ACTUATOR_B) || digitalRead(OPENER_A) || digitalRead(OPENER_B) || digitalRead(COSINUS_A) || digitalRead(COSINUS_B);
}

void chasDeformHandler()
{
  if (deformCorrAttempts > 6)
  {
    term.println("THE ARM IS STUCK");
    problem = true;
    return;
  }
  if (!deformOverride && pushing && !stopped)
  {
    unsigned long Start = millis();
    if (pulling && deformed())
    {
      deformCorrAttempts++;
      pull();
      term.println("MANAGING CHASSIS DEFORMATION WHILE PULLING");
      Start = millis();
      while (deformed() && millis() - Start < 5000 && !stopped)
      {
        httpHandler(); // can't run mainHandler from here because this function is called from mainHandler()
      }
      push(); // resume pushing
    }

    if (!deformOverride && pulling && deformed())
    {
      deformCorrAttempts++;
      push();
      term.println("MANAGING CHASSIS DEFORMATION WHILE PULLING");
      Start = millis();
      while (deformed() && millis() - Start < 5000 && !stopped)
      {
        httpHandler(); // can't run mainHandler from here because this function is called by it
      }
      pull(); // resume pulling
      Start = millis();
      while (millis() - Start < 4000 && !stopped) // give it a chance to push through
      {
        httpHandler();
      }
    }
  }
}

void pusherRetractCheck()
{
  unsigned long Start;
  if (pusherRetracted() && !adjusting && !feeding) // always keep the limit switch free of pressure
  {
    problem = false;
    adjusting = true;
    masterisout = false;
    term.println("ADJUSTING ARM RETRACTION");
    push();

    Start = millis();
    while (pusherRetracted() && !stopped /* && Start - millis() < 25000*/)
    {
      term.println("ARM RETRACTION");
      httpHandler(); // can't run mainHandler from here because this function is called from mainHandler()
      // term.println("pusherRetracted() = " + String(pusherRetracted() ? "PRESSED" : "RELEASED"));
    }
    simpleStop();
    adjusting = false;
    masterisout = true;
  }
}

unsigned long cosStart = millis();
bool cosScanPulled = false;
void cleanOpener() {
  verticalDownStep(71, 10);
  delay(510);
  canBackWard();
  unsigned long Start = millis();
  cosStart = millis();

  while (millis() - Start < 2500 && !stopped)
  {
    if (paused) {
      pauseHandler("canBackWard");
      Start = millis();
    }
    mainHandler();
    cosinusScan();
  }
  canForward();
  Start = millis();
  while (millis() - Start < 2500 && !stopped)
  {
    if (paused) {
      pauseHandler("canForward");
      Start = millis();
    }
    mainHandler();
    cosinusScan();// rubs the head against the cleaning brush
  }
  canBackWard();
  Start = millis();
  while (millis() - Start < 2500 && !stopped)
  {
    if (paused) {
      pauseHandler("canForward");
      Start = millis();
    }
    mainHandler();
    cosinusScan();// rubs the head against the cleaning brush
  }
  stop();
  stopped = false;
  verticalPull();
  stopped = false;
  term.println("DONE CLEANING HEAD");
}

void cosinusScan() { // rubs the head against the cleaning brush
  if (millis() - cosStart > 1500) {
    if (cosScanPulled)
    {
      cosinusPush();
      cosScanPulled = false;
    }
    else {
      cosinusPull();
      cosScanPulled = true;
    }
    cosStart = millis();
  }
}

void problemSolver(){
  if(problem && millis() - problemMillis > 2 * 60 * 60 * 1000)
  {
    Reset();
  } 
}

void pauseHandler(String func)
{
  if (paused)
  {
    simpleStop();
    waspaused = true;
    term.println("CAT FEEDER IS PAUSED");

    // blinker() takes care of specific paused blinking signal

    mainHandler();

  }
  if (waspaused) {
    waspaused = false;

    if (func == "pull")
    {
      pull();
    }
    else if (func == "push")
    {
      push();
    }
    else if (func == "mainArmPull")
    {
      mainArmPull();
    }
    else if (func == "pullMasterRelease")
    {
      pullMasterRelease();
    }
    else if ("smallPush")
    {
      smallPush();
    }
    else if ("smallPull")
    {
      smallPull();
    }
    else if ("canForward")
    {
      canForward();
    }
    else if ("canBackWard")
    {
      canBackWard();
    }
    else if ("lock")
    {
      lock();
    }
    else if ("unlock")
    {
      unlock();
    }
    else if ("verticalPush")
    {
      verticalPush();
    }
    else if ("verticalPull")
    {
      verticalPull();
    }
    else if ("verticalDownStep")
    {
      //verticalDownStep(); // requires arguments (int, int)
    }
    else if ("verticalUpStep")
    {
      // verticalUpStep();
    }
    else if ("verticalUp")
    {
      verticalUp();
    }
    else if ("verticalDown")
    {
      verticalDown();
    }
    else if ("verticalStop")
    {
      verticalStop();
    }
    else if ("cosinusPush")
    {
      cosinusPush();
    }
    else if ("cosinusPull")
    {
      cosinusPull();
    }
    else if ("cosinusPushStep")
    {
      //cosinusPushStep();
    }
    else if ("cosinusPullStep")
    {
      //    cosinusPullStep();
    }
    else if ("cosinusStop")
    {
      cosinusStop();
    }
  }
}

void Reset()
{
  term.println("*****************************************************************************");
  term.println("                               RESETTING ESP                                ");
  term.println("*****************************************************************************");
  unsigned long s = millis();
  while (millis() - s < 100)
  {
    httpHandler();
  }
  esp_restart();
}
