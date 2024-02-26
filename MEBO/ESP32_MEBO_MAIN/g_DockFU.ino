
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
  StartSearch = millis(); // has to be global

  while (!gotstation && !STOPALL && millis() - StartSearch < 120000)
  {
    MainFunctions(); // starts euristics IR receiver

    int countStuck = 0;
    int turndist = 10;
    int Length = 200;
    int delsearch = 50;
    int docking_speed = 170;
    int startingSpeed = docking_speed;
    rearleft = false;
    frontright = false; // lineTest() globals

    int timeOff = 3000;

    makeEncHappen = true;


    findSignal(); /// roam nearby the station trying to catch A signal
    // findSignal() will call getStationSignal()// important so it can restart it if necessary


    STOP();
    MainFunctions();
  }

  term.print("DOCKING FUNCTION ENDED BECAUSE: ");
  if ((!gotsignal || !gotstation) && !STOPALL)
  {
    if (GotStuck)
    {
      term.println("GOT STUCK");
      GotStuck = false;
    }
    else if (millis() - StartSearch > 120000)
    {
      term.println("TIME IS UP!");
    }
    else if (gotstation)
    {
      term.println("STATION WAS FOUND!");
    }
    else       {
      term.println("UNKNOWN REASON...");
    }
    term.println("");
    obstacleMgt();
  }

  if (gotstation)
  {
    term.println("/****************************************/");
    term.println("         STATION HAS BEEN FOUND           ");
    term.println("/****************************************/");

    unsigned long mlr = millis();
    makeEncHappen = true;
    rvs(155, 0);
    while (found && millis() - mlr < 3000 && !STOPALL && !rearOBS() && !sideOBS()) {
      MainFunctions();
    }
  }

}

void findSignal() {

  //noStuck = true; // DON'T READ STUCK VALUES

  rightDone = false;
  leftDone = true;
  GotStuck = false;

  String debug = "";


  unsigned long st = millis();

  while (!gotsignal && !STOPALL && millis() - st < 20000)
  {
    term.println("LOOKING FOR ANY SIGNAL");

    obstacleMgt();// calls loop()
    fwd(160, 10);
    obstacleMgt();// calls loop()
    left(180);
    delay(200);
    STOP();
    delay(500);
    obstacleMgt();// calls loop()

  }
  if (!gotsignal)
  {
    term.println("FAILED TO ACQUIRE ANY SIGNAL");
    return;
  }
  else
  {
    term.println("--------------------SIGNAL ACQUIRED! -----------------------------");
    delay(2000);
    getStationSignal();
  }
  noStuck = false;

}

void  getStationSignal()
{

  makeEncHappen = true;
  boolean restart = false;
  gotsignal = false;

  int Min = 50;
  int Max = 50;
  int val;

  unsigned long st = millis();
  while (!gotstation && !STOPALL && millis() - st < 20000)
  {
    term.println("LOOKING FOR STATION'S SIGNAL");

    if (!gotstation)
    {
      val = random(Min, Max);
      left(200);
      delay(val);
      STOP();
      delay(100);
    }
    obstacleMgt();
    MainFunctions(); // check IR signal

    if (!gotstation)
    {
      val = random(Min, Max);
      right(200);
      delay(val);
      STOP();
      delay(100);
    }

    obstacleMgt();
    MainFunctions(); // check IR signal

  }

  if (gotstation)
  {
    FollowLineRVS();
  }
  else if (!gotsignal && !STOPALL && millis() - StartSearch < 120000)
  {
    term.println("ALL SIGNAL LOST!"); // RESTART OPERATIONS

    findSignal(); // go back to findSignal(), which will restart this whole operation
  }
  else
  {
    term.println("FAILED TO ACQUIRE STATION'S SIGNAL");
  }
  STOP();
  MainFunctions();


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
    MainFunctions();
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
}
