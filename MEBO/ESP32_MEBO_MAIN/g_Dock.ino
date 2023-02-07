void dock() {
  _server.send(200, "text/html", "Docking");

  foreArmUp();
  armUp();
  docking = true;
  STOPALL = false;

  runIRreceiver = true;
  allSet = false;
  check = true;
  Signal = false;
  found = false;
  GotStuck = false;
  noblink = true;
  makeEncHappen = true; // allows encoders to count despite the fact that this is a turn command

  leftDone = true;
  rightDone = false;

  /// MAIN COMMANDS
  dockingFunctions();

  STOP(); // resets cr and cl
  term.println("END OF SIGNAL SEARCH");

  if (gotstation) {
    term.println("GOT STATION!-------------------------------!!!!!!");
    FollowLineRVS();
  }
  else {
    term.println("FAILED...TO FIND THE STATION...");
  }
  GotStuck = false;
  STOP(); // resets cr and cl
}


void FollowLineRVS() {
  docking = true; // prevents volts check which would return false positive for this function
  STOPALL = false;
  runIRreceiver = true; // IR station signal will be looked for
  foreArmUp();
  //armDown();
  stopcharge();
  term.println("Waiting for power to drop to 0");
  unsigned long pt = millis();
  unsigned long cd = millis();
  while (haspower() && millis() - pt < 10000 && !STOPALL)
  {
    if (millis() - cd > 500)
    {
      term.print(".");
      cd = millis();
    }
    MainFunctions();
  }
  if (haspower())
  {
    delay(300);
    if (haspower() && digitalRead(CHARGERelay) == HIGH) // charger is off, if there's current, means docked
    {
      term.println("ALREADY DOCKED AND CHARGING");
      _server.send(200, "text/plain", "DOCKED");
      charge(); //reconnect charger
    }
    else if (haspower() && digitalRead(CHARGERelay) == LOW) {
      // if has power and charge relay is still ON (LOW), meaning get current from battery,
      // then it means something is wrong
      term.println("Failed to turn off the power, docking won't work");
    }
    return;
  }

  makeEncHappen = true; // allow stuck test while turning

  String state = "Docking with line";
  _server.send(200, "text/plain", state);

  check = true; // allow GotStuck to be checked
  GotStuck = false;
  int last = 0;
  STOPALL = false;
  int del = 50;
  int test = 0;

  manageGotStuck = 0;
  bumped = false;
  noblink = true; // blink led takes millis...

  sPeeD = 160;
  startingSpeed = sPeeD;

  int lineretrieveExecuted = 0;

  boolean dontrestartthisloop = false; // if this loop is called by dock, you don't want its restart feature to be triggered, dock already does it.
  // in that case it'll be set to true at break point below.

  unsigned long GStart = millis();//  set a time limit
  while (1)
  { // using break command instead of arguments for debug purpose
    MainFunctions();
    term.println("Elpased Time: " + String((millis() - GStart) / 1000) + " seconds");
    if (millis() - GStart > 30000)
    {
      // there's a problem, retry
      term.println("time out!");
      retrieveLineFwd();
      GStart = millis();
    }

    //term.println("LINE DOCKING");
    HttpReq = false; // make sure it'll take this speed
    rvs(sPeeD, 0);

    execLineRvs();
    sPeeD = startingSpeed;

    if (GotStuck) {
      GotStuck = false;
      term.println("Stuck management at line follow dock");
      retrieveLineFwd();
      STOP();

    }
    if (bumped) { // might be triggered by retrieveLineFwd()
      bumped = false;
    }

    if (STOPALL) {
      //term.print("Follow Rvs Break due to: ");
      term.println("STOPALL!");
      break;
    }
    else if (haspower()) {
      //term.print("Follow Rvs Break due to: ");
      term.println("I'VE GOT THE POWER!");
      break;
    }
    else if (millis() - GStart > 120000)
    {
      //term.print("Follow Rvs Break due to: ");
      term.println("TIME IS UP!");
      break;
    }
  }

  STOP();


  STOPALL = false;

  //
  delay(500);
  if (haspower()) {
    STOP();
    charge();
    _server.send(200, "text/plain", "DOCKED");
  }
  else {
    term.println("FAILED TO DOCK");
  }
  term.println("END OF FOLLOWLINERVS");
  manageGotStuck = 0;
  lineretrieveExecuted = 0;
  makeEncHappen = false;
  noblink = false;
}

void execLineRvs()
{
  int del = 100;
  int tlim = 5000;
  unsigned long a;
  unsigned long b;

  a = millis();
  while (SeeLineLeftRear() && !STOPALL && !haspower() && !GotStuck /*&& !rearOBS()*/ && millis() - a < tlim) {

    Serial.println("Line Dock Right");

    right(180);
    b = millis();
    while (SeeLineLeftRear() && !haspower() && millis() - b < tlim)
    {
      delay(1);//MainFunctions() MUST not run while in motion
    }
    STOP();
    delay(100);
    rvs(160, 0);
    b = millis();
    //reverse minimum 50ms or up to 200ms while still seeing the line
    while ((SeeLineLeftRear() || millis() - b < 50) && !haspower() && millis() - b < 200)
    {
      delay(1);//MainFunctions() MUST not run while in motion
    }

    MainFunctions();

  }
  a = millis();
  while (SeeLineRightRear() && !STOPALL && !haspower() && !GotStuck && millis() - a < tlim) {

    Serial.println("Line Dock left");
    left(180);
    b = millis();
    while (SeeLineRightRear() && !haspower() && millis() - b < tlim)
    {
      delay(1);//MainFunctions() MUST not run while in motion
    }
    STOP();
    delay(100);
    rvs(160, 0);
    b = millis();
    //reverse minimum 50ms or up to 200ms while still seeing the line
    while ((SeeLineRightRear() || millis() - b < 50) && !haspower() && millis() - b < 200)
    {
      delay(1);//MainFunctions() MUST not run while in motion
    }
    MainFunctions();

  }
  if (millis() - a > tlim) // there's a problem, restart procedure
  {
    retrieveLineFwd();
  }
}

void manageStuckWheels() {

  manageGotStuck++;

  if (!rearOBS() && manageGotStuck < 4) {
    term.print("increasing output");
    GotStuck = false;
    if (sPeeD < 210)
    {
      sPeeD += 10;
    }
  }


}

void retrieveLineFwd() {

  boolean hasnotseenlineyet = true;
  boolean FrontLeftSeen = false;
  boolean FrontRightSeen = false;
  boolean shiftDone = false;
  unsigned long timeR;
  unsigned long timeL;
  unsigned long dsd = millis();
  int fwdTime = 2500;
  // !BothFrontSeen() taken out of the equation here because can be stuck while both see the line or docking black band

  while (millis() - dsd < fwdTime && !STOPALL) {
    term.println("Line retrieval FWD");
    fwd(sPeeD, 0);
    execLineFwdWithoutStopSign();
    MainFunctions();
  }
  GotStuck = false;
}

void execLineFwdWithoutStopSign()
{
  int del = 50;
  while (SeeLineLeftFront() && !STOPALL && !bumped) {
    MainFunctions();
    left(200);
    delay(100);
    fwd(SPEED, 0);
    term.print("left  "); term.println(SeeLineLeftFront());
    term.println("SL FWD");
    delay(del);
  }
  while (SeeLineRightFront() && !STOPALL && !bumped) {
    MainFunctions();
    right(200);
    delay(100);
    fwd(SPEED, 0);
    term.print("right "); term.println(SeeLineRightFront());
    term.println("SR FWD");
    delay(del);
  }
}
