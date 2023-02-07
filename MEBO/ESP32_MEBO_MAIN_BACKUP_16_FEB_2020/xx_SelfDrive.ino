
void selfdrive() {
  /*******************************************************************
    BUMP AND STUCK ARE ALWAYS CHECKED IN THE MAIN LOOP, NO MATTER WHAT!
  ************** *****************************************************/
  logs = false; // stop debugging logs
  isSelfDriving = true;
  String state = (isSelfDriving) ? "ROAMING" : "NOT ROAMING";
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

    fwd(thisSPEED, 0);
    StartingPoint = millis();
    while (!STOPALL && !bumped && (frontIR_RVal() < 500 && frontIR_LVal() < 500) && fusupVAL() > 50 && fusLVAL() > 5 && fusRVAL() > 5 && !GotStuck && millis() - StartingPoint < 10000)
    {
      loop();
      stuck();
      BUMP();
    }
    if (!STOPALL) {
      rvs(170, 0);
      delay(300);
      STOP();
      delay(100);

      if (leftIRVal() < rightIRVal()) {
        Serial.println("scanning left");
        StartHere = millis();

        left(200);
        delay(100);

        while (!freeWay() && !STOPALL && !leftOBS() && millis() - StartHere < 5000)
        {
          left(200);
          delay(100);
          checkAround();
          loop();
        }
      }
      else if (rightIRVal() < leftIRVal()) {

        Serial.println("scanning right");
        StartHere = millis();

        right(200);
        delay(100);

        while (!freeWay() && !STOPALL && !rightOBS() && millis() - StartHere < 5000)
        {
          right(200);
          delay(100);
          checkAround();
          loop();
        }
      }
      else
      {
        left(200);
        delay(100);
      }

      STOP();
      digitalWrite(LED, 0); // steady ON LED, signaling obstacles management

      // in case got stuck, use A.I. binary tree to figure it out
      if (GotStuck) {
        Blink(3, 200); // signal a stuck management
        gettouhere();
      }
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


void checkAround()
{
  if ((frontIR_RVal() > 500 || frontIR_LVal() > 500) || fusLVAL() < 4 || fusRVAL() < 4)
  {
    rvs(190, 0);
    delay(300);
  }
  if (rearOBS())
  {
    fwd(180, 0);
    delay(100);
  }
}
boolean freeWay()
{
  return frontIR_RVal() < 200 || frontIR_LVal() < 200 && fusLVAL() > 50 && fusRVAL() > 50;
}

void gettouhere() {
  if (GotStuck && !STOPALL) {
    GotStuck = false;
    int c = 0;
    while (method == 0 && !STOPALL && c < 10) {
      Serial << "method # " << method << endl;
      method++;

      int Min = 0;
      int Max = 10;
      int val = random(Min, Max);
      unsigned long StartHere = millis();

      if (!rearOBS()) {
        rvs(170, 30);
        alreadyranback = true;
      }

      while (!STOPALL && frontIR_RVal() > 500 || frontIR_LVal() > 500 && millis() - StartHere > 4000) {
        method++;
        if (val < 5) {
          Serial.println("Left RANDOM (at gettouhere())");
          left(180);
          delay(300);
        }
        else {
          Serial.println("Right RANDOM (at gettouhere())");
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
      loop();
      c++;
    }

    while (method == 1 && !STOPALL && c < 10) {
      Serial << "method # " << method << endl;
      method++;
      int Min = 0;
      int Max = 10;
      int val = random(Min, Max);
      unsigned long StartHere = millis();

      while (!STOPALL && frontIR_RVal() > 500 || frontIR_LVal() > 500 && millis() - StartHere > 4000) {
        method++;
        if (val < 5) {
          Serial.println("Left RANDOM (at gettouhere())");
          left(180);
          delay(300);
        }
        else {
          Serial.println("Right RANDOM (at gettouhere())");
          right(180);
          delay(300);
        }
      }
      left(180);
      delay(100);
      right(180);
      delay(100);
      loop();
      c++;
    }

    if (method == 2 && !STOPALL && c < 10) {
      Serial << "method # " << method << endl;
      method++;
      rvs(180, 10);
      if (leftIRVal() < rightIRVal()) {
        Serial << "method # " << method << " RIGHT" << endl;
        right(200);
        delay(1000);
      }
      else {
        Serial << "method # " << method << " LEFT" << endl;
        left(200);
        delay(1000);
      }
      loop();
      c++;
    }
  }
  if (method > 2) {
    Serial.println("ALL DESTUCK METHODS FAILED");

    if (!rearOBS() && !alreadyranback) {
      rvs(170, 10);
      alreadyranback = true;
    }
    else {
      alreadyranback = false;
      fwd(180, 5);
    }
    method = 0;

    loop();
  }
  STOP();

}

/*

*/
