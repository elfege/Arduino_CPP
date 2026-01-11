void fwd(int v, int dist) {

  disableCliff = false;

  MvtState = "Forward";

  //Serial.println("moving Fwd");

  if (Cliff() && !flwln && !cliffOFF) {

    Serial.println ("CLIFF!");
    STOP();// to update MvtState
  }
  else {
    noturn = true; // tells speed adjustments this is not a turn so ok to adjust
    mvt = true;

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

    if (dist != 0) {
      noturn = true; // tels speed adjustments this is not a turn so ok to adjust
      mvt = true;
      //      runIRreceiver = true; // no longer needed here (bump and stuck refresh no matter what) and it's toxic to SelfDrive
      Serial.println("STARTING makedist()");
      makedist(dist, rqSp, "fwd");

    }
    else {

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
    }

  }
}

void rvs(int v, int dist) {
  noturn = true; // tells speed adjustments this is not a turn so ok to adjust
  mvt = true;

  MvtState = "Reverse";

  // Serial.println("REVERSE");

  published = false;
  // if there was no recent http request, set to cmd requested speed
  if (!HttpReq) {
    rqSp = v; // tels speed adjustments where to start from
  }
  // otherwise, keep requested speed value as it is asked by the http request

  dcR = rqSp;
  dcL = rqSp;
  mapSpeed(); // for encoders

  if (dist != 0) {
    noturn = true; // tells speed adjustments this is not a turn so ok to adjust
    mvt = true;
    //      runIRreceiver = true; // no longer needed here (bump and stuck refresh no matter what) and it's toxic to SelfDrive
    makedist(dist, rqSp, "rvs");
  }
  else
  {
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
  }
}

void left(int v) {

  MvtState = "Left";

  //Serial.println("moving left" );

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

    // otherwise, keep requested speed value as it is asked by the http request

    if (!flushing) {
      ledcWrite(pwmChannelL, 200);
      ledcWrite(pwmChannelR, 200);
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

  //Serial.println("moving right" );

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


    if (!flushing) {
      ledcWrite(pwmChannelL, 200);
      ledcWrite(pwmChannelR, 200);
      digitalWrite(MOTOR_LEFT1,  A);
      digitalWrite(MOTOR_LEFT2,  B);
      digitalWrite(MOTOR_RIGHT1, A);
      digitalWrite(MOTOR_RIGHT2, B);
    }
    else {
      Serial.println("FLUSHING BUFFER.......NO MVT...");
    }

    //Serial.println("RIGHT");
  }
}

void STOP() {


  //Serial.println("STOP!");
  MvtState = "STOP";

  ledcWrite(pwmChannelL, 0);
  ledcWrite(pwmChannelR, 0);
  digitalWrite(MOTOR_LEFT1,  0);
  digitalWrite(MOTOR_LEFT2,  0);
  digitalWrite(MOTOR_RIGHT1, 0);
  digitalWrite(MOTOR_RIGHT2, 0);


  //  Serial.println("cr ARRIVAL = " + String(cr));
  //  Serial.println("cl ARRIVAL = " + String(cl));
  mvt = false;
  noturn = false;

  Serial.println("Arrived with cr " + String(cr));
  Serial.println("Arrived with cl " + String(cl));

  
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
void simpleFwd() {

  digitalWrite(MOTOR_LEFT1,  A);
  digitalWrite(MOTOR_LEFT2,  B);
  digitalWrite(MOTOR_RIGHT1, B);
  digitalWrite(MOTOR_RIGHT2, A);
}
void simpleRvs() {

  digitalWrite(MOTOR_LEFT1,  B);
  digitalWrite(MOTOR_LEFT2,  A);
  digitalWrite(MOTOR_RIGHT1, A);
  digitalWrite(MOTOR_RIGHT2, B);
}



void FollowLineFWD() {
  String state = "Leaving docking station";
  _server.send(200, "text/plain", state);

  fwd(255, 0);
  delay(200);
  STOP();

  flwln = true; // disable cliff sensing (deprecated)
  int last = 0;
  STOPALL = false;
  SPEED = 160;
  int del = 50;
  int test = 0;
  unsigned long GStart = millis();//  set a time limit
  bumped = false;
  noblink = true; // blink led takes millis...

  while (!STOPALL && !BothFrontSeen() && millis() - GStart < 20000 && !bumped) {

    fwd(SPEED, 0);
    Serial.println("G FWD");
    Serial.print("bumped = "); Serial.println(bumped);

    execLineFwd();
    loop();

  }
  STOP();
  bumped = false;
  GotStuck = false;
  noblink = false;

  STOPALL = false;
  flwln = false; // enable cliff sensing

}



void execLineFwd() {
  int del = 50;
  while (SeeLineLeftFront() && !STOPALL && !BothFrontSeen() && !bumped) {
    loop();
    left(200);
    delay(100);
    fwd(SPEED, 0);
    Serial.print("left  "); Serial.println(SeeLineLeftFront());
    Serial.println("SL FWD");
    delay(del);
  }
  while (SeeLineRightFront() && !STOPALL && !BothFrontSeen() && !bumped) {
    loop();
    right(200);
    delay(100);
    fwd(SPEED, 0);
    Serial.print("right "); Serial.println(SeeLineRightFront());
    Serial.println("SR FWD");
    delay(del);
  }
}
