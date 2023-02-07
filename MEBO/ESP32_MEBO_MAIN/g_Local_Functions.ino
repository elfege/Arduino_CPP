boolean stuck() {

  int thres = 15;

  int DelayPreRec = 200; //intervals within which you record last encoder value
  int Delay = 1000; // time after which you'll compare current encoder values with previous record

  //  term.print( "cl BEFORE -- " );term.println( cl );
  //  term.print( "cr BEFORE -- " );term.println( cr );

  if (mvt && noturn && !noStuck || makeEncHappen)
  {
    // mvt: mvt boolean updated by encoder management in the workload tab and by mvt functions
    // noturn: noturn boolean updated by mvt functions
    // noStuck: noStuck is used in certain functions only
    // makeEncHappen: makeEncHappen to force encoders' count and stuck check while turning

    if (millis() - RecTime > DelayPreRec && !PreRecDone) // if you've been running for more than 200 millis, and so on every 200 millis, record values
    {
      // record a new test value only every XXX millis
      //term.println("Pre Record");
      RecTime = millis();
      ReadL = cl;
      ReadR = cr; // record values at this time
      PreRecDone = true; // this is in order to forbid any new value storage before comparison has been made
    }

    // now compare values and see if there was substantial change, if not then stuck is true
    if (millis() > RecTime + Delay && PreRecDone) // 200 millis after last record and ONLY IF PreRecDone is true
    {
      PreRecDone = false;
      //      term.println("Comparing " + String(Delay) + " millisecs after last record ");
      //      term.print("recorded ReadR = ");term.println(String(ReadR) + "compared to current cr = " + String(cr));
      //      term.print("recorded ReadL = ");term.println(String(ReadL) + "compared to current cl = " + String(cl));
      // compare values within defined time interval
      if (ReadL == cl || ReadR == cr ) {
        positive++; // run a couple more tests to confirm
        //term.print( "EQUAL  ============= " );term.println( positive );term.println( " ======================= " );
        // if one of them shows identical values
        if (positive == 1) { // record values the first time it happened
          secReadR = ReadR;
          secReadL = ReadL;
        }

        if (positive >= thres && (cr == secReadR || cl == secReadL)) {
          // if you had equal values for "thres" times in a row
          // AND if these values haven't changed since the first time you noticed that, then definitely return true
          positive = 0; // reset this value
          term.println( "-----------------STUCK-------------------------" );
          GotStuck = true; // will be reset by functions such as getouthere() and roam()
          resultStuck = true; // will stay true until further mvt
        }
      }
      else {
        //term.println("NOT -- stuck" );
        positive = 0; // if any encoder has changed/moved, reset this value to cancel possible false alarm and allow further testing
        resultStuck = false;
      }
    }
  }
  else {
    RecTime = millis();
    PreRecDone = false;
    //term.println("NOT stuck" );
    positive = 0; // reset this value to cancel possible false alarm and allow further testing
    // if no longer moving, keep record of the last returned value
  }

  if (positive > thres + 1) {
    secReadR = 0;
    secReadL = 0;
    positive = 0;
  }
  return resultStuck;
}



boolean unstuckRVSok() {
  bool result = leftIRVal() < 500 && rightIRVal() < 500 && RRIRVal() < 500 && RLIRVal() < 500 && frontIR_RVal() < 200 && frontIR_LVal() < 200;
  return result;
}


boolean SeeLineLeftFront() {
  return digitalRead(LineLeftFront);
}
boolean SeeLineRightFront() {
  return digitalRead(LineRightFront);
}

boolean SeeLineLeftRear() {
  return analogRead(LineLeftRear) == 2047;
}
boolean SeeLineRightRear() {
  return analogRead(LineRightRear) == 2047;
}

boolean OneSeesTheLineFront() {
  bool result = SeeLineLeftFront() || SeeLineRightFront();
  return result;
}

boolean OneSeesTheLineRear() {
  bool result = SeeLineLeftRear() || SeeLineRightRear() ;
  return result;
}

boolean BothFrontSeen() {
  bool result = SeeLineLeftFront() && SeeLineRightFront();

  if (result) {
    // double check
    delay(30);
  }
  result = SeeLineLeftFront() && SeeLineRightFront();
  return result;
}
boolean BothRearSeen() {
  bool result = SeeLineLeftRear() && SeeLineRightRear();

  if (result) {
    // double check
    delay(30);
  }
  result = SeeLineLeftRear() && SeeLineRightRear();
  return result;
}


boolean StationInSight() {
  //delay(50);

  if (irrecv.decode(&results)) {
    Signal = true;
    gotsignal = true;
    term.println(results.value, HEX);

    if (results.value == 0xa90) {
      term.println("DOCKING STATION !!!");
      found = true;
      gotstation = true; // same as found but isn't reset within this scope
    }
    else {
      found = false;
    }
    results.value = 0; // reset the value
    irrecv.resume(); // Receive the next value
  }
  else {
    Signal = false;
    found = false; // if signal no longer seen value won't reset, so reset it here
  }
  return found;
}



boolean thereIsMvt() {
  if (digitalRead(MOTOR_LEFT1) || digitalRead(MOTOR_LEFT2) || digitalRead(MOTOR_RIGHT1) || digitalRead(MOTOR_RIGHT2)) {
    return true;
  }
  else {
    return false;
  }
}

void Blink(int times, int lapse) {
  int c = 0;
  while (c < times) {
    digitalWrite(LED, 1);
    delay(lapse);
    digitalWrite(LED, 0);
    delay(lapse);
    c++;
  }
}
