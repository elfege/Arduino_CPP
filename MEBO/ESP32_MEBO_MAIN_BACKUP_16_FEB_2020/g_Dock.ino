void dock() {
  _server.send(200, "text/html", "Docking");
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
  Serial.println("END OF SIGNAL SEARCH");

  if (gotstation) {
    Serial.println("GOT STATION!-------------------------------!!!!!!");
    FollowLineRVS();
  }
  else {
    Serial.println("FAILED...TO FIND THE STATION...");
  }
  GotStuck = false;
  STOP(); // resets cr and cl
}


void FollowLineRVS() {
  docking = true; // prevents volts check which would return false positive for this function
  STOPALL = false;
  runIRreceiver = true; // IR station signal will be looked for
  foreArmUp();
  armDown();
  stopcharge();
  unsigned long pt = millis();
  while(haspower() && millis() - pt < 10000)
  {
    Serial.println("Waiting for power to drop to 0");
  }
  if(haspower())
  {
    Serial.println("Failed to turn off the power, docking won't work");
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
    loop();
    Serial.println("Elpased Time: "+String((millis()-GStart)/1000)+" seconds");
    if (millis() - GStart > 30000)
    {
      // there's a problem, retry
      Serial.println("time out!");
      retrieveLineFwd();
      GStart = millis();
    }

    Serial.println("LINE DOCKING");
    HttpReq = false; // make sure it'll take this speed
    rvs(sPeeD, 0);

    execLineRvs();
    sPeeD = startingSpeed;

    if (GotStuck) {
      GotStuck = false;
      Serial.println("Stuck management at line follow dock");
      retrieveLineFwd();
      STOP();

    }
    if (bumped) { // might be triggered by retrieveLineFwd()
      bumped = false;
    }

    if (STOPALL) {
      Serial.print("Follow Rvs Break due to: ");
      Serial.println("STOPALL!");
      break;
    }
    else if (haspower()) {
      Serial.print("Follow Rvs Break due to: ");
      Serial.println("I'VE GOT THE POWER!");
      break;
    }
    else if (millis() - GStart > 120000)
    {
      Serial.print("Follow Rvs Break due to: ");
      Serial.println("TIME IS UP!");
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
    Serial.println("FAILED TO DOCK");
  }
  Serial.println("END OF FOLLOWLINERVS");
  manageGotStuck = 0;
  lineretrieveExecuted = 0;
  makeEncHappen = false;
  noblink = false;
}

void execLineRvs()
{
  int del = 100;
  // Signal is always true if request came from Dock with reverse button!
  unsigned long ss = millis();
  int tlim = 5000;
  while (SeeLineLeftRear() && !STOPALL && !haspower() && !GotStuck && !rearOBS() && millis() - ss < tlim) {

    Serial.println("Line Dock Right");

    right(200);
    delay(100);
    rvs(sPeeD, 0);
    loop();

  }
  ss = millis();
  while (SeeLineRightRear() && !STOPALL && !haspower() && !GotStuck && !rearOBS() && millis() - ss < tlim) {

    Serial.println("Line Dock left");
    left(200);
    delay(100);
    rvs(sPeeD, 0);
    loop();

  }
  if (millis() - ss > tlim) // there's a problem, restart procedure
  {
    retrieveLineFwd();
  }


}

void manageStuckWheels() {

  manageGotStuck++;

  if (!rearOBS() && manageGotStuck < 4) {
    Serial.print("increasing output");
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
    Serial.println("Line retrieval FWD");
    fwd(sPeeD, 0);
    execLineFwdWithoutStopSign();
    loop();
  }
  GotStuck = false;
}

void execLineFwdWithoutStopSign()
{
  int del = 50;
  while (SeeLineLeftFront() && !STOPALL && !bumped) {
    loop();
    left(200);
    delay(100);
    fwd(SPEED, 0);
    Serial.print("left  "); Serial.println(SeeLineLeftFront());
    Serial.println("SL FWD");
    delay(del);
  }
  while (SeeLineRightFront() && !STOPALL && !bumped) {
    loop();
    right(200);
    delay(100);
    fwd(SPEED, 0);
    Serial.print("right "); Serial.println(SeeLineRightFront());
    Serial.println("SR FWD");
    delay(del);
  }
}
