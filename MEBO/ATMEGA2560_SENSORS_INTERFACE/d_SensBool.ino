
// String Booleans must be kept as actual Booleans for furhter comparisons (such as OBS())

boolean OBS() {

  bool obstacle = frontIR() || BUMP() || fusup() ;

  if (obstacle) {
    return true;
  }
  else {
    return false;
  }
}

boolean BUMP() {
  // reversed logic because digital port INPUT_PULLUP
  if (BUMPL() || BUMPR()) {
    return false;
  }
  else {
    return true;
  }
}

boolean BUMPL() {
  // reversed logic because digital port INPUT_PULLUP

  if (digitalRead(BL)) {
    return false;
  }
  else {
    return true;
  }
}
boolean BUMPR() {
  // reversed logic because digital port INPUT_PULLUP

  if (digitalRead(BR)) {
    return false;
  }
  else {
    return true;
  }
}

boolean frontIR() {
  if (analogRead(FRIR) > 600 || analogRead(FLIR) > 600) {
    return true;
  }
  else {
    return false;
  }
}

boolean fusup() {
  if (fusupVAL() < 50) {
    return true;
  }
  else {
    return false;
  }
}


int fusupVAL() {
  // report whichever is the smaller value

  int left = fusupVAL_L();
  int right = fusupVAL_R();
  int a = min(left, right);

  Start = millis();
  int c = 0;
  while (a < 10 && c < 10) {
    left = fusupVAL_L();
    right = fusupVAL_R();
    a = min(left, right);
    c++;
  }
  if (c >= 10) {
    return a;
  }
  else {
    left = fusupVAL_L();
    right = fusupVAL_R();
    a = min(left, right);
    return a;
  }

  //  Serial.println("");
  //  Serial << "left = " << left << endl;
  //  Serial << "right = " << right << endl;
  //  Serial << "returning = " << a << endl;

  return a;
}

int fusupVAL_L() {

  // Clears the trigPinUPL
  digitalWrite(trigPinUPL, LOW);
  delayMicroseconds(5);
  // Sets the trigPinUPL on HIGH state for 10 micro seconds
  digitalWrite(trigPinUPL, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinUPL, LOW);

  // Reads the echoPinUPL, returns the sound wave travel time in microseconds
  long duration =pulseIn(echoPinUPL, HIGH);

  // Calculating the distanceUP
  int distanceUPL = (duration / 2) * 0.0343 ;

  return distanceUPL;
}

int fusupVAL_R() {

  // Clears the trigPinUPL
  digitalWrite(trigPinUPR, LOW);
  delayMicroseconds(5);
  // Sets the trigPinUPL on HIGH state for 10 micro seconds
  digitalWrite(trigPinUPR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinUPR, LOW);

  // Reads the echoPinUPL, returns the sound wave travel time in microseconds
  long duration =pulseIn(echoPinUPR, HIGH);

  // Calculating the distanceUP
  int distanceUPR = (duration / 2) * 0.0343 ;

  return distanceUPR;
}

int fusRVAL() {

  // Clears the trigPinUPL
  digitalWrite(trigPinFR, LOW);
  delayMicroseconds(5);
  // Sets the trigPinUPL on HIGH state for 10 micro seconds
  digitalWrite(trigPinFR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinFR, LOW);

  // Reads the echoPinUPL, returns the sound wave travel time in microseconds
  long duration =pulseIn(echoPinFR, HIGH);

  // Calculating the distanceUP
  distanceFR = (duration / 2) * 0.0343 ;

  // Prints the distanceUP on the Serial Monitor
  //   Serial.print("distanceUP: ");Serial.println(distanceUP);


  return distanceFR;
}

int fusLVAL() {

  // Clears the trigPinUPL
  digitalWrite(trigPinFL, LOW);
  delayMicroseconds(5);
  // Sets the trigPinUPL on HIGH state for 10 micro seconds
  digitalWrite(trigPinFL, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinFL, LOW);

  // Reads the echoPinUPL, returns the sound wave travel time in microseconds
  long duration =pulseIn(echoPinFL, HIGH);

  // Calculating the distanceUP
  distanceFL = (duration / 2) * 0.0343 ;

  // Prints the distanceUP on the Serial Monitor
  //   Serial.print("distanceUP: ");Serial.println(distanceUP);


  return distanceFL;
}

boolean RearRightIR() {
  bool val = analogRead(RRIR) > 600;
  return val;
}

boolean RearLeftIR() {
  bool val = analogRead(RLIR) > 600;
  return val;
}



boolean rearOBSClose() {
  bool result = analogRead(RRIR) > 700 || analogRead(RLIR) > 700;
  return result;
}

boolean rearOBS() {
  bool result = RearLeftIR() || RearRightIR();
  return result;
}

boolean rightOBS() {
  bool result = analogRead(RIR) > 600;
  return result;
}
boolean leftOBS() {
  bool result = leftIRVal() > 600;
  return result;
}
boolean sideOBS() {
  bool result = leftOBS() && rightOBS();
  return result;
}


boolean RearFrontOK() {
  boolean a = analogRead(RRIR) <= 300 &&  analogRead(RLIR) <= 300 &&  analogRead(FRIR) <= 300 &&  analogRead(FLIR) <= 300;
  return a;
}


boolean ArmIsDown() {
  return !digitalRead(ArmDownS);
}

boolean ArmIsUp() {
  return !digitalRead(ArmUpS);
}

boolean ForeArmIsUp() {
  return !digitalRead(ForeUpS);
}

boolean ForeArmIsDown() {
  return !digitalRead(ForeDownS);
}

boolean ClampIsOpen() {
  return !digitalRead(ClampOpenS);
}

boolean ClampIsClosed() {
  return !digitalRead(ClampClosedS);
}

boolean Wrist_L_EndReached() {
  return !digitalRead(Wrist_L_EndS);
}
boolean Wrist_R_EndReached() {
  return !digitalRead(Wrist_R_EndS);
}
boolean Wrist_Middle_Reached() {
  return !digitalRead(Wrist_Middle);
}
boolean ArmIsCentered() {
  return !digitalRead(Arm_Center);
}

boolean haspower() {
  bool result = analogRead(PW) >= 500;

  return result;
}
