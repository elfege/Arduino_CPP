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
  clrec = 0;
  crrec = 0;


  /// MAIN COMMANDS
  dockingFunctions();

  STOP(); // resets cr and cl
  makeEncHappen = false;
  Serial.println("END OF SIGNAL SEARCH");

  if (Signal) {
    Serial.println("GOT SIGNAL!-------------------------------!!!!!!");
    FollowLineRVS();
  }
  else {
    Serial.println("FAILED...TO FIND THE STATION...");
  }
  STOP(); // resets cr and cl
}


void stepright(int sPeeD, int Del, int timeOff, int turndist)
{
  right(sPeeD);
  delay(Del);
  clrec += cl; // when you turn right, the left wheel moves more
  STOP(); // resets cr
  Serial.print("crrec = "); Serial.println(crrec);
  delay(Del);
  if (clrec >= turndist)
  {
    clrec = 0; // for next round...
    rightDone = true;
    leftDone = false;
  }
}
void stepleft(int sPeeD, int Del, int timeOff, int turndist)
{
  left(sPeeD);
  delay(Del);
  crrec += cr; // when you turn left the right wheel moves more
  STOP(); // resets cl
  Serial.print("clrec = "); Serial.println(clrec);
  delay(Del);
  if (crrec >= turndist)
  {
    crrec = 0; // for next round...
    rightDone = false;
    leftDone = true;
  }
}

void FollowLineRVS() {
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
  SPEED = 170;
  int del = 50;
  int test = 0;

  bumped = false;
  noblink = true; // blink led takes millis...

  int sPeeD = 170;
  int startingSpeed = sPeeD;

  int lineretrieveExecuted = 0;

  boolean dontrestartthisloop = false; // if this loop is called by dock, you don't want its restart feature to be triggered, dock already does it.
  // in that case it'll be set to true at break point below.

  unsigned long GStart = millis();//  set a time limit
  while (!STOPALL && !haspower() && millis() - GStart < 120000) {
    loop();

    Serial.println("LINE DOCKING");
    rvs(SPEED, 0);
    unsigned long strt = millis();
    while (found && millis() - strt < 200) {
      Serial.println("..");
      loop();
    }

    // Signal is always true if request came from javascript webpage!
    while (Signal && SeeLineLeftRear() && !STOPALL && !haspower() && !GotStuck) {
      loop();
      Serial.println("Line Dock Right");
      right(180);
      unsigned long thisS = millis();
      while (SeeLineRightRear() && !STOPALL && millis() - thisS < 50) {
        loop();
      }
    }
    delay(del);
    loop();
    while (Signal && SeeLineRightRear() && !STOPALL && !haspower() && !GotStuck) {
      loop();

      Serial.println("Line Dock left");
      left(180);
      unsigned long thisS = millis();
      while (SeeLineRightRear() && !STOPALL && millis() - thisS < 50) {
        loop();
        delay(del);
        loop();
      }
    }
    if (GotStuck) {
      Serial.print("increasing output");
      GotStuck = false;
      if (sPeeD < 210)
      {
        sPeeD += 10;
      }
    }
    else {
      sPeeD = startingSpeed;
    }

    STOP();
    if (!Signal) // this will work only if useSignal is true!
    {
      // move fwd a little
      unsigned long dff = millis();
      lineretrieveExecuted++;
      while (millis() - dff < 800 && !STOPALL && fusLVAL() > 5 && fusRVAL() > 5 && !frontIR() && !bumped) {
        execLineFwd();
      }
      /// search signal again using docking functions
      dockingFunctions();
    }
    else if (GotStuck) {
      GotStuck = false;
      Serial.println("Stuck management at line follow dock");

      if (lineretrieveExecuted < 4) {
        Serial.println("EXECUTING LINE RETRIEVAL!!");
        lineretrieveExecuted++;
        retrieveLine();
        STOP();
      }
      else if (lineretrieveExecuted < 6) {
        unsigned long aei = millis();
        lineretrieveExecuted++;
        while (millis() - aei < 1500 && !STOPALL && fusLVAL() > 5 && fusRVAL() > 5 && !frontIR() && !bumped) {
          execLineFwd();
        }
        STOP();
        if (bumped) {
          bumped = false;
        }
      }
      else {
        Serial.println("OUT OF OPTIONS... MAIN BREAK!!!");
        break;
      }
      STOP();
      if (bumped) {
        bumped = false;
      }
    }
    STOP();
  }

  Serial.println("Follow Rvs Break due to: ");
  if (millis() - GStart > 120000)
  {
    Serial.print("TIME IS UP!");
  }





  STOP();
  makeEncHappen = false;
  noblink = false;

  STOPALL = false;
  flwln = false; // enable cliff sensing

  //rvs(200, 2);// little extra push toward contacts
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

}

void dockingFunctions()
{

  boolean gotsignal = false;
  unsigned long Startrightscan;
  unsigned long StartleftScan;

  int del1 = 50;
  int del2 = 50;

  int cycles = 0;
  int cycleslimit = 5;


  // move until you get an IR signal
  unsigned long StartSearch = millis();
  // move slowly backward to get the specific signal

  int rdmdel;// random turn delay

  while (!Signal && !STOPALL && millis() - StartSearch < 120000)
  {
    loop(); // starts euristics IR receiver
    gotsignal = false;


    int countStuck = 0;
    int turndist = 5;
    int delsearch = 50;
    int sPeeD = 170;
    int startingSpeed = sPeeD;
    rearleft = false;
    frontright = false; // lineTest() globals

    int timeOff = 3000;


    while (!Signal && !AllObs() && !STOPALL && cycles < cycleslimit) // && !lineTest())
    {

      /////////////////----------SWEEPING-----------------/////////////////

      if (!leftDone && rightDone) { // enough done to the right, reset right and go to the left

        Serial.println("SWEEP LEFT");

        stepleft(sPeeD, 50, timeOff, turndist); // speed, delay, timer, turn distance

        if (GotStuck) {
          Serial.print("increasing output");
          GotStuck = false;
          if (sPeeD < 220)
          {
            sPeeD += 10;
            stepleft(sPeeD, 30, timeOff, turndist);  // speed, delay, timer, turn distance
          }
        }
        else {
          sPeeD = startingSpeed;
        }
      }
      else if (leftDone && !rightDone) // enough done to the left, reset left and go right
      {

        Serial.println("SWEEP RIGHT");

        stepright(sPeeD, 50, timeOff, turndist); // speed, delay, timer, turn distance

        if (GotStuck) {
          Serial.print("increasing output");
          GotStuck = false;
          if (sPeeD < 220)
          {
            sPeeD += 10;
            stepright(sPeeD, 50, timeOff, turndist); // speed, delay, timer, turn distance
          }
        }
        else {
          sPeeD = startingSpeed;
        }
        loop();
        cycles++;
      }
    }
    /////////////////----------END OF SWEEPING----------/////////////////

    if (!Signal && !STOPALL) {
      Serial.println("LOOP BROKE DUE TO: "); ////.///... OBSTACLE MANAGEMENT
      if (cycles >= cycleslimit)
      {
        Serial.println("TOO MANY CYCLES");
        rvs(160, 0);
        cycles = 0;
        delay(400);
      }
      else if (leftIRVal() > 500) {
        Serial.println("LEFT OBSTACLE");
        left(200);
      }
      else if (rightIRVal() > 500) {
        Serial.print("RIGHT OBSTACLE");
        right(200);
      }
      else if (fusupVAL() < 30 || frontIRVal() > 500 || fusLVAL() < 5 || fusRVAL() < 5)
      {
        Serial.print("FRONT OBSTACLE");
        rvs(170, 5);
      }
      else if (rearOBS()) {
        Serial.print("REAR OBSTACLE");
        fwd(170, 5);
      }
      else if (millis() - StartSearch > 120000)
      {
        Serial.print("TIME IS UP!");
      }
      delay(200);
      STOP();
    }
    STOP();
    loop();
  }
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
