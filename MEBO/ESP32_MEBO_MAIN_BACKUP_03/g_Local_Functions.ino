boolean stuck() {

  int thres = 30; // map(rqSp, SLOW, FAST, 200, 70);
  int Delay = 10; // map(rqSp, SLOW, FAST, 50, 15);

  ReadL = cl;
  ReadR = cr; // record values
  //  Serial.print( "cl BEFORE -- " );Serial.println( cl );
  //  Serial.print( "cr BEFORE -- " );Serial.println( cr );

  if (mvt && noturn) {

    unsigned long GStart = millis();

    while (millis() - GStart < Delay) {   // NOTE mvt boolean updated by encoder management in the workload tab
      ;;// wait a couple millisecs while cl and cr may still increment, or not
    }

    //    Serial.print( "cl AFTER  -- " );Serial.println( cl );
    //    Serial.print( "cr AFTER  -- " );Serial.println( cr );
    //    Serial.print( "positive val  -- " );Serial.println( positive );

    // // now compare values and see if there was substantial change, if not then stuck is true

    if (ReadL == cl || ReadR == cr ) {
      positive++; // run a couple more tests to confirm
      //Serial.print( "EQUAL  ============= " );Serial.println( positive );Serial.println( " ======================= " );
      // if one of them shows identical values
      if (positive == 1) { // record read values the first time it happened
        secReadR = ReadR;
        secReadL = ReadL;
      }

      if (positive >= thres && (cr == secReadR || cl == secReadL)) {
        // if you had equal values for "thres" times in a row
        // AND if these values haven't changed since the first time you noticed that, then definitely return true
        positive = 0; // reset this value
        Serial.print( "-----------------STUCK-------------------------" );
        GotStuck = true;
        resultStuck = true; // will stay true until further mvt
      }
    }
    else {
      //Serial.println("NOT -- stuck" );
      positive = 0; // reset this value to cancel possible false alarm and allow further testing
      resultStuck = false;
    }
  }
  else {
    //Serial.println("NOT stuck" );
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
  bool result = leftIRVal() < 500 && rightIRVal() < 500 && RRIRVal() < 500 && RLIRVal() < 500 && frontIRVal() < 500;
  return result;
}


boolean SeeLineLeftFront() {
  return digitalRead(LineLeftFront);
}
boolean SeeLineRightFront() {
  return digitalRead(LineRightFront);
}

boolean SeeLineLeftRear() {
  return digitalRead(LineLeftRear);
}
boolean SeeLineRightRear() {
  return digitalRead(LineRightRear);
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
    Serial.println(results.value, HEX);

    if (results.value == 0xa90) {
      IRconfirm++;
      //Serial.println ( IRconfirm );
      if (IRconfirm >= 10) {
        Serial.println("DOCK STATION !!!" );
        //
        found = true;
        IRconfirm = 0; // allow next cycle to reconfirm
      }
    }
    else {
      //Serial.println("  " );
      IRconfirm = 0;// restart confirmation process because signal was not consistent for N attempts
    }
    results.value = 0; // reset the value
    irrecv.resume(); // Receive the next value
  }
    else {
      found = false; // global variable to be reset by functions calling this boolean
    }
  return found;
}



boolean Cliff() {
  bool r = analogRead(CliffSensor) < 1000;
  CliffDetect = r;
  if(r){
    _server.send(200, "text/plain", "Cliff");
  }
  
  return r;
}

boolean thereIsMvt(){
      if(digitalRead(MOTOR_LEFT1) || digitalRead(MOTOR_LEFT2) || digitalRead(MOTOR_RIGHT1) || digitalRead(MOTOR_RIGHT2)){
        return true;
      }
      else{
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
