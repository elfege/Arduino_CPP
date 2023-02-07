


///////////////// SENSORS TESTS ///// OBSTACLE MANAGEMENT
boolean OBS() {
  bool obstacle = frontIR() || BUMP() || fusup() ;

  if (obstacle) {
    term.println("OBSTACLE detected!");
    return true;
  }
  else {
    return false;
  }
}

boolean BUMP() {
  bool result = BUMPR() || BUMPL();
  if (result) {
    bumped = true;
    //term.println("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
  }
  return result;
}


boolean frontIR() {
  if (frontIR_RVal() > 500 || frontIR_LVal() > 500) {
    term.println("Front IR obs detected!-------------------------------------FIR-------------");
    return true;
  }
  else {
    return false;
  }
}


boolean fusup() {
  //
  //  term.println("");
  //  term.print("fusupVal at bool = "); term.println(fusupVAL());
  //  term.println("");
  int val = fusupVAL();
  if (val < 50) {
    //    delay(10);
    //    val = fusupVAL();
    //    if (val < 30) {
    term.println("fusup obs detected!=============================================== VALUE = " +String(val));
    return true;
    //    } else {
    //      term.println("FALSE POSITIVE at fusup");
    //      return false;
    //    }
  }
  else {
    return false;
  }

}


boolean BUMPR() {
  cmd = "+BUMP_R-";
  int t = 0;
  String received = "";

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);

  while (!Serial2.available())
  {
    ;; //MainFunctions();
  }
  received = UartReceive(cmd);

  if (received == "+1-") {
    return true;
    bumpedR = true;
  }
  else if (received == "+0-") {
    return false;
  }
}
boolean BUMPL() {
  cmd = "+BUMP_L-";
  int t = 0;
  String received = "";

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);


  while (!Serial2.available())
  {
    ;; //MainFunctions();
  }
  received = UartReceive(cmd);
  //String received = UartReceive(cmd);
  //term.print("received bump val: ");term.println(received);

  if (received == "+1-") {
    return true;
    bumpedL = true;
  }
  else if (received == "+0-") {
    return false;
  }
}

boolean RearLeftIR() {
  cmd = "+RLIR-";
  int t = 0;
  String received = "";

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);


  while (!Serial2.available())
  {
    ;; //MainFunctions();
  }
  received = UartReceive(cmd);
  //String received = UartReceive(cmd);
  //term.print("received val: ");term.println(received);

  if (received == "+1-") {
    return true;
  }
  else if (received == "+0-") {
    return false;
  }
}

boolean RearRightIR() {
  cmd = "+RRIR-";
  int t = 0;
  String received = "";

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);

  //
  while (!Serial2.available())
  {
    ;; //MainFunctions();
  }
  received = UartReceive(cmd);
  //String received = UartReceive(cmd);
  //term.print("received val: ");term.println(received);

  if (received == "+1-") {
    return true;
  }
  else if (received == "+0-") {
    return false;
  }
}

boolean rearOBS() {
  return RRIRVal() > 500 || RLIRVal() > 500;
}

boolean rightOBS() {
  return rightIRVal() > IRlimit;
}

boolean leftOBS() {
  return leftIRVal() > IRlimit;
}

boolean sideOBS() {
  return leftOBS() || rightOBS();
}

boolean RearFrontOK() {
  cmd = "+RearFrontOK-";
  int t = 0;
  String received = "";

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);

  //
  while (!Serial2.available())
  {
    ;; //MainFunctions();
  }
  received = UartReceive(cmd);

  if (received == "+1-") {
    return true;
  }
  else if (received == "+0-") {
    return false;
  }
}

boolean AllObs() {
// returns true if obstacle
  if (fusupVAL() < 30 && frontIR_RVal() > 500 && frontIR_LVal() > 500 && fusLVAL() < 5 && fusRVAL() < 5 && leftIRVal() > 300 && rightIRVal() > 400 && GotStuck && !rearOBS())
  {
    return true;
  }
  else
  {
    return false;
  }
}
