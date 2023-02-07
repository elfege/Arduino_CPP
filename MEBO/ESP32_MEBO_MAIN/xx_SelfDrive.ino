
void roam() {
  /*******************************************************************
    BUMP AND STUCK ARE ALWAYS CHECKED IN THE MAIN LOOP, NO MATTER WHAT!
  ************** *****************************************************/
  logs = false; // stop debugging logs
  isSelfDriving = true;
  String state = isSelfDriving ? "ROAMING" : "NOT ROAMING";
  _server.send(200, "text/html", state);

  armUp();
  ignoreobs = true;// should be true, except for tests
  STOPALL = false;
  docking = false;// should be false, except for tests
  boolean rvsdone = false;
  boolean wentLeft = false;

  runIRreceiver = false; // don't activate IR runIRreceiver

  int thisSPEED = 160;

  method = 0; // binary tree A.I.
  int USdist = 30;
  GotStuck = false;

  noblink = true; // don't allow LED blink for it generates delays


  unsigned long StartingPoint;
  unsigned long StartHere;

  while (!STOPALL) {

    check = true; // tell main loop that this loop is already taking care of the stuck test
    bumped = false;
    GotStuck = false;
    boolean logged = false;

    fwd(150, 0);
    StartingPoint = millis();

    digitalWrite(LED, 0); // steady ON LED, signaling fwd mvt
    while (!STOPALL && !bumped && (frontIR_RVal() < IRlimit && frontIR_LVal() < IRlimit) && fusupVAL() > 30 && fusLVAL() > 5 && fusRVAL() > 5 && !GotStuck && millis() - StartingPoint < 10000)
    {
      MainFunctions();
      stuck();
      BUMP();
      if ((BUMPR() && !BUMPL()) || fusupVAL_R() < 20)
      {
        rvs(170, 0);
        delay(400);
        left(200);
        delay(100);
        fwd(150, 0);//resume initial cmd
      }
      else if ((BUMPL() && !BUMPR()) || fusupVAL_L() < 20)
      {
        rvs(170, 0);
        delay(400);
        right(200);
        delay(100);
        fwd(150, 0);//resume initial cmd
      }
    }
    STOP();
    Blink(3, 150); // signaling obstacles management
    GotStuck = false; // reset this value
    bumped = false; // reset this value

    if (!STOPALL) {
      rvs(175, 0);
      delay(200);
      STOP();
      String side = leftIRVal() < 200 && rightIRVal() > 200 ? "left" : rightIRVal() < 200 && leftIRVal() > 200 ? "right" : "undecided";

      if (side == "left") {
        term.println("More space to the left");
        StartHere = millis();
        simpleLeft(180); // no automated stop
        logged = false;
        while (!freeWay() && !STOPALL && !leftOBS() && !GotStuck && !bumped && millis() - StartHere < 5000)
        {
          if (!logged) {
            term.println("scanning left");
            logged = true;
          }
          if (rearOBS())
          {
            fwd(170, 0);
            delay(400);
            simpleLeft(180);//resume initial cmd
          }
          if ((BUMPL() && !BUMPR()) || fusupVAL_L() < 20)
          {
            rvs(170, 0);
            delay(400);
            right(200);
            delay(200);
            simpleLeft(180);//resume initial cmd
          }
          MainFunctions();
          stuck();
          BUMP();
        }
      }
      else if ("right") {
        term.println("More space to the right");
        StartHere = millis();
        simpleRight(180); // no automated stop
        logged = false;
        while (!freeWay() && !STOPALL && !rightOBS() && !GotStuck && !bumped && millis() - StartHere < 5000)
        {
          if (!logged) {
            term.println("scanning right");
            logged = true;
          }
          if (rearOBS())
          {
            fwd(170, 0);
            delay(400);
            simpleRight(180);//resume initial cmd
          }
          if ((BUMPR() && !BUMPL()) || fusupVAL_R() < 20)
          {
            rvs(170, 0);
            delay(400);
            left(200);
            delay(200);
            simpleRight(180);//resume initial cmd
          }
          MainFunctions();
          stuck();
          BUMP();
        }
      }
      else if ("undecided")
      {
        term.println("UNDECIDED");
        gettouhere();
      }


      checkAround();
      STOP();

      // in case got stuck, use binary tree to figure it out
      if (GotStuck) {
        Blink(10, 50); // signal a stuck management
        gettouhere();
      }
    }
    MainFunctions();
  }
  STOP();

  isSelfDriving = false;
  state = isSelfDriving ? "ROAMING" : "NOT ROAMING";
  _server.send(200, "text/html", state);
  // signal successful end of operations
  Blink(10, 10);
  // resume normal blinking operations
  noblink = false;

}


void checkAround() {
  if ((frontIR_RVal() > 500 || frontIR_LVal() > 500) || fusLVAL() < 4 || fusRVAL() < 4)
  {
    rvs(160, 0);
    delay(200);
  }
  if (rearOBS())
  {
    fwd(160, 0);
    delay(200);
  }
}
boolean freeWay() {
  return frontIR_RVal() < 100 && frontIR_LVal() < 100 && fusLVAL() > 100 && fusRVAL() > 100 && fusupVAL() > 100;
}

void gettouhere() {
  if (GotStuck && !STOPALL) {
    GotStuck = false;
    int c = 0;
    while (method == 0 && !STOPALL && c < 10) {
      term.println("method: " + String(method));
      method++;

      int Min = 0;
      int Max = 9;
      int val = random(Min, Max);
      unsigned long StartHere = millis();

      if (!rearOBS()) {
        rvs(170, 30);
        alreadyranback = true;
      }
      while (!STOPALL && frontIR_RVal() > IRlimit || frontIR_LVal() > IRlimit && millis() - StartHere > 4000) {
        method++;
        if (val < 5) {
          term.println("Left RANDOM (at gettouhere())");
          left(180);
          delay(300);
        }
        else {
          term.println("Right RANDOM (at gettouhere())");
          right(180);
          delay(300);
        }
      }
      if (OBS()) {
        rvs(180, 0);
        delay(100);
        left(180);
        delay(100);
        rvs(180, 0);
        delay(100);
        right(180);
        delay(100);
      }
      else {
        break;
      }
      MainFunctions();
      c++;
    }
    while (method == 1 && !STOPALL && c < 10) {
      term.println("method # " + String(method));
      method++;
      int Min = 0;
      int Max = 10;
      int val = random(Min, Max);
      unsigned long StartHere = millis();

      while (!STOPALL && frontIR_RVal() > 500 || frontIR_LVal() > 500 && millis() - StartHere > 4000) {
        method++;
        if (val < 5) {
          term.println("Left RANDOM (at gettouhere())");
          left(180);
          delay(300);
        }
        else {
          term.println("Right RANDOM (at gettouhere())");
          right(180);
          delay(300);
        }
      }
      left(180);
      delay(100);
      right(180);
      delay(100);
      MainFunctions();
      c++;
    }
    if (method == 2 && !STOPALL && c < 10) {
      term.println("method # " + String(method));
      method++;
      rvs(180, 10);
      if (leftIRVal() < rightIRVal()) {
        term.println("method # " + String(method));
        right(200);
        delay(1000);
      }
      else {
        term.println("method # " + String(method));
        left(200);
        delay(1000);
      }
      MainFunctions();
      c++;
    }
  }
  if (method > 2) {
    term.println("ALL DESTUCK METHODS FAILED");
    if (!rearOBS() && !alreadyranback) {
      rvs(170, 10);
      alreadyranback = true;
    }
    else {
      alreadyranback = false;
      fwd(180, 5);
    }
    method = 0;
    MainFunctions();
  }
  STOP();

}

/*

*/
