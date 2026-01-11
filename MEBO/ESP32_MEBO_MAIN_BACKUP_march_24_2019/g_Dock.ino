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

  useSignal = true; // required to allow using mere signal (and not just "found");
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

    if (GotStuck) {
      GotStuck = false;
      Serial.println("Stuck management at line follow dock");
      lineretrieveExecuted++;

      unsigned long aei = millis();
      boolean hasnotseelineyet = true;
      int fwdTime = 2500;
      while ((millis() - aei < fwdTime || hasnotseelineyet) && !STOPALL && !bumped) {
        Serial.println("Line retrieval FWD 1");
        fwd(sPeeD, 0);
        if (SeeLineLeftFront() || SeeLineRightFront())
        {
          hasnotseelineyet = false;
          fwdTime += 1000;
        }
        if(millis() - aei > fwdTime + 1000)
        {
          break;
        }
        delay(50);
        execLineFwd();
        loop();
      }
      Serial.print("lineretrieveExecuted = "); Serial.println(lineretrieveExecuted);
      if (lineretrieveExecuted >= 4)
      {
        Serial.println("docking functions...");
        /// search signal again using docking functions
        //  THIRD TIME AROUND ONLY!
        useSignal = true;
        Signal = false;
        dockingFunctions();
        useSignal = false;
        lineretrieveExecuted = 0;
      }
      STOP();
      if (bumped) {
        bumped = false;
      }
    }
    else if (rearOBS())
    {
      lineretrieveExecuted++;
      boolean hasnotseelineyet = true;
      unsigned long dsd = millis();
      int fwdTime = 2500;
      while ((millis() - dsd < fwdTime || hasnotseelineyet) && !STOPALL && fusLVAL() > 5 && fusRVAL() > 5 && !frontIR() && !bumped) {
        Serial.println("Line retrieval FWD 2");
        fwd(sPeeD, 0);
        delay(50);
        if (SeeLineLeftFront() || SeeLineRightFront())
        {
          hasnotseelineyet = false;
          fwdTime += 1000;
        }
        if(millis() - dsd > fwdTime + 1000)
        {
          break;
        }
        execLineFwd();
      }
      Serial.print("lineretrieveExecuted = "); Serial.println(lineretrieveExecuted);
      if (lineretrieveExecuted >= 4)
      {
        Serial.println("docking functions...");
        /// search signal again using docking functions
        //  THIRD TIME AROUND ONLY!
        useSignal = true;
        Signal = false;
        dockingFunctions();
        useSignal = false;
        lineretrieveExecuted = 0;
      }
      STOP();
    }
    if (STOPALL) {
      Serial.print("Follow Rvs Break due to: ");
      Serial.println("STOPALL!");
      break;
    }
    else if (haspower()) {
      Serial.print("Follow Rvs Break due to: ");
      Serial.println("'GOT DA POWER!");
      rvs(200, 1);// little extra push toward contacts
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
