
void fwd(int v, int dist) {
  disableCliff = false;

  if ((SeeLineLeft() || SeeLineLeft()) && !flwln && !cliffOFF) {

    Serial << "CLIFF !!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
  }
  else {
    noturn = true; // tells speed adjustments this is not a turn so ok to adjust
    mvt = true;
    docking = false;

    // dist is to be requested in cm
    // Serial.println("FORWARD");
    published = false;

    // if there was no recent http request, set to cmd requested speed
    if (!HttpReq) {
      rqSp = v; // tels speed adjustments where to start from
    }
    // otherwise, keep requested speed value as it is asked by the http request

    dcR = rqSp;
    dcL = rqSp;
    mapSpeed(); // for encoders



    //  if (digitalRead(RELAY1) != 1) {
    //    poweron();
    //  }
    if (digitalRead(RELAY2) == 0) {
      stopcharge();
    }

    ledcWrite(pwmChannelL, rqSp);
    ledcWrite(pwmChannelR, rqSp);
    digitalWrite(PWM_LEFT1,  A);
    digitalWrite(PWM_LEFT2,  B);
    digitalWrite(PWM_RIGHT1, B);
    digitalWrite(PWM_RIGHT2, A);

    delay(10); // allow wheels to engage a little before adjusting speed with encoders


    if (dist != 0) {
      noturn = true; // tels speed adjustments this is not a turn so ok to adjust
      mvt = true;
      euristics = true; // allows GotStuck global variable to refresh
      makedist(dist);

    }

  }

}

void rvs(int v, int dist) {
  noturn = true; // tells speed adjustments this is not a turn so ok to adjust
  mvt = true;

  published = false;
  // if there was no recent http request, set to cmd requested speed
  if (!HttpReq) {
    rqSp = v; // tels speed adjustments where to start from
  }
  // otherwise, keep requested speed value as it is asked by the http request

  dcR = rqSp;
  dcL = rqSp;
  mapSpeed(); // for encoders

  //  if (digitalRead(RELAY1) != 1) {
  //    poweron();
  //  }
  if (digitalRead(RELAY2) == 0) {
    stopcharge();
  }
  //  Serial.println("REVERSE");
  ledcWrite(pwmChannelL, rqSp);
  ledcWrite(pwmChannelR, rqSp);
  digitalWrite(PWM_LEFT1,  B);
  digitalWrite(PWM_LEFT2,  A);
  digitalWrite(PWM_RIGHT1, A);
  digitalWrite(PWM_RIGHT2, B);

  delay(10); // allow wheels to engage a little before adjusting speed with encoders


  if (dist != 0) {
    noturn = true; // tells speed adjustments this is not a turn so ok to adjust
    mvt = true;
    euristics = true;
    makedist(dist);
  }



}

void left(int v) {
  if ((SeeLineLeft() || SeeLineLeft()) && !flwln && !cliffOFF) {
    Serial << "CLIFF !!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
  }
  else {
    published = false;
    noturn = false; // tells speed adjustments this is a turn so NOT ok to adjust
    //  if (digitalRead(RELAY1) != 1) {
    //    poweron();
    //  }
    if (digitalRead(RELAY2) == 0) {
      stopcharge();
    }
    mvt = true;

    // if there was no recent http request, set to cmd requested speed
    if (!HttpReq) {
      rqSp = v; // tels speed adjustments where to start from
    }
    // otherwise, keep requested speed value as it is asked by the http request


    ledcWrite(pwmChannelL, rqSp);
    ledcWrite(pwmChannelR, rqSp);
    digitalWrite(PWM_LEFT1,  B);
    digitalWrite(PWM_LEFT2,  A);
    digitalWrite(PWM_RIGHT1, B);
    digitalWrite(PWM_RIGHT2, A);

    //Serial.println("LEFT");
  }
}

void right(int v) {
  if ((SeeLineLeft() || SeeLineLeft()) && !flwln && !cliffOFF) {
    Serial << "CLIFF !!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
  }
  else {
    published = false;
    noturn = false; // tels speed adjustments this is a turn so NOT ok to adjust
    //  if (digitalRead(RELAY1) != 1) {
    //    poweron();
    //  }
    if (digitalRead(RELAY2) == 0) {
      stopcharge();
    }
    mvt = true;
    // if there was no recent http request, set to cmd requested speed
    if (!HttpReq) {
      rqSp = v; // tels speed adjustments where to start from
    }
    // otherwise, keep requested speed value as it is asked by the http request

    ledcWrite(pwmChannelL, rqSp);
    ledcWrite(pwmChannelR, rqSp);
    digitalWrite(PWM_LEFT1,  A);
    digitalWrite(PWM_LEFT2,  B);
    digitalWrite(PWM_RIGHT1, A);
    digitalWrite(PWM_RIGHT2, B);
    delay(1);

    //Serial.println("RIGHT");
  }
}

void STOP() {

  ledcWrite(pwmChannelL, 0);
  ledcWrite(pwmChannelR, 0);
  digitalWrite(PWM_LEFT1,  0);
  digitalWrite(PWM_LEFT2,  0);
  digitalWrite(PWM_RIGHT1, 0);
  digitalWrite(PWM_RIGHT2, 0);

  mvt = false;
  noturn = false;
  cr = 0;
  cl = 0;

  dutyCycle = Default;

}

void simpleSTOP() {

  ledcWrite(pwmChannelL, 0);
  ledcWrite(pwmChannelR, 0);
  digitalWrite(PWM_LEFT1,  0);
  digitalWrite(PWM_LEFT2,  0);
  digitalWrite(PWM_RIGHT1, 0);
  digitalWrite(PWM_RIGHT2, 0);
}

void unstuck() {
  Serial.println("UNSTUCKING++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

  DELAY = 400; // random(50, 200);
  SPEED = 180; // random(170, 180);

  right(SPEED);
  delay(DELAY);
  fwd(SPEED, 10);
  delay(DELAY);
  left(SPEED);
  delay(DELAY);
  fwd(SPEED, 10);

  STOP();

}

void unstuckRVS() {
  Serial.println("UNSTUCKING RVS -----------------000000000000000000RRRRRRRRRRRRRRRRRRRRRRR");

  DELAY = 50; // random(50, 200);
  SPEED = 180; // random(170, 180);
  int IRdist = 500;
  unsigned long Sb;
  unsigned long Sa = millis();

  while (!unstuckRVSok() && !STOPALL && !haspower() && millis() - Sa < 4000) {

    Sb = millis();
    while (analogRead(LIR) > analogRead(RIR) && !unstuckRVSok() && !STOPALL && !haspower() && millis() - Sb < 500) {
      right(SPEED);
      delay(DELAY);
      rvs(SPEED, 0);
      delay(DELAY);
      loop();
    }
    Sb = millis();
    while (analogRead(LIR) < analogRead(RIR) && !unstuckRVSok() && !STOPALL && !haspower() && millis() - Sb < 500) {
      left(SPEED);
      delay(DELAY);
      rvs(SPEED, 0);
      delay(DELAY);
      loop();
    }

    loop();
  }
  if (analogRead(LIR) > analogRead(RIR)) {
    right(SPEED);
  }
  else {
    left(SPEED);
  }
  delay(500);
  Serial.println("unstuckRVS DONE..");
  STOP();
}



void FollowLineFWD() {

  flwln = true; // disable cliff sensing

  int last = 0;
  STOPALL = false;
  SPEED = 170;
  int del = 50;
  int test = 0;
  unsigned long GStart = millis();//  set a time limit
  bumped = false;

  while (!STOPALL && !BothFrontSeen() && millis() - GStart < 20000 && !bumped && !frontIR()) {
    loop();


    //Start = millis();//&& !STOPALL && millis() - Start < 5) {
    fwd(SPEED, 0);
    //noturn = false;

    while (!OneSeesTheLineFront() && !STOPALL && !BothFrontSeen() && !bumped  && !frontIR()) {
      loop();
    }
    while (SeeLineLeft() && !STOPALL && !BothFrontSeen() && !bumped  && !frontIR()) {
      loop();
      left(MEDIUM);
      delay(del);
      fwd(SPEED, 0);
      delay(del);
    }
    while (SeeLineRight() && !STOPALL && !BothFrontSeen() && !bumped && !frontIR()) {
      loop();
      right(MEDIUM);
      delay(del);
      fwd(SPEED, 0);
      delay(del);
    }

    // test to confirm stop sign
    //    if (BothFrontSeen()) {
    //
    //      fwd(255, 3);
    //      delay(500);
    //      rvs(255, 2);
    //      delay(500);
    //
    //      if (BothFrontSeen()) {
    //        break;
    //      }
    //
    //      test++;
    //    }

  }
  STOP();

  STOPALL = false;
  flwln = false; // enable cliff sensing

}

