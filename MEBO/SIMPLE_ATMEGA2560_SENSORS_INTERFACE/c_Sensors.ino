String OBSstr() {
  if (OBS()) {
    return "+1-";
  }
  else {
    return "+0-";
  }
}

String BUMPstr() {
  if (BUMP()) {
    return "+1-";
  }
  else {
    return "+0-";
  }
}
String BUMPLstr() {
  if (BUMPL()) {
    return "+1-";
  }
  else {
    return "+0-";
  }
}
String BUMPRstr() {
  if (BUMPR()) {
    return "+1-";
  }
  else {
    return "+0-";
  }
}

String frontIRstr() {
  if (frontIR()) {
    return "+1-";
  }
  else {
    return "+0-";
  }
}

int frontIRVal() {
  return analogRead(FIR);
}
int leftIRVal() {
  return analogRead(LIR);
}
int rightIRVal() {
  return analogRead(RIR);
}
int RLIRVal() {
  return analogRead(RLIR);
}
int RRIRVal() {
  return analogRead(RRIR);
}


String fusupstr() {

  if (fusup()) {
    return "+1-";
  }
  else {
    return "+0-";
  }
}


String RearLeftIRstr() {

  if (RearLeftIR()) {
    return "+1-";
  }
  else {
    return "+0-";
  }
}

String RearRightIRstr() {

  if (RearRightIR()) {
    return "+1-";
  }
  else {
    return "+0-";
  }
}


String rearOBSClosestr() {
  if (rearOBSClose()) {
    return "+1-";
  }
  else {
    return "+0-";
  }
}

String rearOBSstr() {
  if (rearOBS()) {
    return "+1-";
  }
  else {
    return "+0-";
  }
}

String rightOBSstr() {
  if (rightOBS()) {
    return "+1-";
  }
  else {
    return "+0-";
  }
}
String leftOBSstr() {
  if (leftOBS()) {
    return "+1-";
  }
  else {
    return "+0-";
  }
}
String sideOBSstr() {
  if (sideOBS()) {
    return "+1-";
  }
  else {
    return "+0-";
  }
}

String RearFrontOKstr() {
  if (sideOBS()) {
    return "+1-";
  }
  else {
    return "+0-";
  }
}

