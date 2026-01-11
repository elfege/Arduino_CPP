void fwd(int v, int dist) {

  disableCliff = false;

  MvtState = "Forward";

  Serial.println("Moving Fwd");

  if (Cliff() && !flwln && !cliffOFF) {

    Serial.println ("CLIFF!");
    STOP();// to update MvtState
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
      Serial.println("This is an http speed request");
      rqSp = v; // tels speed adjustments where to start from
    }
    // otherwise, keep requested speed value as it is asked by the http request

    dcR = rqSp;
    dcL = rqSp;
    mapSpeed(); // for encoders

    if (digitalRead(CHARGERelay) == 0) {
      stopcharge();
    }

    if (!flushing) {
      ledcWrite(pwmChannelL, rqSp);
      ledcWrite(pwmChannelR, rqSp);
      digitalWrite(MOTOR_LEFT1,  A);
      digitalWrite(MOTOR_LEFT2,  B);
      digitalWrite(MOTOR_RIGHT1, B);
      digitalWrite(MOTOR_RIGHT2, A);

    }
    else {
      Serial.println("FLUSHING BUFFER.......NO MVT...");
    }

    delay(10); // allow wheels to engage a little before adjusting speed with encoders

    if (dist != 0) {
      noturn = true; // tels speed adjustments this is not a turn so ok to adjust
      mvt = true;
      euristics = true; // allows GotStuck global variable to refresh
      Serial.println("STARTING makedist()");
      makedist(dist);

    }
  }
}

void rvs(int v, int dist) {
  noturn = true; // tells speed adjustments this is not a turn so ok to adjust
  mvt = true;

  MvtState = "Reverse";

  Serial.println("REVERSE");

  published = false;
  // if there was no recent http request, set to cmd requested speed
  if (!HttpReq) {
    rqSp = v; // tels speed adjustments where to start from
  }
  // otherwise, keep requested speed value as it is asked by the http request

  dcR = rqSp;
  dcL = rqSp;
  mapSpeed(); // for encoders


  if (digitalRead(CHARGERelay) == 0) {
    stopcharge();
  }

  //  Serial.println("REVERSE");

  if (!flushing) {
    ledcWrite(pwmChannelL, rqSp);
    ledcWrite(pwmChannelR, rqSp);
    digitalWrite(MOTOR_LEFT1,  B);
    digitalWrite(MOTOR_LEFT2,  A);
    digitalWrite(MOTOR_RIGHT1, A);
    digitalWrite(MOTOR_RIGHT2, B);


  }
  else {
    Serial.println("FLUSHING BUFFER.......NO MVT...");
  }

  delay(10); // allow wheels to engage a little before adjusting speed with encoders


  if (dist != 0) {
    noturn = true; // tells speed adjustments this is not a turn so ok to adjust
    mvt = true;
    euristics = true;
    makedist(dist);
  }



}

void left(int v) {

  MvtState = "Left";

  Serial.println("moving left" );

  if (Cliff() && !flwln && !cliffOFF) {
    Serial.println("CLIFF!" );
  }
  else {
    published = false;
    noturn = false; // tells speed adjustments this is a turn so NOT ok to adjust

    if (digitalRead(CHARGERelay) == 0) {
      stopcharge();
    }
    mvt = true;

    // if there was no recent http request, set to cmd requested speed
    if (!HttpReq) {
      rqSp = v; // tels speed adjustments where to start from
      if (rqSp < 170) {
        rqSp = 170;
      }
    }
    // otherwise, keep requested speed value as it is asked by the http request

    if (!flushing) {
      ledcWrite(pwmChannelL, rqSp);
      ledcWrite(pwmChannelR, rqSp);
      digitalWrite(MOTOR_LEFT1,  B);
      digitalWrite(MOTOR_LEFT2,  A);
      digitalWrite(MOTOR_RIGHT1, B);
      digitalWrite(MOTOR_RIGHT2, A);
    }
    else {
      Serial.println("FLUSHING BUFFER.......NO MVT...");
    }
    //Serial.println("LEFT");
  }
}

void right(int v) {

  Serial.println("moving right" );

  MvtState = "Right";

  if (Cliff() && !flwln && !cliffOFF) {
    Serial.println("CLIFF!" );
  }
  else {
    published = false;
    noturn = false; // tels speed adjustments this is a turn so NOT ok to adjust

    if (digitalRead(CHARGERelay) == 0) {
      stopcharge();
    }
    mvt = true;
    // if there was no recent http request, set to cmd requested speed
    if (!HttpReq) {
      rqSp = v; // tels speed adjustments where to start from
      if (rqSp < 170) {
        rqSp = 170;
      }
    }
    // otherwise, keep requested speed value as it is asked by the http request

    if (!flushing) {
      ledcWrite(pwmChannelL, rqSp);
      ledcWrite(pwmChannelR, rqSp);
      digitalWrite(MOTOR_LEFT1,  A);
      digitalWrite(MOTOR_LEFT2,  B);
      digitalWrite(MOTOR_RIGHT1, A);
      digitalWrite(MOTOR_RIGHT2, B);
    }
    else {
      Serial.println("FLUSHING BUFFER.......NO MVT...");
    }
    delay(1);

    //Serial.println("RIGHT");
  }
}

void STOP() {

  Serial.println("STOP!");
  MvtState = "STOP";

  ledcWrite(pwmChannelL, 0);
  ledcWrite(pwmChannelR, 0);
  digitalWrite(MOTOR_LEFT1,  0);
  digitalWrite(MOTOR_LEFT2,  0);
  digitalWrite(MOTOR_RIGHT1, 0);
  digitalWrite(MOTOR_RIGHT2, 0);

  mvt = false;
  noturn = false;

  Serial.print( "cr ARRIVAL = " ); Serial.println( cr  );
  Serial.print( "cl ARRIVAL = " ); Serial.println( cl  );
  cr = 0;
  cl = 0;

  dutyCycle = Default;


  //Serial.println("----------------STOP--------------------");

}

void simpleSTOP() {

  ledcWrite(pwmChannelL, 0);
  ledcWrite(pwmChannelR, 0);
  digitalWrite(MOTOR_LEFT1,  0);
  digitalWrite(MOTOR_LEFT2,  0);
  digitalWrite(MOTOR_RIGHT1, 0);
  digitalWrite(MOTOR_RIGHT2, 0);
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

void fullStop() {
  digitalWrite(MOTOR_LEFT1,  0);
  digitalWrite(MOTOR_LEFT2,  0);
  digitalWrite(MOTOR_RIGHT1, 0);
  digitalWrite(MOTOR_RIGHT2, 0);

  Serial.println("FULL STOP");
}

void unstuckRVS() {
  //  Serial.println("UNSTUCKING RVS -----------------000000000000000000RRRRRRRRRRRRRRRRRRRRRRR");
  //
  //  DELAY = 50; // random(50, 200);
  //  SPEED = 180; // random(170, 180);
  //  int IRdist = 500;
  //  unsigned long Sb;
  //  unsigned long Sa = millis();
  //
  //  while (!unstuckRVSok() && !STOPALL && !haspower() && millis() - Sa < 4000) {
  //
  //    Sb = millis();
  //    while (leftIRVal() > rightIRVal() && !unstuckRVSok() && !STOPALL && !haspower() && millis() - Sb < 500) {
  //      right(SPEED);
  //      delay(DELAY);
  //      rvs(SPEED, 0);
  //      delay(DELAY);
  //      loop();
  //    }
  //    Sb = millis();
  //    while (leftIRVal() < rightIRVal() && !unstuckRVSok() && !STOPALL && !haspower() && millis() - Sb < 500) {
  //      left(SPEED);
  //      delay(DELAY);
  //      rvs(SPEED, 0);
  //      delay(DELAY);
  //      loop();
  //    }
  //
  //    loop();
  //  }
  //  if (leftIRVal() > rightIRVal()) {
  //    right(SPEED);
  //  }
  //  else {
  //    left(SPEED);
  //  }
  //  delay(500);
  //  Serial.println("unstuckRVS DONE..");
  //  STOP();
}



void FollowLineFWD() {
  String state = "Leaving docking station";
  _server.send(200, "text/plain", state);

  flwln = true; // disable cliff sensing (deprecated)
  int last = 0;
  STOPALL = false;
  SPEED = 160;
  int del = 50;
  int test = 0;
  unsigned long GStart = millis();//  set a time limit
  bumped = false;
  noblink = true; // blink led takes millis...

  while (!STOPALL && !BothFrontSeen() && millis() - GStart < 20000 && !bumped && !frontIR()) {
    loop();
    fwd(SPEED, 0);
    Serial.println("G FWD");

    execLineFwd();

  }
  STOP();
  noblink = false;

  STOPALL = false;
  flwln = false; // enable cliff sensing

}

void FollowLineRVS() {
  stopcharge();
  delay(10);

  String state = "Docking with line";
  _server.send(200, "text/plain", state);

  check = true; // allow GotStuck to be checked
  GotStuck = false;
  flwln = true; // disable cliff sensing (deprecated)
  int last = 0;
  STOPALL = false;
  SPEED = 160;
  int del = 50;
  int test = 0;
  unsigned long GStart = millis();//  set a time limit
  bumped = false;
  noblink = true; // blink led takes millis...


  while (!STOPALL && !haspower() && millis() - GStart < 120000 && !bumped && !frontIR()) {
    loop();
    rvs(SPEED, 0);
    Serial.println("G RVS");


    while (SeeLineLeftRear() && !STOPALL && !haspower() && !bumped  && !frontIR() && !GotStuck) {
      loop();
      right(180);
      delay(100);
      rvs(SPEED, 0);
      Serial.print("left  "); Serial.println(SeeLineLeftFront());
      Serial.println("SL RVS");
      delay(del);
    }
    while (SeeLineRightRear() && !STOPALL && !haspower() && !bumped && !frontIR() && !GotStuck) {
      loop();
      left(180);
      delay(100);
      rvs(SPEED, 0);
      Serial.print("right "); Serial.println(SeeLineRightFront());
      Serial.println("SR RVS");
      delay(del);
    }
    if (GotStuck) {
      GotStuck = false;
      Serial.println("EXECUTING LINE FWD!!");
      unsigned long thisStart = millis();
      while (!STOPALL && !BothFrontSeen() && millis() - thisStart < 3000 && !bumped && !frontIR()) {
        loop();
        fwd(SPEED, 0);
        Serial.println("G FWD");
        execLineFwd();
      }
    }
  }


  STOP();

  noblink = false;

  STOPALL = false;
  flwln = false; // enable cliff sensing

  if (haspower()) {
    charge();
    _server.send(200, "text/plain", "CHARGING");
  }

}

void execLineFwd() {
  int del = 50;
  while (SeeLineLeftFront() && !STOPALL && !BothFrontSeen() && !bumped  && !frontIR()) {
    loop();
    left(180);
    delay(100);
    fwd(SPEED, 0);
    Serial.print("left  "); Serial.println(SeeLineLeftFront());
    Serial.println("SL FWD");
    delay(del);
  }
  while (SeeLineRightFront() && !STOPALL && !BothFrontSeen() && !bumped && !frontIR()) {
    loop();
    right(180);
    delay(100);
    fwd(SPEED, 0);
    Serial.print("right "); Serial.println(SeeLineRightFront());
    Serial.println("SR FWD");
    delay(del);
  }
}
