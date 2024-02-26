void fwd(int v, int dist) {


  MvtState = "Forward";

  noturn = true; // tells speed adjustments this is not a turn so ok to adjust
  mvt = true;

  // dist is to be requested in cm
  // term.println("FORWARD");
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
    term.println("STARTING makedist()");
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

void rvs(int v, int dist) {
  noturn = true; // tells speed adjustments this is not a turn so ok to adjust
  mvt = true;

  MvtState = "Reverse";

  // term.println("REVERSE");

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

    //  term.println("REVERSE");

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

  published = false;
  noturn = false; // tells speed adjustments this is a turn so NOT ok to adjust

  if (digitalRead(CHARGERelay) == 0) {
    stopcharge();
  }
  mvt = true;

  v = v < 170 ? 170 : v;

  if (!flushing) {
    ledcWrite(pwmChannelL, v);
    ledcWrite(pwmChannelR, v);
    digitalWrite(MOTOR_LEFT1,  B);
    digitalWrite(MOTOR_LEFT2,  A);
    digitalWrite(MOTOR_RIGHT1, B);
    digitalWrite(MOTOR_RIGHT2, A);
  }
  else {
    Serial.println("FLUSHING BUFFER.......NO MVT...");
  }

  //term.println("LEFT");

  MvtStateMillis = millis();
}

void left_step(){
  left(200);
  delay(50);
  STOP();
}

void right(int v) {

  //term.println("moving right" );

  MvtState = "Right";

  published = false;
  noturn = false; // tels speed adjustments this is a turn so NOT ok to adjust

  if (digitalRead(CHARGERelay) == 0) {
    stopcharge();
  }
  mvt = true;

  v = v < 170 ? 170 : v ;

  if (!flushing) {
    ledcWrite(pwmChannelL, v);
    ledcWrite(pwmChannelR, v);
    digitalWrite(MOTOR_LEFT1,  A);
    digitalWrite(MOTOR_LEFT2,  B);
    digitalWrite(MOTOR_RIGHT1, A);
    digitalWrite(MOTOR_RIGHT2, B);
  }
  else {
    Serial.println("FLUSHING BUFFER.......NO MVT...");
  }

  //term.println("RIGHT");

  MvtStateMillis = millis();
}

void right_step(){
  right(200);
  delay(50);
  STOP();
}

void simpleLeft(int v) {

  //  MvtState = "Left"; // we don't have auto stop on this one
  currentMove = "Left"; // but we still need to know what's going on
  published = false;
  noturn = false; // tells speed adjustments this is a turn so NOT ok to adjust

  mvt = true;

  // otherwise, keep requested speed value as it is asked by the http request

  if (!flushing) {
    ledcWrite(pwmChannelL, v);
    ledcWrite(pwmChannelR, v);
    digitalWrite(MOTOR_LEFT1,  B);
    digitalWrite(MOTOR_LEFT2,  A);
    digitalWrite(MOTOR_RIGHT1, B);
    digitalWrite(MOTOR_RIGHT2, A);
  }
  else {
    Serial.println("FLUSHING BUFFER.......NO MVT...");
  }

  MvtStateMillis = millis();
}

void simpleRight(int v) {

  //term.println("moving right" );

  //  MvtState = "Right";// we don't have auto stop on this one
  currentMove = "Right"; // but we still need to know what's going on

  published = false;
  noturn = false; // tels speed adjustments this is a turn so NOT ok to adjust

  mvt = true;

  if (!flushing) {
    ledcWrite(pwmChannelL, v);
    ledcWrite(pwmChannelR, v);
    digitalWrite(MOTOR_LEFT1,  A);
    digitalWrite(MOTOR_LEFT2,  B);
    digitalWrite(MOTOR_RIGHT1, A);
    digitalWrite(MOTOR_RIGHT2, B);
  }
  else {
    Serial.println("FLUSHING BUFFER.......NO MVT...");
  }

  MvtStateMillis = millis();
}

void STOP() {


  //term.println("STOP!");
  MvtState = "STOP";
  currentMove = "STOP";
  ledcWrite(pwmChannelL, 0);
  ledcWrite(pwmChannelR, 0);
  digitalWrite(MOTOR_LEFT1,  0);
  digitalWrite(MOTOR_LEFT2,  0);
  digitalWrite(MOTOR_RIGHT1, 0);
  digitalWrite(MOTOR_RIGHT2, 0);


  //  term.println("cr ARRIVAL = " + String(cr));
  //  term.println("cl ARRIVAL = " + String(cl));
  mvt = false;
  noturn = false;

  //  term.println("Arrived with cr " + String(cr));
  //  term.println("Arrived with cl " + String(cl));


  cr = 0;
  cl = 0;

  dutyCycle = Default;


  //term.println("----------------STOP--------------------");

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

  armUp();
  fwd(255, 0);
  delay(200);
  STOP();

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
    //term.println("G FWD");
    //term.print("bumped = "); term.println(bumped);

    execLineFwd();
    MainFunctions();

  }
  STOP();
  bumped = false;
  GotStuck = false;
  noblink = false;

  STOPALL = false;

}



void execLineFwd() {
  int del = 50;
  while (SeeLineLeftFront() && !STOPALL && !BothFrontSeen() && !bumped) {
    MainFunctions();
    left(200);
    delay(100);
    fwd(SPEED, 0);
    term.print("left  "); term.println(SeeLineLeftFront());
    term.println("SL FWD");
    delay(del);
  }
  while (SeeLineRightFront() && !STOPALL && !BothFrontSeen() && !bumped) {
    MainFunctions();
    right(200);
    delay(100);
    fwd(SPEED, 0);
    term.print("right "); term.println(SeeLineRightFront());
    term.println("SR FWD");
    delay(del);
  }
}
