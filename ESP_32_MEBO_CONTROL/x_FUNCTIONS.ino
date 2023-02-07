#include <math.h>

void makedist(float dist) {
  Serial << "requested distance in cm = " << dist << endl;
  float ratio = 10.00 / 8.00; // 10 bits = 8 cm
  Serial << "ratio is = " << ratio << endl;

  float convertedDist = ratio * dist;

  Serial << "requested distance in bits = " << convertedDist << endl;

  int limit = cl + convertedDist; // set the bitdistance as a limit

  Serial.print("cl start:  "); Serial.println(cl);
  Serial.print("cr start:  "); Serial.println(cr);

  delay(1); // necessary to allow wheels to run and change of state

  called = true; // calling for interrupt counter
  requireHeuristics = true; // allows GotStuck global variable to refresh

  while (cr < limit && cl < limit && !STOPALL && !GotStuck) { // && (!OBS() && !rearOBS()) && !STOPALL) {


    loop();

  }
  GotStuck = false;

  STOP();
  Serial << "cr ARRIVAL = " << cr  << endl;
  Serial << "cl ARRIVAL = " << cl  << endl;
  //  cl = 0;
  //  cr = 0;
  called = false;

}

////////////////// MOVEMENT
void fwd(int v, int dist) {

  docking = false;

  // dist is to be requested in cm
  // Serial.println("FORWARD");
  published = false;
  reqSpeed = v; // tels speed adjustments where to start from
  dutyCycleR = reqSpeed;
  dutyCycleL = reqSpeed;
  mapSpeed(); // for encoders

  noturn = true; // tels speed adjustments this is not a turn so ok to adjust

  if (digitalRead(RELAY1) != 1) {
    poweron();
  }
  if (digitalRead(RELAY2) == 0) {
    stopcharge();
  }
  mvt = true;
  ledcWrite(pwmChannelL, v);
  ledcWrite(pwmChannelR, v);
  digitalWrite(PWM_LEFT1,  A);
  digitalWrite(PWM_LEFT2,  B);
  digitalWrite(PWM_RIGHT1, B);
  digitalWrite(PWM_RIGHT2, A);

  if (dist != 0) {

    makedist(dist);

  }
  //  else {
  //    //Serial.println("No requested distance");
  //  }
}

void rvs(int v, int dist) {


  published = false;
  reqSpeed = v; // tels speed adjustments where to start from
  dutyCycleR = reqSpeed;
  dutyCycleL = reqSpeed;
  mapSpeed(); // for encoders

  noturn = true; // tels speed adjustments this is not a turn so ok to adjust
  if (digitalRead(RELAY1) != 1) {
    poweron();
  }
  if (digitalRead(RELAY2) == 0) {
    stopcharge();
  }
  //  Serial.println("REVERSE");
  mvt = true;
  ledcWrite(pwmChannelL, v);
  ledcWrite(pwmChannelR, v);
  digitalWrite(PWM_LEFT1,  B);
  digitalWrite(PWM_LEFT2,  A);
  digitalWrite(PWM_RIGHT1, A);
  digitalWrite(PWM_RIGHT2, B);


  if (dist != 0) {
    makedist(dist);
  }
  //  else {
  //   // Serial.println("No requested distance");
  //  }

}

void left(int v) {
  published = false;
  noturn = false; // tels speed adjustments this is a turn so NOT ok to adjust
  if (digitalRead(RELAY1) != 1) {
    poweron();
  }
  if (digitalRead(RELAY2) == 0) {
    stopcharge();
  }
  mvt = true;
  ledcWrite(pwmChannelL, v);
  ledcWrite(pwmChannelR, v);
  digitalWrite(PWM_LEFT1,  B);
  digitalWrite(PWM_LEFT2,  A);
  digitalWrite(PWM_RIGHT1, B);
  digitalWrite(PWM_RIGHT2, A);

  //Serial.println("LEFT");
}

void right(int v) {
  published = false;
  noturn = false; // tels speed adjustments this is a turn so NOT ok to adjust
  if (digitalRead(RELAY1) != 1) {
    poweron();
  }
  if (digitalRead(RELAY2) == 0) {
    stopcharge();
  }
  mvt = true;
  ledcWrite(pwmChannelL, v);
  ledcWrite(pwmChannelR, v);
  digitalWrite(PWM_LEFT1,  A);
  digitalWrite(PWM_LEFT2,  B);
  digitalWrite(PWM_RIGHT1, A);
  digitalWrite(PWM_RIGHT2, B);
  delay(1);

  //Serial.println("RIGHT");
}

void STOP() {

  ledcWrite(pwmChannelL, 0);
  ledcWrite(pwmChannelR, 0);
  digitalWrite(PWM_LEFT1,  0);
  digitalWrite(PWM_LEFT2,  0);
  digitalWrite(PWM_RIGHT1, 0);
  digitalWrite(PWM_RIGHT2, 0);
  mvt = false;



  error = false;
  dutyCycle = Default;

}

void poweron() {
  Serial.println("POWER ON");
  digitalWrite(RELAY1, 1);
  String var = "MEBO camon";
  st::Everything::sendSmartString(var);
}
void poweroff() {

  Serial.println("POWER OFF");
  digitalWrite(RELAY1, 0);
  String var = "MEBO camoff";
  st::Everything::sendSmartString(var);
}

void selfdrive() {
  ignoreobs = true;
  STOPALL = false;
  boolean rvsdone = false;

  if (digitalRead(RELAY1) != 1) {
    poweron();
  }

  while (!STOPALL) {
    loop();// need main loop to be called to allow stop
    Serial << "step 1" << endl;
    fwd(SLOW, 0);

    delay(200);
    Serial << "step 2" << endl;
    if (stuck()) {
      unstuck();
    }

    while (!OBS() && !STOPALL) {
      loop(); // need this loop to be called to allow stop
    }
    Serial << "step 4" << endl;

    int occL = 0; // left turn occurences counter
    int occR = 0; // right turn occurences counter
    int maxocc = random(2, 5); // randomized maximum turn occurences
    if (OBS() && !rearOBS() && !STOPALL) {
      // scan for a free way out
      Start = millis();// but set a time limit
      while (OBS() && !STOPALL && millis() - Start < 20000) {
        loop(); // need this loop to be called to allow stop
        if (!rvsdone) {
          // retract once before spinning / scanning
          loop(); // need this loop to be called to allow stop
          rvs(MEDIUM, 0);// rvs once
          delay(400);
          STOP(); // pause to let sensors work
          delay(200);
          // check there's no obstacle in the back
          if (rearOBS() && !STOPALL) {
            fwd(FAST, 0);
            delay(200);
            STOP();
          }
          // make sure to not repeat this action
          rvsdone = true;
        }
        // Now that we're out of trouble, turn and scan for freeway
        if (occL < maxocc) {
          unsigned long StL = millis();
          // look for a true clear path
          while (!stuck() && analogRead(FIR) > 600 && !STOPALL && millis() - StL < 3000) {
            loop();
            left(FAST);
            delay(50);
            STOP();
          }
          occL++; // number of times it turned
        }
        else if (occR < maxocc) {
          unsigned long StR = millis();
          // look for a true clear path
          while (analogRead(FIR) > 600 && !STOPALL && millis() - StR < 3000) {
            loop();
            right(FAST);
            delay(50);
            STOP();

          }
          occR++;
        }
        else { /// rare case, probably stuck somewhere
          if (stuck()) {
            unstuck();
          }
          else {
            occR = 0;
            occL = 0;
            rvs(255, 0);
            delay(400);
            STOP();
            //STOPALL = true; // cancel all selfdrive operations
            break;
          }
        }
        delay(200);
        STOP(); // pause to let sensors work
        OBS(); // call for sensors scan
        delay(200);
        if (rearOBS() && !STOPALL) {
          fwd(FAST, 0);
          delay(200);
          STOP();
        }
      }
      rvsdone = false; // reset this boolean for next time
    }

    if (fusup() && !STOPALL) {
      rvs(255, 0);
      Start = millis();
      while (OBS() && !STOPALL && millis() - Start < 500) {
        loop();
      }
    }

    if (STOPALL) {
      STOP();
      break;
    }
    // if rearOBS() continue to go fwd
  }
}

void unstuck() {
  Serial.println("UNSTUCKING");

  DELAY = random(50, 500);
  SPEED = random(180, 255);
  loop(); // need to call loop for override / STOPALL / reset purpose

  fwd(SPEED, 0);
  delay(DELAY);

  left(FAST);
  delay(DELAY);

  rvs(SPEED, 0);
  delay(DELAY);

  right(FAST);
  delay(DELAY);

  rvs(SPEED, 0);
  delay(DELAY);

  right(FAST);
  delay(DELAY);

  fwd(SPEED, 0);
  delay(DELAY);

}
boolean stuck() {
  bool result = false;

  int thres = 10; // map(reqSpeed, SLOW, FAST, 200, 70);
  int Delay = 10; // map(reqSpeed, SLOW, FAST, 50, 15);

  ReadL = cl;
  ReadR = cr; // record values
  //  Serial << "cl BEFORE -- " << cl << endl;
  //  Serial << "cr BEFORE -- " << cr << endl;

  if (mvt && noturn) {

    unsigned long GStart = millis();

    while (millis() - GStart < Delay) {   // NOTE mvt boolean updated by encoder management in the workload tab
      ;;// wait a couple millisecs while cl and cr may still increment, or not
    }

    //    Serial << "cl AFTER  -- " << cl << endl;
    //    Serial << "cr AFTER  -- " << cr << endl;
    //    Serial << "positive val  -- " << positive << endl;

    // // now compare values and see if there was substantial change, if not then stuck is true

    if (ReadL == cl || ReadR == cr ) {
      positive++; // run a couple more tests to confirm
      //Serial << "EQUAL  ============= " << positive << " ======================= " << endl;
      // if one of them shows identical values
      if (positive == 1) { // record read values the first time it happened
        secReadR = ReadR;
        secReadL = ReadL;
      }

      if (positive >= thres && (cr == secReadR || cl == secReadL)) {
        // if you had equal values for "thres" times in a row
        // AND if these values haven't changed since the first time you noticed that, then definitely return true
        positive = 0; // reset this value
        Serial << "-----------------STUCK-------------------------" << endl;
        GotStuck = true;
        result = true;
      }
    }
    else {
      //Serial << "NOT -- stuck" << endl;
      positive = 0; // reset this value to cancel possible false alarm and allow further testing
      result = false;
    }
  }
  else {
    //Serial << "NOT stuck" << endl;
    positive = 0; // reset this value to cancel possible false alarm and allow further testing
    result = false;
  }

  if (positive > thres + 1) {
    secReadR = 0;
    secReadL = 0;
    positive = 0;
  }
  return result;
}


///////////////// SENSORS TESTS ///// OBSTACLE MANAGEMENT
boolean OBS() {


  obstacle = frontIR() || BUMP() || fusup() ;

  //  Serial << " obstacle ? " << obstacle << endl;
  //  Serial << " FrontIR ? " << FrontIR << endl;
  //  Serial << " FrontIR VALUE----------------------------- ? " << analogRead(FIR) << endl;
  //  Serial << " bump ? " << bump << endl;
  //  Serial << " FUSUP ? " << FUSUP << endl;
  //  Serial << " distance ? " << distance << endl;
  //  Serial << "Main Loop on CPU # " << xPortGetCoreID()  << endl;


  return obstacle;
}

boolean BUMP() {
  // digital as booleans
  bump = digitalRead(bumberPin);
  return bump;
}
boolean frontIR() {
  // analog direct to bool
  FrontIR = analogRead(FIR) > 2000;
  return FrontIR;
}
boolean fusup() {

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2;

  // Prints the distance on the Serial Monitor
  //   Serial.print("Distance: ");Serial.println(distance);

  FUSUP = distance < 100;
  return FUSUP;
}
boolean RearLeftIR() {
  // RRIR less than 400 means long distance
  boolean d = analogRead(RLIR) >= 2000;
  //  Serial << "RLIR = " << digitalRead(RLIR) << endl;
  //  Serial << "RRIR as obs sensor = " << analogRead(RRIR) << endl;
  return d;
}
boolean RearRightIR() {
  // LIR less than 400 means long distance, means cliff
  boolean rrir = analogRead(RRIR) >= 2000 ;
  //  Serial << "RLIR = " << digitalRead(RLIR) << endl;
  //  Serial << "RRIR as obs sensor = " << analogRead(RRIR) << endl;
  return rrir;
}

boolean rearOBS() {
  bool result = RearLeftIR() || RearRightIR();
  return result;
}


void charge() {
  // battery charging relay
  digitalWrite(RELAY2, 0);
}
void stopcharge() {
  // battery charging relay
  digitalWrite(RELAY2, 1);
}

boolean haspower() {
  bool result = analogRead(PW) >= 2000;

  return result;
}
void dock() {
  docked = false;
  ignoreobs = true;
  docking = true;// allows ignoring obstacle management in webserver()
  int attempts = 0;
  stopcharge();
  delay(500);
  if (!haspower()) {
    // first, try to position toward station
    SearchStation();

    //go to station
    FollowLineRVS();
  }
  else if (haspower()) {
    STOP();
    docked = true; // tell the system that we're docked
    poweroffDONE = false;
    timeCountToPwOff = millis(); // start counter to poweroff
    charge();
    String var = "MEBO docked";
    st::Everything::sendSmartString(var); // will trigger arm retraction and complete shutdown
  }
  else {
    Serial.println("FAILED TO DOCK");
    STOP();
  }

  // reset stopall
  STOPALL = false;
  docking = false;
  Serial.println("END OF DOCKING LOOP");
}



void SearchStation() {
  requireHeuristics = true;

  found = false; // when found the station
  int i = 0;
  int j = 1;
  int ranDom;
  STOPALL = false;
  
  
  //find the IR signal of the station

  Serial.println("SEARCHING FOR STATION");
  unsigned long GStart = millis();
  while (!STOPALL && !found && millis() - GStart < 20000) {
    loop();

    Start = millis(); // turn left for n seconds max

    while (j == 1 && !STOPALL && !found && millis() - Start < 800) {
      i = 0; // tells last move
      j = 0; // allows next loop
      right(180);
      delay(25);
      STOP();
      delay(50);
      loop();
    }
    Start = millis(); // turn left for 2 seconds max

    while (j == 0 && !STOPALL && !found && millis() - Start < 800) {
      i = 1; // tells last move
      j = 1; // allows next loop
      left(180);
      delay(25);
      STOP();
      delay(50);
      loop();
    }
    if(!found){
      if(i == 0){
        // last was left, next should go to the right 
        j = 1;
      }
      else if(i == 1){
        // last was right, next should go to the left 
        j = 0;
      }
    }
  }
  STOP();
  found = false; // reset this value
}

boolean StationInView() {

  if (irrecv.decode(&results)) {
    someIR = true;
    //Serial.println(results.value, HEX);

    if (results.value == 0xa90) {
      IRconfirm++;
      //Serial << IRconfirm << endl;
      if (IRconfirm > 10) {
        //Serial << "DOCK STATION IN VIEW!!!" << endl;
        //results.value = 0; // reset the value
        found = true;
        IRconfirm = 0; // allow next cycle to reconfirm
      }
    }
    else {
      //Serial << " someIR " << endl;
      IRconfirm = 0;// restart confirmation process because signal was not consistent over 10 attempts
    }
    irrecv.resume(); // Receive the next value
  }
  else {
    someIR = false;
  }
  return found;
}


void FollowLineFWD() {

  int last = 0;
  STOPALL = false;
  SPEED = 170;
  int del = 50;
  int test = 0;
  unsigned long GStart = millis();//  set a time limit

  while (!STOPALL && !BothFrontSeen() && millis() - GStart < 20000) {
    loop();

    //    Serial << " SeeLineRearLeft ? " << SeeLineRearLeft() << endl;
    //    Serial << " SeeLineRearRight ? " << SeeLineRearRight() << endl;
    //    Serial << " SeeLineLeft() ? " << SeeLineLeft() << endl;
    //    Serial << " SeeLineRight() ? " << SeeLineRight() << endl;

    //Start = millis();//&& !STOPALL && millis() - Start < 5) {
    fwd(SPEED, 0);
    //noturn = false;

    while (!OneSeesTheLineFront() && !STOPALL && !BothFrontSeen()) {
      loop();
    }
    while (SeeLineLeft() && !STOPALL && !BothFrontSeen()) {
      loop();
      left(MEDIUM);
      delay(del);
      fwd(SPEED, 0);
      delay(del);
    }
    while (SeeLineRight() && !STOPALL && !BothFrontSeen()) {
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
}

void FollowLineRVS() {
  requireHeuristics = true;

  String var = "retractarms";
  Serial.println(var);
  st::Everything::sendSmartString(var); // will trigger arm retraction / needs to be done to prevent false OBS() positive

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
  int MinV4Dist = 200; // min dist val for when robot is still far relatively to rear obstacle

  boolean hasbeenfound = false;
  boolean keepfixedspeed = false;
  GotStuck = false;
  unsigned long timer = 160000;

  delay(500);

  int i = MdSp;
  //rvs(MdSp, 0);
  //noturn = false; // disable encoders to prevent overloads

  unsigned long GStart = millis();//  set a time limit


  while (!STOPALL && !haspower() && millis() - GStart < timer) {
    loop();
    //Serial.print("New SPEED = "); Serial.println(MdSp);
    Serial << " ELAPSED TIME = " << (millis() - GStart) / 1000 << "SECONDS OUT OF " << timer / 1000 << endl;

    if (!keepfixedspeed) { // if this is not a correction attempt, adjust speed with distance
      MdSp = MatchD2S(MaxV4Dist, MinV4Dist, 150, 180);
    }
    else {
      MdSp = 180;
    }
    // otherwise, push harder
    rvs(MdSp, 0);
    loop();
    found = false; // called in loop()
    someIR = false; // called in loop()
    loop(); // refreshes "found" and "someIR" global booleans

    if (!hasbeenfound && millis() - GStart > 2000) {
      SearchStation();
      if (found) {
        // don't do it again
        hasbeenfound = true;
      }
    }

    if (GotStuck && millis() - GStart > 10000) { // stuck() is called in loop()
      GotStuck = false; // reset this value

      Serial << " A.I. CORRECTION " << endl;
      // now you will reverse with line but push harder
      keepfixedspeed = true; // allow aligning with enough wheel power (no pun intended...)
      // as of now speed will be 200 every step of the way until time limit is reached

      followFWD(del, MdSp); // realign fwd

      GotStuck = false; // reset this global boolean
      someIR = false;// reset this global boolean
    }

    GotStuck = false; // reset this value

    if (OneSeesTheLineRear()) {
      followRvs(del, MdSp);
    }
    loop();
  }


  STOP();

  delay(1000);
  if (haspower() && !docking) {

    charge();
    docked = true; // Set robot as docked
    poweroffDONE = false;
    timeCountToPwOff = millis(); // start counter to poweroff
    var = "MEBO docked";
    Serial.println(var);
    st::Everything::sendSmartString(var); // will trigger arm retraction and complete shutdown (shutdown disabled at the moment in ST device handler)
  }
  else {
    docked = false;
    Serial.println("FAILED TO DOCK");
  }
  STOP();
  Serial.println("END OF FOLLOW LINE RVS");
  STOPALL = false;
}

int followRvs(int del, int SPEED) {


  while (SeeLineRearRight()  && !STOPALL && !haspower()) {
    loop();
    //    if (GotStuck) {
    //      right(200);
    //      delay(50);
    //    }
    GotStuck = false;
    left(200);
    delay(del);
    rvs(180, 0);
    delay(20);
  }
  while (SeeLineRearLeft() && !STOPALL && !haspower()) {
    loop();
    //    if (GotStuck) {
    //      left(200);
    //      delay(50);
    //    }
    GotStuck = false;
    right(200);
    delay(del);
    rvs(180, 0);
    delay(20);
  }

}

int followFWD(int del, int SPEED) {
  fwd(SPEED, 0);
  //noturn = false;
  GotStuck = false;
  while (!OneSeesTheLineFront() && !STOPALL && !BUMP()) {
    loop();
  }
  while (SeeLineLeft() && !STOPALL && !BUMP()) {
    loop();
    left(MEDIUM);
    delay(del);
    fwd(SPEED, 0);
    delay(del);
  }
  while (SeeLineRight() && !STOPALL && !BUMP()) {
    loop();
    right(MEDIUM);
    delay(del);
    fwd(SPEED, 0);
    delay(del);
  }
  GotStuck = false;
}


int MatchD2S(int Close, int far, int slow, int fast) {
  int  d = analogRead(RLIR);
  int Map = map(d, Close, far, slow, fast); // the closer, the slower

  // round to the closest five (183 --> 185)
  Map = round10(Map);

  if (Map > 200) {
    Map = 200;
  }

  delay(10);
  return Map;

}

int round10(int n) {
  int result = (n / 10 + (n % 10 > 2)) * 10;

  return result;
}

void CaptureTheLineRVS() {
  //////////////////////////// NOT TESTED YET...
  int last = 0;
  STOPALL = false;
  stopcharge();
  int del = 50;
  found = false;
  SPEED = 170;
  delay(500);

  // find the line within X seconds
  unsigned long GStart = millis();//  set a time limit
  while (!STOPALL && millis() - GStart < 30000) {

    rvs(160, 0);
    while (!OneSeesTheLineRear() && !STOPALL && millis() - Start < 5000) {
      // slowly come backward until a line is seen
      loop();
      if (OneSeesTheLineRear()) {
        // confirm
        delay(50);
      }
    }


    while (!found && !STOPALL) {
      // zig-zag backward and try to keep the station in view
      left(200);
      StationInView();
      delay(100);
      if (found) {
        break;
      }
      delay(200);
      right(200);
      delay(200);
      StationInView();
      if (found) {
        break;
      }
      else {
        rvs(SPEED, 0);
      }
      delay(200);
    }
    StationInView();
    delay(200);


    if (OneSeesTheLineRear()) {
      while (SeeLineRearRight() && !STOPALL && !haspower()) {
        loop();
        right(MEDIUM);
        delay(del);
        rvs(SPEED, 0);
        delay(del);
      }
      //Serial.println("RIGHT");
      while (SeeLineRearLeft() && !STOPALL && !haspower()) {
        loop();
        left(MEDIUM);
        delay(del);
        rvs(SPEED, 0);
        delay(del);
      }
    }
    if (!BothRearSeen()) {
      // if both rear sensors are now outside the line...
      break;
      // after this, should run the followlinervs()...
    }

  }

  STOP();


  Serial.println("END OF CATCH THE LINE");
  STOPALL = false;


}

void RestartDock() {

  STOP();
  STOPALL = true;
  Serial.println("//////////////////////////////////////////// FAILURE ////////////////////////////////////////////");

}

void mapSpeed() {
  // calculate best adjustment margin for the required speed
  //the higher the speed, the lower the speed alteration margin must be:
  N = map(reqSpeed, 180, 230, 20, 0); // at 255 you cant add speed, so make sure no adjustment happens beyond a max safe speed of 230


}

void refresh() {

}


void checkdock() {

}

//// STANDARD FUNCTIONS
void Reset() {
  //Blink(5, 500);
  ESP.restart();
}

boolean SeeLineRearRight() {
  return digitalRead(LineRearRight);
}
boolean SeeLineRearLeft() {
  return digitalRead(LineRearLeft);
}
boolean SeeLineLeft() {
  return digitalRead(LineLeft);
}
boolean SeeLineRight() {
  return digitalRead(LineRight);
}

boolean OneSeesTheLine() {
  bool result = SeeLineLeft() || SeeLineRight() || SeeLineRearRight() || SeeLineRearLeft();
  return result;
}

boolean OneSeesTheLineFront() {
  bool result = SeeLineLeft() || SeeLineRight() ;
  return result;
}

boolean OneSeesTheLineRear() {
  bool result = SeeLineRearRight() || SeeLineRearLeft();
  return result;
}

boolean BothFrontSeen() {
  bool result = SeeLineLeft() && SeeLineRight();

  if (result) {
    // double check
    delay(30);
  }
  result = SeeLineLeft() && SeeLineRight();
  return result;
}

boolean BothRearSeen() {
  bool result = SeeLineRearRight() && SeeLineRearLeft();
  return result;
}



