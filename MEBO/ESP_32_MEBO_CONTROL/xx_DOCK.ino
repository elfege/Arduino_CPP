void dock() {
  cliffOFF = true;
  euristics = true;
  ignoreobs = true;
  docking = true;

  String var = "retractarms";
  st::Everything::sendSmartString(var); // will trigger arm retraction / needs to be done to prevent false OBS() positive
  Serial.println(var);

  int last = 0;
  STOPALL = false;
  stopcharge();

  int  SPEED = 160;
  int d;
  int e;
  int tooClose = 0;
  int MdSp = SPEED;
  int del = 20;
  long AllowedRemainTime;
  int MaxV4Dist = 2047; // max dist val for when robot gets closer relatively to rear obstacle
  int MinV4Dist = 500; // min dist val for when robot is still far relatively to rear obstacle

  boolean keepfixedspeed = false;
  GotStuck = false;
  unsigned long timer = 240000;
  hasSearched = 0;
  hastriedunstuck = 0;
  hastried = 0;
  hastriedOBS = 0;

  delay(500);

  int i = MdSp;
  int tries = 1;
  //rvs(MdSp, 0);
  //noturn = false; // disable encoders to prevent overloads



  unsigned long GStart = millis();//  set a time limit

  while (!STOPALL && !haspower() && millis() - GStart < timer) {

    //Serial.print("New SPEED = "); Serial.println(MdSp);
    Serial << " ELAPSED TIME = " << (millis() - GStart) / 1000 << "SECONDS OUT OF " << timer / 1000 << endl;
    GotStuck = false;
    Signal = false; // this global must be reset before test
    found = false;// this global must be reset before test
    loop(); // will test Signal and found globals

    Serial.println("SEARCH  AAAAAAAAAAAAAAAAAAAAAAAA");
    if (!Signal && hasSearched < 5) {
      SearchSignal();
      hasSearched++;
    }
    else if (!found) { // never know, if it's already found, don't look for it
      SearchStation(30000, 10000, 10, 1000);
    }

    // reverse toward station
    Start = millis();
    while (found && !haspower() && millis() - Start < 5000 && !STOPALL && !GotStuck) {
      Serial << "++++++++++++++++++++" << endl;
      MdSp = MatchD2SREAR(MaxV4Dist, MinV4Dist, 160, 180); // adjust speed with remaining distance
      rvs(MdSp, 0);

      // give it a chance
      Start = millis();
      while (millis() - Start < 1500 && !STOPALL && !haspower()) {
        MdSp = MatchD2SREAR(MaxV4Dist, MinV4Dist, 160, 180); // adjust speed with remaining distance
        rvs(MdSp, 0);
        loop();
      }

      avoidRear();

      Signal = false; // this global must be reset before test
      found = false;// this global must be reset before test
      loop();

      if (!found && analogRead(RIR) < 2047 && analogRead(LIR) < 2047) { // if lost track, try to find it before breaking // only if not into corridor already
        Serial.println("SEARCH BBBBBBBBBBBBBBBBBBBBBBBBB");
        SearchStation(30000, 10000, 10, 1000);
      }

      loop();
    }
    loop();
    RetrievalMethods(170);


  }

  STOP();

  delay(2000);

  if (haspower() && docking) {
    charge();
    docking = false;
    docked = true; // Set robot as docked
    poweroffDONE = false;
    timeCountToPwOff = millis(); // start counter to poweroff
    var = "MEBO docked";
    Serial.println(var);
    st::Everything::sendSmartString(var); // will trigger arm retraction and complete shutdown (shutdown disabled at the moment in ST device handler)
    Serial.println("DOCK SUCCESFULL");
  }
  else {
    docked = false;
    Serial.println("FAILED TO DOCK");
  }
  charge();
  STOP();
  Serial.println("END OF DOCK");
  STOPALL = false;
  cliffOFF = false;
}

void avoidRear() {
  if (analogRead(RRIR) >= 1500 && !haspower() && !STOPALL) {
    Serial << "REAR RIGHT AVOIDANCE RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR" << endl;
    fwd(170, 0);
    delay(200);
    right(MEDIUM);
    delay(50);
  }
  if (analogRead(RLIR) >= 1500 && !haspower() && !STOPALL) {
    Serial << "REAR LEFT AVOIDANCE LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL" << endl;
    fwd(170, 0);
    delay(200);
    left(MEDIUM);
    delay(50);
  }
}
void RetrievalMethods(int MdSp) {
  // retrieval RetrievalMethods
  bool gotoNext = false;
  found = false;
  Serial << "hastried ===================== " << hastried << endl;
  Serial << "GotStuck ===================== " << GotStuck << endl;
  if (hastried < 30 && !haspower() && !STOPALL) {


    if (GotStuck || rearOBS() || sideOBS() && hastriedunstuck < 1 && !haspower() && !STOPALL) {
      Serial.println("METHOD 1");
      Serial.println("METHOD 1");
      Serial.println("METHOD 1");
      Serial.println("METHOD 1");
      unstuck();
      // test stuck
      GotStuck = false;
      rvs(160, 0);
      unsigned long st = millis();
      while (millis() - st < 1000 && !GotStuck) {
        loop();
      }
      STOP();
      if (GotStuck) {
        // go to unstuckrvs test
        gotoNext = true;
      }

      hastriedunstuck++;
    }
    if ((gotoNext || GotStuck || hastriedunstuck >= 1) && ((analogRead(FIR) >= 700 || BUMP() || fusupVAL() <= 20) && (analogRead(LIR) >= 2047 || analogRead(RIR)) >= 2047) && !haspower() && !STOPALL) {
      Serial.println("METHOD 2");
      Serial.println("METHOD 2");
      Serial.println("METHOD 2");
      Serial.println("METHOD 2");
      unstuckRVS();
      GotStuck = false; // reset this value
      gotoNext = false; // reset this value

      hastriedunstuck++;
    }
    if (hastried >= 4) {
      if (((analogRead(FIR) >= 700 || BUMP() || fusupVAL() <= 20) && (analogRead(LIR) >= 2047 || analogRead(RIR)) >= 2047) && !haspower() && !STOPALL) {
        Serial.println("METHOD 3");
        Serial.println("METHOD 3");
        Serial.println("METHOD 3");
        Serial.println("METHOD 3");
        unstuckRVS();
        GotStuck = false; // reset this value
        gotoNext = false; // reset this value
      }
    }
    hastried++;
  }
  else if (hastried >= 30 && !haspower() && !STOPALL) {
    Serial << "ALL RetrievalMethods WERE TRIED, FAILLURE......................" << endl;
    STOP();
    STOPALL = true;
  }
  hastried++;
  GotStuck = false;
  loop();
}

void scanforRoom() {
  euristics = true;

  loop();

  Serial << "SCANNING FOR SOME ROOM .." << endl;
  unsigned long GStart = millis();


  while (!STOPALL && millis() - GStart < 5000 && !RearFrontOK() && !haspower()) {

    Start = millis();
    while (!STOPALL && millis() - Start < 2000 && !RearFrontOK() && !haspower()) {
      left(MEDIUM);
      loop();
      delay(200);
      STOP();
      delay(100);
    }
    loop();
    Start = millis();
    while (!STOPALL && millis() - Start < 2000 && !RearFrontOK() && !haspower()) {
      right(MEDIUM);
      loop();
      delay(200);
      STOP();
      delay(100);
    }
    STOP();

    loop();
  }

  if (!Signal) {
    SearchSignal();
  }
  STOP();
}

void FrontAndRear() {
  if (!haspower() && !STOPALL) {
    if (analogRead(RRIR) > 1000 || analogRead(RLIR) > 1000) {
      fwd(150, 0); // move a little forward if you detect you're too close to a rear obstacle
      delay(200);
      if (analogRead(RRIR) > analogRead(RLIR)) {
        left(170);
        delay(200);
      }
      else {
        right(170);
        delay(200);
      }
    }
    if (analogRead(FIR) > 1000) {
      rvs(150, 0); // move backward if you detect you're too close to a front obstacle
      delay(400);
    }
  }
}

void SearchStation(int GlobalDelay, int ShortDelay, int delturn, int delpause) {
  FrontAndRear();

  euristics = true;
  String lastmove = "";

  GotStuck = false;
  found = false;

  int i;
  int ct;

  Serial.println("SEARCHING FOR STATION");
  unsigned long GStart = millis();
  while (!STOPALL && !found && millis() - GStart < GlobalDelay && !haspower() && !GotStuck) {
    avoidRear();
    Signal = false;
    loop();

    // MAIN MOVE 1
    GotStuck = false;
    ct = 0;
    Start = millis(); // turn right for x seconds
    while (!STOPALL && !found && millis() - Start < ShortDelay && !haspower() && !GotStuck && Signal) {
      lastmove = "r";
      right(FAST);
      noturn = true;
      mvt = true;
      delay(delturn);
      STOP();
      loop(); // gather euristics data
      delay(delpause);
      ct++; // count how many times you moved to the right
    }

    if (!Signal) {
      Serial << "LOST SIGNAL DURING RIGHT SEARCH--------------------------------------------------" << endl;
      while (!Signal && !STOPALL && !haspower() && !GotStuck && millis() - Start < 5000) {
        left(FAST);
        noturn = true;
        mvt = true;
        delay(50);
        STOP();
        loop(); // gather euristics data
        delay(200);
        ct--; // backward logic
      }
      if (!Signal) {
        Serial << "FAILED TO RETRIEVE SIGNAL TO THE LEFT--------------------------------------------------" << endl;
        RetrievalMethods(170);
      }
      else {
        Serial << "SIGNAL RETRIEVED TO THE LEFT--------------------------------------------------" << endl;
      }
    }


    //if not found after turning right, come back to initial position
    Start = millis();
    i = 0;
    while (i < ct && !found && lastmove == "r" && !STOPALL && !found  && !haspower()) {
      left(FAST);
      noturn = true;
      mvt = true;
      delay(delturn);
      STOP();
      loop(); // gather euristics data
      delay(50); // shorter delay because we're using counter and not millis()
      i++;
    }

    Signal = false;
    loop();

    // MAIN MOVE 2
    ct = 0;
    Start = millis(); // turn left
    while (!STOPALL && !found && millis() - Start < ShortDelay && !haspower() && !GotStuck && Signal) {
      lastmove = "l";
      left(FAST);
      noturn = true;
      mvt = true;
      delay(delturn);
      STOP();
      loop(); // gather euristics data
      delay(delpause);
      ct++; // count how many times you moved to the left
    }

    if (!Signal) {
      Serial << "LOST SIGNAL DURING LEFT SEARCH--------------------------------------------------" << endl;
      Start = millis();
      while (!Signal && !STOPALL && !haspower() && !GotStuck && millis() - Start < 5000) {
        right(FAST);
        noturn = true;
        mvt = true;
        delay(50);
        STOP();
        loop(); // gather euristics data
        delay(200);
        ct--; // backward logic
      }
      if (!Signal) {
        Serial << "FAILED TO RETRIEVE SIGNAL TO THE RIGHT--------------------------------------------------" << endl;
        RetrievalMethods(170);
      }
      else {
        Serial << "SIGNAL RETRIEVED TO THE RIGHT--------------------------------------------------" << endl;
      }
    }


    //if not found after turning left, come back to initial position
    Start = millis();
    i = 0;
    while (i < ct && !found && lastmove == "l" && !STOPALL && !found  && !haspower()) {
      right(FAST);
      noturn = true;
      mvt = true;
      delay(delturn);
      STOP();
      loop(); // gather euristics data
      delay(50); // shorter delay because we're using counter and not millis()
      i++;
    }
    loop();
  }
  if (found) {
    Serial << "FOUND THE STATION--------------------------------------------------FOUND THE STATION" << endl;
  }
  else if (!Signal) {
    Serial << "LOST SIGNAL DURING SEARCH--------------------------------------------------LOST SIGNAL DURING SEARCH" << endl;
  }

  STOP();
}

void SearchSignal() {

  ////////////////// SIGNAL ....////////////////

  FrontAndRear();
  euristics = true;
  String lastmove = "";

  int delturn = 50;
  int delpause = 50;
  unsigned long ct;
  int i;
  int RECcr = 0;
  int RECcl = 0;
  int SP = 180;
  cr = 0;
  cl = 0;

  Serial.println("SEARCHING FOR SIGNAL");
  GotStuck = false; // starting point consider not stuck
  Signal = false;
  loop(); // double check it's still true to prevent moving away from it while already caught

  unsigned long GStart = millis();
  while (!STOPALL && !Signal && millis() - GStart < 60000 && !GotStuck && !haspower()) {

    // MAIN MOVE 1
    Start = millis(); // turn right for x seconds

    while (!STOPALL && !Signal && millis() - Start < 20000 && !GotStuck && !haspower()) {
      lastmove = "r";
      right(SP);
      noturn = true;
      mvt = true;
      delay(delturn);
      simpleSTOP();
      delay(delpause);

      Serial << "RECcl = " << RECcl << endl;
      loop(); // gather euristics data
      ct = millis() - Start; // record for how long it's been turning to the right
      RECcl = cl; // also count left encoder // we use both running time and encoder for encoder count is subject to stuck
    }
    STOP();

    //if not found after turning right, come back to initial position
    Start = millis();
    left(SP);
    noturn = true;
    mvt = true;
    while (cl < RECcl && millis() - Start < ct && !Signal && lastmove == "r" && !STOPALL && !haspower()) {
      Serial << RECcl << " < " << cl << endl;
      loop(); // gather euristics data
    }
    STOP();

    // MAIN MOVE 2
    Start = millis(); // turn left
    left(SP);
    noturn = true;
    mvt = true;
    while (!STOPALL && !Signal && millis() - Start < 20000 && !GotStuck && !haspower()) {
      lastmove = "l";

      Serial << "RECcr = " << RECcr << endl;
      left(SP);
      noturn = true;
      mvt = true;
      delay(delturn);
      simpleSTOP();
      delay(delpause);
      loop(); // gather euristics data
      ct = millis() - Start; // record for how long it's been turning to the right
      RECcr = cr; // also count right encoder // we use both running time and encoder for encoder count is subject to stuck
    }
    STOP();

    //if not found after turning left, come back to initial position
    Start = millis();

    right(SP);
    noturn = true;
    mvt = true;
    while (cr < RECcr && millis() - Start < ct && i < ct && !Signal && lastmove == "l" && !STOPALL && !haspower()) {
      Serial << RECcr << " < " << cr << endl;
      loop(); // gather euristics data
    }
    STOP();
    loop();
  }
  STOP();
  if (GotStuck) {
    Serial << "SIGNAL SEARCH CANCELED BECAUSE MEBO GOT STUCK " << endl;
  }
  if (Signal) {
    Serial << "SIGNAL FOUND " << endl;
  }

  //  // narrow down
  SearchStation(30000, 5000, 20, 500);


  GotStuck = false;
}

void RandomSearch() {
  unsigned long GS = millis();
  int dT = 200;
  GotStuck = false;
  Signal = false;
  int Sp = 160;
  bumped = false;

  while (!Signal && !STOPALL && !haspower() && millis() - GS < 50000) {
    if (!GotStuck && !Signal && !STOPALL && !haspower()) {
      fwd(Sp, 15);
      if (frontIR() || bumped && !Signal && !STOPALL) {
        bumped = false;
        rvs(Sp, 10);
        right(180);
        noturn = true;
        mvt = true;
        delay(dT);
        loop();
      }
      fwd(Sp, 5);
      int x = random(1, 11); // upper bound is exclusive
      Serial << "random x = " << x << endl;
      if (x <= 5 && !Signal && !STOPALL) {
        left(170);
      }
      else {
        right(170);
      }
      noturn = true;
      mvt = true;
      loop();
      delay(dT);
    }
    // if got stuck
    else {
      Serial << "U N S T U C K ---------------------------------------------------------------------" << endl;
      if (analogRead(FIR) > 1500 && (analogRead(LIR) > 2000 || analogRead(LIR) > 2000 ) && !Signal && !STOPALL) {
        unstuckRVS();
        GotStuck = false;
      }
      else if (!Signal && !STOPALL) {
        unstuck();
        GotStuck = false;
      }
    }
    if (!Signal && !STOPALL) {
      loop();
    }
  }
  STOP();
  if (Signal) {
    Serial << "SIGNAL FOUND DURING RANDOM SEARCH" << endl;
  }
}

