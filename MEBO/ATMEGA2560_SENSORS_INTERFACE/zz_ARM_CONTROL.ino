void wristMiddle() {
  wristRight(); // go all the way to the right (counterclockwise)
  digitalWrite(Wrist1, 1);
  digitalWrite(Wrist2, 0); // go to the left until you detect the middle position sensor
  Start = millis();
  while (!Wrist_Middle_Reached() && !Wrist_L_EndReached() && !stoparm && millis() - Start < 2000) {
    tasks();
  }
  delay(300);
  digitalWrite(Wrist1, 0);
  digitalWrite(Wrist2, 0);

}

void wristLeft() {
  stoparm = false;
  Serial.println("Wrist Going Left");
  digitalWrite(Wrist1, 1);
  digitalWrite(Wrist2, 0);
  Start = millis();
  while (!Wrist_L_EndReached() && !stoparm && millis() - Start < 1500) {
    tasks();
  }
  Serial.println("Wrist Left Motion Done");
  //  delay(400);
  digitalWrite(Wrist1, 0);
  digitalWrite(Wrist2, 0);

}

void wristRight() {
  stoparm = false;
  Serial.println("Wrist Going Right");
  digitalWrite(Wrist1, 0);
  digitalWrite(Wrist2, 1);
  Start = millis();
  while (!Wrist_R_EndReached() && !stoparm && millis() - Start < 1500) {
    tasks();
  }
  //  delay(400);
  Serial.println("Wrist Right Motion Done");
  digitalWrite(Wrist1, 0);
  digitalWrite(Wrist2, 0);
}

void wristLeftStep() {
  stoparm = false;
  Serial.println("Wrist Going Left");
  if (!Wrist_L_EndReached() && !stoparm) {
    digitalWrite(Wrist1, 1);
    digitalWrite(Wrist2, 0);
    delay(200);
    Serial.println("Wrist Left Motion Done");
    digitalWrite(Wrist1, 0);
    digitalWrite(Wrist2, 0);
  }
  else {
    Serial.println("Left End Reached");
  }
}
void wristRightStep() {
  stoparm = false;
  Serial.println("Wrist Going Right");
  if (!Wrist_R_EndReached() && !stoparm) {
    digitalWrite(Wrist1, 0);
    digitalWrite(Wrist2, 1);
    delay(200);
    Serial.println("Wrist Right Motion Done");
    digitalWrite(Wrist1, 0);
    digitalWrite(Wrist2, 0);
  }
  else {
    Serial.println("Right End Reached");
  }
}

void armUp() {
  stoparm = false;
  Serial.println("ARM going UP");
  digitalWrite(Arm1, 1);
  digitalWrite(Arm2, 0); // up
  unsigned long ArmStart = millis();
  while (!ArmIsUp() && !stoparm &&  millis() - ArmStart < 3000) {
    tasks();
    delay(1);
  }
  Serial.println("ARM IS UP");

  digitalWrite(Arm1, 0);
  digitalWrite(Arm2, 0);

}

void armDown() {
  stoparm = false;
  Serial.println("ARM going down");
  digitalWrite(Arm1, 0);
  digitalWrite(Arm2, 1); // down
  unsigned long ArmStart = millis();
  while (!ArmIsDown() && !stoparm && millis() - ArmStart < 3000) {
    tasks();
    delay(1);
  }
  Serial.println("ARM IS DOWN");

  digitalWrite(Arm1, 0);
  digitalWrite(Arm2, 0);

}


void armUpStep() {
  stoparm = false;
  Serial.println("ARM stepping UP");

  if (!ArmIsUp() && !stoparm) {
    digitalWrite(Arm1, 1);
    digitalWrite(Arm2, 0); // up
  }

  delay(200);

  digitalWrite(Arm1, 0);
  digitalWrite(Arm2, 0);

}

void armDownStep() {
  stoparm = false;
  Serial.println("ARM stepping down");

  if (!ArmIsDown() && !stoparm) {
    digitalWrite(Arm1, 0);
    digitalWrite(Arm2, 1); // down
  }

  delay(200);
  Serial.println("Arm is down");

  digitalWrite(Arm1, 0);
  digitalWrite(Arm2, 0);

}

void armCenter() {
  stoparm = false;

  Serial.println("ARM CENTER");

  int centerDelay = 0;
  int c = 0;
  // never go up in this function for robot can be docked thus it'd push against the wall behind it
  armDown();
  digitalWrite(Arm1, 1);
  digitalWrite(Arm2, 0); // up
  unsigned long Acenter = millis();
  while (!ArmIsCentered() && !ArmIsDown() && !stoparm && millis() - Acenter < 4000) {
    tasks();
    delay(1);
  }
  if (!ArmIsCentered()) {
    Serial.println("FAILED, trying a different method.");
    armDown(); // just to make sure that's where we start from
    digitalWrite(Arm1, 1);
    digitalWrite(Arm2, 0); // up
    // try again, using time
    while (!ArmIsCentered() && !stoparm && millis() - Acenter < 4000) {
      tasks();
      delay(1);
    }
    centerDelay = 0;
  }
  else {
    centerDelay = 1000; // delay for when methode 1 succeeded
  }

  if (!ArmIsCentered()) {
    Serial.println("FATAL FAILURE... ARM COULD NOT BE CENTERED.");
  }
  else {
    Serial.println("Arm is Centered");
  }

  delay(centerDelay); // sensor offset
  digitalWrite(Arm1, 0);
  digitalWrite(Arm2, 0);

}

void foreArmUp() {
  stoparm = false;
  Serial.println("Forearm going up");
  digitalWrite(ForeArm1, 0);
  digitalWrite(ForeArm2, 1); // up
  unsigned long ArmStart = millis();
  while (!ForeArmIsUp() && !stoparm &&  millis() - ArmStart < 3000) {
    tasks();
    delay(1);
  }
  Serial.println("Forearm is up");
  digitalWrite(ForeArm1, 0);
  digitalWrite(ForeArm2, 0);
}

void foreArmDown() {
  stoparm = false;
  Serial.println("Forearm going down");
  digitalWrite(ForeArm1, 1);
  digitalWrite(ForeArm2, 0); // up

  unsigned long ArmStart = millis();
  while (!ForeArmIsDown() && !stoparm &&  millis() - ArmStart < 3000) {
    tasks();
    delay(1);
  }
  Serial.println("Forearm is down");
  digitalWrite(ForeArm1, 0);
  digitalWrite(ForeArm2, 0);
}


void armStop(){
  digitalWrite(Arm1, 0);
  digitalWrite(Arm2, 0);
}
void foreArmStop()
{
  digitalWrite(ForeArm1, 0);
  digitalWrite(ForeArm2, 0);
}

void closeClamp() {
  stoparm = false;
  Serial.println("CLOSING CLAMPS");
  digitalWrite(Clamp1, 1);
  digitalWrite(Clamp2, 0); // up
  unsigned long ArmStart = millis();
  while (!ClampIsClosed() && !stoparm &&  millis() - ArmStart < 3000) {
    tasks();
  }
  if (!ClampIsOpen()) {
    Serial.println("Clamp is Closed");
  }
  else {
    Serial.println("STOPPED BY SOMETHING ELSE");
  }

  digitalWrite(Clamp1, 0);
  digitalWrite(Clamp2, 0);
}

void openClamp() {
  stoparm = false;
  Serial.println("OPENING CLAMPS");
  digitalWrite(Clamp1, 0);
  digitalWrite(Clamp2, 1); // up

  unsigned long ArmStart = millis();
  while (!ClampIsOpen() && !stoparm &&  millis() - ArmStart < 3000) {
    tasks();
  }
  if (ClampIsOpen()) {
    Serial.println("Clamp is Open");
  }
  else {
    Serial.println("STOPPED BY SOMETHING ELSE");
  }
  digitalWrite(Clamp1, 0);
  digitalWrite(Clamp2, 0);
}
