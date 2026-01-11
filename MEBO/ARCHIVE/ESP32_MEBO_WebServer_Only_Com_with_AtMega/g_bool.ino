
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
  //  bool result = analogRead(LIR) < 500 && analogRead(RIR) < 500 && analogRead(RRIR) < 500 && analogRead(RLIR) < 500 && analogRead(FIR) < 500;
  //  return result;
}

///////////////// SENSORS TESTS ///// OBSTACLE MANAGEMENT
boolean OBS() {


  obstacle = frontIR() || BUMP() || fusup() ;

  //  Serial.print( " obstacle ? " );Serial.println( obstacle );
  //  Serial.print( " FrontIR ? " );Serial.println( FrontIR );
  //  Serial.print( " FrontIR VALUE----------------------------- ? " );Serial.println( analogRead(FIR) );
  //  Serial.print( " bump ? " );Serial.println( bump );
  //  Serial.print( " FUSUP ? " );Serial.println( FUSUP );
  //  Serial.print( " distance ? " );Serial.println( distance );
  //  Serial.print( "Main Loop on CPU # " );Serial.println( xPortGetCoreID()  );


  return obstacle;
}

boolean BUMP() {
  cmd = "sBUMPe";
  int t = 0;
  String received = "";

  UartSend(cmd);

  while (!Serial2.available())
  {
    delay(1);
  }
  received = UartReceive();
  //String received = UartReceive();
  //Serial.print("received bump val: ");Serial.println(received);

  if (received == "sYAYe") {
    return true;
  }
  else if (received == "sNAYe") {
    return false;
  }
}

boolean frontIR() {
  cmd = "sfrontIRe";
  int t = 0;
  String received = "";

  UartSend(cmd);

  while (!Serial2.available())
  {
    delay(1);
  }
  received = UartReceive();
  //String received = UartReceive();
  //Serial.print("received val: ");Serial.println(received);

  if (received == "sYAYe") {
    return true;
  }
  else if (received == "sNAYe") {
    return false;
  }
}

boolean fusup() {
  cmd = "sfusupe";
  int t = 0;
  String received = "";

  UartSend(cmd);

  Start = millis();
  while (!Serial2.available() && millis() - Start < 100)
  {
    delay(1);
  }
  received = UartReceive();
  //String received = UartReceive();
  //Serial.print("received val: ");Serial.println(received);

  if (received == "sYAYe") {
    return true;
  }
  else if (received == "sNAYe") {
    return false;
  }
}

int fusupVAL() {
  cmd = "sfusupVALe";
  int t = 0;
  String received;

  UartSend(cmd);
  Start = millis();
  while (!Serial2.available() && millis() - Start < 1000)
  {
    delay(1);
  }

  received = UartReceive();
  //Serial.print("received before removal = "); Serial.println(received);
  received.remove(0 , 1); // remove 1 char at index 0
  received.remove(received.length() - 1 , 1); // remove 1 char at the end
  //Serial.print("received after removal = "); Serial.println(received);
  int result = received.toInt();
  //Serial.print("received to Int = "); Serial.println(received);

  return result;
}

boolean RearLeftIR() {
  cmd = "fusup";
  UartSend(cmd);
  while (!Serial2.available())
  {
    delay(1);
  }
  bool result = UartReceive().toInt();
}

boolean RearRightIR() {
  //  // LIR less than 400 means long distance, means cliff
  //  boolean rrir = analogRead(RRIR) >= 1800 ;
  //  //  Serial.print( "RLIR = " );Serial.println( digitalRead(RLIR) );
  //  //  Serial.print( "RRIR as obs sensor = " );Serial.println( analogRead(RRIR) );
  //  return rrir;
}

boolean rearOBSClose() {
  //  bool result = analogRead(RRIR) > 2000 || analogRead(RLIR) > 2000;
  //  return result;
}

boolean rearOBS() {
  //  bool result = RearLeftIR() || RearRightIR();
  //  return result;
}

boolean rightOBS() {
  //  bool result = analogRead(RIR) > 1800;
  //  return result;
}
boolean leftOBS() {
  //  bool result = analogRead(LIR) > 1800;
  //  return result;
}
boolean sideOBS() {
  //  bool result = leftOBS() && rightOBS();
  //  return result;
}

boolean SeeLineLeft() {
  //  return digitalRead(LineLeft);
}
boolean SeeLineRight() {
  //  return digitalRead(LineRight);
}

boolean OneSeesTheLine() {
  //  bool result = SeeLineLeft() || SeeLineRight();
  //  return result;
}

boolean OneSeesTheLineFront() {
  //  bool result = SeeLineLeft() || SeeLineRight() ;
  //  return result;
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

boolean RearFrontOK() {
  //  boolean a = analogRead(RRIR) <= 200 &&  analogRead(RLIR) <= 500 &&  analogRead(FIR) <= 500;
  //  return a;
}

boolean StationInLine() {
  //delay(50);
  if (irrecv.decode(&results)) {
    Signal = true;
    Serial.println(results.value, HEX);

    if (results.value == 0xa90) {
      IRconfirm++;
      //Serial.println ( IRconfirm );
      if (IRconfirm >= 10) {
        Serial.println("DOCK STATION IN VIEW!!!" );
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
  //  else {
  //    found = false;
  //  }
  return found;
}


boolean haspower() {
  //bool result = analogRead(PW) >= 2000;

  // return result;
}

