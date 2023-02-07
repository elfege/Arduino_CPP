
void selfdrive() {
  cliffOFF = false;// should be false, just testing
  ignoreobs = true;
  STOPALL = false;
  docking = false;
  boolean rvsdone = false;

  boolean wentLeft = false;
  allSet = false; // don't check bump and fusup in main loop
  euristics = false; // don't activate euristics (if true it activates bump() and fusup() tests in main loop

  SPEED = 160;

  methods = 0; // binary tree A.I.
  int USdist = 30;
  GotStuck = false;


  unsigned long StartingPoint;
  unsigned long StartHere;

  while (!STOPALL) {

    check = true; // tell main loop that this loop is already taking care of the stuck test, or not
    bumped = false;
    GotStuck = false;

    StartingPoint = millis();
    while (!STOPALL && !OBS() && !GotStuck && millis() - StartingPoint < 10000) {
      fwd(SPEED, 0);

      if (!STOPALL && fusLVAL() < 20 && fusRVAL() > 20 && !OBS()) {
        Serial << "LEFT FRONT SIDE AVOIDANCE" << endl;
        rvs(180, 5);
        fwd(SPEED, 0);
        noturn = false; // disable encoders balancing
        ledcWrite(pwmChannelR, 140);
        ledcWrite(pwmChannelL, 200);
        StartHere = millis();
        while (!STOPALL && fusLVAL() < 20 && !OBS() && millis() - StartHere < 800 && !GotStuck) {
          loop();
        }
        delay(1);
        if (millis() - StartHere > 800) {
          Serial.println("BREAK..");
          rvs(180, 10);
        }
      }
      else if (!STOPALL && fusRVAL() < 20 && fusLVAL() > 20 && !OBS()) {
        Serial << "RIGHT FRONT SIDE AVOIDANCE" << endl;
        rvs(180, 5);
        fwd(SPEED, 0);
        noturn = false; // stop encoders balancing
        ledcWrite(pwmChannelR, 200);
        ledcWrite(pwmChannelL, 140);
        StartHere = millis();
        while (!STOPALL && fusRVAL() < 20 && !OBS() && millis() - StartHere < 800 && !GotStuck) {
          loop();
        }
        delay(1);
        if (millis() - StartHere > 800) {
          Serial.println("BREAK..");
          StartHere = millis();
          rvs(180, 0);
          while (!STOPALL && !rearOBS() && millis() - StartHere < 800 && !GotStuck) {
            loop();
          }
          STOP();
          if (rearOBS()) {
          GotStuck = true;
        }
        break;
      }
    }
    delay(1);
    }

    // in case got stuck, use A.I. binary tree to figure it out
    if (GotStuck) {
      gettouhere();
    }

    // for all other cases, reverse after obstacle detection
    if (!rearOBS()) {
      Serial.println("STANDARD REVERSE AVOIDANCE");
      rvs(180, 5);
    }
    else {
      Serial.println("NO REVERSE DUE TO REAR OBSTACLE");
      fwd(160, 5);
    }
    STOP();
    if (leftIRVal() < rightIRVal() && !leftOBS()) {
      Serial.println("right Turn");
      left(180);
      wentLeft = true;
    }
    else if (rightIRVal() < leftIRVal() && !rightOBS()) {
      Serial.println("Left Turn");
      right(180);
      wentLeft = false;
    }
    else {
      if (!rearOBS()) {
        rvs(180, 0);
        delay(200);
        STOP();
      }
      int Min = 0;
      int Max = 10;
      int val = random(Min, Max);
      if (val < 5) {
        Serial.println("Left RANDOM");
        left(180);
      }
      else {
        Serial.println("Right RANDOM");
        right(180);
      }
    }

    delay(200);
    STOP();

    StartHere = millis();
    if (wentLeft) {
      Serial.println("scanning left");

      while ((frontIRVal() > 300 || fusRVAL() < 30) && !STOPALL && !leftOBS() && millis() - StartHere < 4000) {
        left(180);
        delay(50);
        STOP();
        delay(50);
        loop();
      }
      fwd(200, 5);
    }
    else {
      StartHere = millis();
      Serial.println("scanning right");

      while ((frontIRVal() > 300 || fusLVAL() < 30) && !STOPALL && !rightOBS() && millis() - StartHere < 4000) {
        right(180);
        delay(50);
        STOP();
        delay(50);
        loop();
      }
      fwd(200, 5);
    }
    if (millis() - StartHere >= 4000) {
      // probably stuck while scanning
      rvs(180, 10);
    }

    loop();
  }
  STOP();
  STOPALL = true;
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
      fwd(200, 5);
    }
    methods = 0;

    loop();
  }
  STOP();

}
