


///////////////// SENSORS TESTS ///// OBSTACLE MANAGEMENT
boolean OBS() {
  bool obstacle = frontIR() || BUMP() || fusup() ;

  if (obstacle) {
    Serial.println("OBSTACLE detected!");
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
  }
  return result;
}


boolean frontIR() {
  if (frontIRVal() > 600) {
    Serial.println("Front IR obs detected!-------------------------------------FIR-------------");
    return true;
  }
  else {
    return false;
  }
}


boolean fusup() {
  //
  //  Serial.println("");
  //  Serial.print("fusupVal at bool = "); Serial.println(fusupVAL());
  //  Serial.println("");
  int val = fusupVAL();
  if (val < 50) {
    //    delay(10);
    //    val = fusupVAL();
    //    if (val < 30) {
    Serial << "fusup obs detected!=============================================== VALUE = " << val << endl;
    return true;
    //    } else {
    //      Serial.println("FALSE POSITIVE at fusup");
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
    ;; //loop();
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
    ;; //loop();
  }
  received = UartReceive(cmd);
  //String received = UartReceive(cmd);
  //Serial.print("received bump val: ");Serial.println(received);

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
    ;; //loop();
  }
  received = UartReceive(cmd);
  //String received = UartReceive(cmd);
  //Serial.print("received val: ");Serial.println(received);

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
    ;; //loop();
  }
  received = UartReceive(cmd);
  //String received = UartReceive(cmd);
  //Serial.print("received val: ");Serial.println(received);

  if (received == "+1-") {
    return true;
  }
  else if (received == "+0-") {
    return false;
  }
}

boolean rearOBS() {
  cmd = "+rearOBS-";
  int t = 0;
  String received = "";

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);


  while (!Serial2.available())
  {
    ;; //loop();
  }
  received = UartReceive(cmd);
  //String received = UartReceive(cmd);
  //Serial.print("received val: ");Serial.println(received);

  if (received == "+1-") {
    return true;
  }
  else if (received == "+0-") {
    return false;
  }
}

boolean rightOBS() {
  cmd = "+rightOBS-";
  int t = 0;
  String received = "";

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);

  //
  while (!Serial2.available())
  {
    ;; //loop();
  }
  received = UartReceive(cmd);
  //String received = UartReceive(cmd);
  //Serial.print("received val: ");Serial.println(received);

  if (received == "+1-") {
    return true;
  }
  else if (received == "+0-") {
    return false;
  }
}

boolean leftOBS() {
  cmd = "+leftOBS-";
  int t = 0;
  String received = "";

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);

  //
  while (!Serial2.available())
  {
    ;; //loop();
  }
  received = UartReceive(cmd);
  //String received = UartReceive(cmd);
  //Serial.print("received val: ");Serial.println(received);

  if (received == "+1-") {
    return true;
  }
  else if (received == "+0-") {
    return false;
  }
}

boolean sideOBS() {
  cmd = "+sideOBS-";
  int t = 0;
  String received = "";

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);


  while (!Serial2.available())
  {
    ;; //loop();
  }
  received = UartReceive(cmd);
  //String received = UartReceive(cmd);
  //Serial.print("received val: ");Serial.println(received);

  if (received == "+1-") {
    return true;
  }
  else if (received == "+0-") {
    return false;
  }
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
    ;; //loop();
  }
  received = UartReceive(cmd);

  if (received == "+1-") {
    return true;
  }
  else if (received == "+0-") {
    return false;
  }
}


boolean haspower() {
  docking = true; // prevents power check, which turns on the switch, which would give a false positive
  // since the goal here is to detect docking current, not battery current...
  cmd = "+haspower-";
  int t = 0;
  String received = "";

  //Serial2.flush(); //Waits for the transmission of outgoing serial data to complete.
  UartSend(cmd);

  //  docking = false; this will be reset only after a succesful docking
  
  while (!Serial2.available())
  {
    ;; //loop();
  }
  received = UartReceive(cmd);

  if (received == "+1-") {
    return true;
  }
  else if (received == "+0-") {
    return false;
  }
}
