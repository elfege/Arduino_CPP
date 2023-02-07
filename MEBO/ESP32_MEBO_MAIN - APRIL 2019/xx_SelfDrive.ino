
void selfdrive() {
  /*******************************************************************
    BUMP AND STUCK ARE ALWAYS CHECKED IN THE MAIN LOOP, NO MATTER WHAT!
  ************** *****************************************************/

  isSelfDriving = true;
  String state = (isSelfDriving) ? "ROAMING" : "NOT ROAMING";
  _server.send(200, "text/html", state);

  cliffOFF = false;// should be false, except for tests
  ignoreobs = true;// should be true, except for tests
  STOPALL = false;
  docking = false;// should be false, except for tests
  boolean rvsdone = false;
  boolean wentLeft = false;

  runIRreceiver = false; // don't activate IR runIRreceiver

  int thisSPEED = 160;

  methods = 0; // binary tree A.I.
  int USdist = 30;
  GotStuck = false;

  noblink = true; // don't allow LED blink for it generates delays


  unsigned long StartingPoint;
  unsigned long StartHere;

  while (!STOPALL) {

    check = true; // tell main loop that this loop is already taking care of the stuck test, or not
    bumped = false;
    GotStuck = false;

    StartingPoint = millis();

    while (!STOPALL && (frontIRVal() < 500 && fusupVAL() > 50 && fusLVAL() > 30 && fusRVAL() > 30) && !GotStuck && millis() - StartingPoint < 10000) {

      //Serial.print("frontIRVal() = "); Serial.println(frontIRVal());
      //Serial.print("fusupVAL() = "); Serial.println(fusupVAL());
      fwd(thisSPEED, 0);

      digitalWrite(LED, 1); // turn on the LED
      delay(10);
      digitalWrite(LED, 0); // turn on the LED
    }

    digitalWrite(LED, 1); // steady ON LED, signaling obstacles management

    // in case got stuck, use A.I. binary tree to figure it out
    if (GotStuck) {
      Blink(3, 50); // signal a stuck management
      gettouhere();
    }



    // for all other cases, reverse after obstacle detection

    Serial.println("STANDARD REVERSE AVOIDANCE");
    Blink(5, 30); // signal
    if (!rearOBS()) {
      rvs(thisSPEED, 10);
    }

    delay(500);
    if (bumped) {
      bumped = false;
      if (leftIRVal() < rightIRVal() && !leftOBS() || fusupVAL_L() > fusupVAL_R()) {
        left(200);
        delay(400);
      }
      else if (rightIRVal() < leftIRVal() && !rightOBS() || fusupVAL_R() > fusupVAL_L()) {
        right(200);
        delay(400);
      }
      else {
        left(200);
        delay(400);
        STOP();
      }
    }




    if (leftIRVal() < rightIRVal() && !leftOBS() || fusupVAL_L() > fusupVAL_R()) {
      Serial.println("scanning left");
      StartHere = millis();
      left(170);
      while ((frontIRVal() > 500 || fusRVAL() < 60 || fusLVAL() < 60 || fusupVAL() < 60) && !STOPALL && !leftOBS() && millis() - StartHere < 4000) {
        digitalWrite(LED, 1);
        loop();
        digitalWrite(LED, 0);
      }
    }
    else if (rightIRVal() < leftIRVal() && !rightOBS() || fusupVAL_R() > fusupVAL_L()) {

      Serial.println("scanning right");
      StartHere = millis();
      right(170);
      while ((frontIRVal() > 500 || fusLVAL() < 60 || fusRVAL() < 60 || fusupVAL() < 60) && !STOPALL && !rightOBS() && millis() - StartHere < 4000) {
        digitalWrite(LED, 1);
        loop();
        digitalWrite(LED, 0);
      }
    }
    else {
      digitalWrite(LED, 1);
      left(250);
      delay(500);
      STOP();
    }

    loop();

  }
  STOP();

  isSelfDriving = false;
  state = (isSelfDriving) ? "ROAMING" : "NOT ROAMING";
  _server.send(200, "text/html", state);
  // signal successful end of operations
  Blink(10, 10);
  // resume normal blinking operations
  noblink = false;

}

void gettouhere() {
  if (GotStuck && !STOPALL) {
    GotStuck = false;
    int c = 0;
    while (methods == 0 && !STOPALL && c < 10) {
      Serial << "method # " << methods << endl;
      methods++;

      int Min = 0;
      int Max = 10;
      int val = random(Min, Max);
      unsigned long StartHere = millis();

      if (!rearOBS()) {
        rvs(170, 10);
        alreadyranback = true;
      }

      while (!STOPALL && frontIRVal() > 400 && millis() - StartHere > 4000) {
        methods++;
        if (val < 5) {
          Serial.println("Left RANDOM (at gettouhere())");
          left(180);
        }
        else {
          Serial.println("Right RANDOM (at gettouhere())");
          right(180);
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
      loop();
      c++;
    }

    while (methods == 1 && !STOPALL && c < 10) {
      Serial << "method # " << methods << endl;
      methods++;
      int Min = 0;
      int Max = 10;
      int val = random(Min, Max);
      unsigned long StartHere = millis();

      while (!STOPALL && frontIRVal() > 400 && millis() - StartHere > 4000) {
        methods++;
        if (val < 5) {
          Serial.println("Left RANDOM (at gettouhere())");
          left(180);
        }
        else {
          Serial.println("Right RANDOM (at gettouhere())");
          right(180);
        }
      }
      left(180);
      delay(100);
      right(180);
      delay(100);
      loop();
      c++;
    }

    while (methods == 2 && !STOPALL && c < 10 && !rearOBS()) {
      Serial << "method # " << methods << endl;
      methods++;
      rvs(180, 10);
      if (leftIRVal() < rightIRVal()) {
        Serial << "method # " << methods << " RIGHT" << endl;
        right(200);
        delay(300);
      }
      else {
        Serial << "method # " << methods << " LEFT" << endl;
        left(200);
        delay(300);
      }
      loop();
      c++;
    }
  }
  if (methods > 2) {
    Serial.println("ALL DESTUCK METHODS FAILED");

    if (!rearOBS() && !alreadyranback) {
      rvs(170, 10);
      alreadyranback = true;
    }
    else {
      alreadyranback = false;
      fwd(180, 5);
    }
    methods = 0;

    loop();
  }
  STOP();

}
