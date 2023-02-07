

void wristLeft() {
  digitalWrite(Wrist1, 1);
  digitalWrite(Wrist2, 0);
  delay(200);
  digitalWrite(Wrist1, 0);
  digitalWrite(Wrist2, 0);

}

void wristRight() {
  digitalWrite(Wrist1, 0);
  digitalWrite(Wrist2, 1);
  delay(200);
  digitalWrite(Wrist1, 0);
  digitalWrite(Wrist2, 0);
}

void armUp() {
  digitalWrite(Arm1, 1);
  digitalWrite(Arm2, 0); // up
  delay(200);
  digitalWrite(Arm1, 0);
  digitalWrite(Arm2, 0);
}

void armDown() {
  digitalWrite(Arm1, 0);
  digitalWrite(Arm2, 1); // down
  delay(200);
  digitalWrite(Arm1, 0);
  digitalWrite(Arm2, 0);
}

void foreArmUp() {
  digitalWrite(ForeArm1, 0);
  digitalWrite(ForeArm2, 1); // up
  delay(200);
  digitalWrite(ForeArm1, 0);
  digitalWrite(ForeArm1, 0);
}

void foreArmDown() {
  digitalWrite(ForeArm1, 1);
  digitalWrite(ForeArm2, 0); // up
  delay(200);
  digitalWrite(ForeArm1, 0);
  digitalWrite(ForeArm2, 0);
}

void closeClamp() {
  digitalWrite(Clamp1, 1);
  digitalWrite(Clamp2, 0); // up
  delay(200);
  digitalWrite(Clamp1, 0);
  digitalWrite(Clamp2, 0);
}

void openClamp() {
  digitalWrite(Clamp1, 0);
  digitalWrite(Clamp2, 1); // up
  delay(200);
  digitalWrite(Clamp1, 0);
  digitalWrite(Clamp2, 0);
}

