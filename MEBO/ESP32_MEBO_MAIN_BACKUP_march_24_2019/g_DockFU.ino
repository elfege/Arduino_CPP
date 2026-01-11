
void dockingFunctions()
{

  gotsignal = false;
  gotstation = false;
  unsigned long Startrightscan;
  unsigned long StartleftScan;

  int del1 = 80;
  int del2 = 50;

  int cycles = 0;
  int cycleslimit = 2;

  // move until you get an IR signal
  unsigned long StartSearch = millis();

  while (!gotstation && !STOPALL && millis() - StartSearch < 120000)
  {
    loop(); // starts euristics IR receiver

    int countStuck = 0;
    int turndist = 10;
    int Length = 200;
    int delsearch = 50;
    int sPeeD = 170;
    int startingSpeed = sPeeD;
    rearleft = false;
    frontright = false; // lineTest() globals

    int timeOff = 3000;

    makeEncHappen = true;

    goTowardStation(); /// GO toward station trying to catch A signal

    STOP();

    getStationSignal();// move and try to catch the specific Station's signal

    STOP();
    loop();
  }

  Serial.print("LOOP ENDED BECAUSE: ");
  if ((!gotsignal || !gotstation) && !STOPALL) {
    if (GotStuck) {
      Serial.println("GOT STUCK");
      GotStuck = false;
    }
    else if (millis() - StartSearch > 120000)
    {
      Serial.println("TIME IS UP!");
    }
    else if (gotstation)
    {
      Serial.println("STATION WAS FOUND!");
    }
    else       {
      Serial.println("UNKNOWN REASON...");
    }
    Serial.println("");
    obstacleMgt();
  }

  if (gotstation)
  {
    Serial.println("/****************************************/");
    Serial.println("         STATION HAS BEEN FOUND           ");
    Serial.println("/****************************************/");

    unsigned long mlr = millis();
    makeEncHappen = true;
    rvs(155, 0);
    while (found && millis() - mlr < 3000 && !STOPALL && !rearOBS() && !sideOBS()) {
      loop();
    }
  }

}

void goTowardStation() {

  makeEncHappen = true; // INCREMENT ENCODERS
  noStuck = true; // DON'T READ STUCK VALUES
  encodersOk = false; // don't correct trajectories

  unsigned long sfjd = 0;
  unsigned long fdee = 0;
  rightDone = false;
  leftDone = true;
  GotStuck = false;
  int cycles = 0;
  int cycleslimit = 2;
  int rundist = 20;

  int spLimitUp = 170;
  int spLimitDo = 140;

  simpleRvs();
  String debug = "";

  while (!gotsignal && !STOPALL && !rearOBS() && !GotStuck)
  {

    Serial.println(cycles);

    if (cycles > cycleslimit) {
      cycles = 0;
      simpleFwd();
      delay(1000);
    }
    else if (cycles <= 0) {
      simpleRvs();
    }

    // natural selection toward signal


    ledcWrite(pwmChannelR, spLimitUp);
    ledcWrite(pwmChannelL, spLimitDo);
    while (!Signal && cr < rundist && !STOPALL)
    {
      debug = "RR " + String(cr);
      obstacleMgt();
      Serial.println(debug);
      loop();
    }

    cr = 0;
    
    ledcWrite(pwmChannelL, spLimitUp);
    ledcWrite(pwmChannelR, spLimitDo);
    while (!Signal && cl < rundist && !STOPALL)
    {
      debug = "LL " + String(cl);
      Serial.println(debug);
      obstacleMgt();
      loop();
    }
    cycles++;
    cl = 0;
    
    loop();
    if (gotsignal)
    {
      Serial.println("***************SIGNAL!******************");
      unsigned long mlr = millis();
      rvs(150, 0);
      while (Signal && !gotstation && millis() - mlr < 3000 && !STOPALL && !rearOBS() && !sideOBS()) {
        Serial.println("O");
        obstacleMgt();
        loop();
      }
    }

    loop();
  }
  noStuck = false;
  makeEncHappen = false;
  encodersOk = true;
}

void  getStationSignal()
{

  makeEncHappen = true;
  boolean restart = false;

  if (gotsignal)
  {
    rvs(155, 10);

    while (!gotstation && !STOPALL && !restart)
    {
      while (Signal && !gotstation && !leftOBS() && !STOPALL && !GotStuck) {
        Serial.println("SCANNING TO THE LEFT");
        left(200);
        delay(30);
        obstacleMgt();
        STOP();
        delay(300);
        loop();
      }
      obstacleMgt();
      STOP();
      loop();
      while (Signal && !gotstation && !rightOBS() && !STOPALL && !GotStuck) {
        Serial.println("SCANNING TO THE RIGHT");
        right(200);
        delay(30);
        obstacleMgt();
        STOP();
        delay(300);
        loop();
      }
      obstacleMgt();
      STOP();
      while (!Signal && !gotstation && !STOPALL && !sideOBS() && !GotStuck)
      {
        Serial.println("SIGNAL RETRIEVAL MANAGEMENT");
        loop();
        if (!Signal && !gotstation) {
          left(200);
          delay(30);
          STOP();
          delay(200);
          if (!Signal && !gotstation) {
            Serial.println("RESTARTING OPERATION");
            restart = true; // cancel this whole operation
            goTowardStation();
            break;
          }
        }
        obstacleMgt();
        STOP();
        loop();
        if (!Signal && !gotstation) {
          right(200);
          delay(30);
          STOP();
          delay(200);
          if (!Signal && !gotstation) {
            Serial.println("RESTARTING OPERATION");
            restart = true; // cancel this whole operation
            goTowardStation(); // restart
            break;
          }
        }
      }
      STOP();
      loop();
    }
  }
  makeEncHappen = false;
}

void retrieveLine()
{
  unsigned long erfe = millis();
  while (!lineTest() && millis() - erfe < 2000 && !STOPALL)
  {
    unsigned long gjerh = millis();
    while (!lineTest() && millis() - gjerh < 200)
    {
      left(220);
    }
    gjerh = millis();
    while (!lineTest() && millis() - gjerh < 200)
    {
      right(220);
    }
    loop();
  }
}



boolean lineTest() {

  if (SeeLineLeftRear()) {
    rearleft = true;
  }
  if (SeeLineRightFront()) {
    frontright = true;
  }
  boolean result = rearleft && frontright;
  return result;
}

void obstacleMgt()
{
  makeEncHappen = false;
  if (leftOBS())
  {
    right(200);
    delay(100);
  }
  if (rightOBS())
  {
    left(200);
    delay(100);
  }
  if (rearOBS())
  {
    fwd(200, 0);
    delay(100);
  }
  if (AllObs())
  {
    rvs(155, 0);
    delay(100);
  }
  if (bumped)
  {
    bumped = false;
  }
  //  simpleSTOP();
  makeEncHappen = true;
}
