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
  armDown();
  stopcharge();
  delay(10);

  makeEncHappen = true; // allow stuck test while turning

  String state = "Docking with line";
  _server.send(200, "text/plain", state);

  check = true; // allow GotStuck to be checked
  GotStuck = false;
  flwln = true; // disable cliff sensing (deprecated)
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
  while (1) { // using break command instead of arguments for debug purpose
    loop();

    Serial.println("LINE DOCKING");
    HttpReq = false; // make sure it'll take this speed
    rvs(sPeeD, 0);

    execLineRvs();
    sPeeD = startingSpeed;

    if (GotStuck || rearOBS()) {
      GotStuck = false;
      Serial.println("Stuck management at line follow dock");
      retrieveLineFwd();
      STOP();
      if (bumped) {
        bumped = false;
      }
    }

    if (bumped) {
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

      rvs(200, 5);// little extra push toward contacts
      unsigned long dfh = millis();
      while (!haspower() && millis() - dfh < 5000)
      {
        // if by chance you lost the power, go back and forth until you get it
        rvs(180, 5);
        fwd(180, 3);
      }
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
  flwln = false; // enable cliff sensing

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
  while (SeeLineLeftRear() && !STOPALL && !haspower() && !GotStuck && !rearOBS()) {

    Serial.println("Line Dock Right");

    right(200);
    delay(100);
    rvs(sPeeD, 0);
    loop();

  }
  while (SeeLineRightRear() && !STOPALL && !haspower() && !GotStuck && !rearOBS()) {

    Serial.println("Line Dock left");
    left(200);
    delay(100);
    rvs(sPeeD, 0);
    loop();

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
  int fwdTime = 5000;


  while ((millis() - dsd < fwdTime && !BothFrontSeen()) && !STOPALL && fusLVAL() > 5 && fusRVAL() > 5 && !frontIR() && !bumped && !found) {
    Serial.println("Line retrieval FWD 2");
    fwd(sPeeD, 0);
    execLineFwd();
    loop();
  }
}
