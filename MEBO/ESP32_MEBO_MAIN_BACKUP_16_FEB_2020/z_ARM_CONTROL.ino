void armUp() {
  Serial.println("ARM UP");
  cmd = "+armUp-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);
}

void armDown() {
  Serial.println("ARM DOWN");
  cmd = "+armDown-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);
}
void armUpStep() {
  Serial.println("ARM UP STEP");
  cmd = "+armUpStep-";
  int t = 0;
  String received;

  _server.send(200, "text/plain", cmd);
  UartSend(cmd);

}

void armDownStep() {
  Serial.println("ARM DOWN STEP");
  cmd = "+armDownStep-";
  int t = 0;
  String received;

  _server.send(200, "text/plain", cmd);
  UartSend(cmd);

}

void foreArmUpStep() {
  Serial.println("ARM UP STEP");
  cmd = "+foreArmUp-";
  int t = 0;
  String received;

  _server.send(200, "text/plain", cmd);
  UartSend(cmd);
  delay(200);
  stopArm();

}

void foreArmDownStep() {
  Serial.println("ARM DOWN STEP");
  cmd = "+foreArmDown-";
  int t = 0;
  String received;

  _server.send(200, "text/plain", cmd);
  UartSend(cmd);
  delay(200);
  stopArm();

}

void armCenter() {
  Serial.println("ARM DOWN STEP");
  cmd = "+armCenter-";
  int t = 0;
  String received;

  _server.send(200, "text/plain", cmd);
  UartSend(cmd);

}



void foreArmUp() {
  Serial.println("FOREARM UP");
  cmd = "+foreArmUp-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);

}

void foreArmDown() {
  Serial.println("FOREARM DOWN");
  cmd = "+foreArmDown-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);

}


void wristMiddle() {
  Serial.println("WRIST MIDDLE");
  cmd = "+wristMiddle-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);

}
void wristLeft() {
  Serial.println("WRIST LEFT");
  cmd = "+wristLeft-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);
}

void wristRight() {
  Serial.println("WRIST RIGHT");
  cmd = "+wristRight-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);
}

void wristleftstep() {
  Serial.println("WRIST STEP LEFT");
  cmd = "+wristleftstep-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);
}
void wristrightstep() {
  Serial.println("WRIST STEP RIGHT");
  cmd = "+wristrightstep-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);
}

void closeClamp() {
  Serial.println("CLOSE CLAMP");
  cmd = "+closeClamp-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);
}

void openClamp() {
  Serial.println("OPEN CLAMP");
  cmd = "+openClamp-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);
}

void stopArm() {
  cmd = "+stoparm-";
  int t = 0;
  String received = "";
  UartSend(cmd);
  _server.send(200, "text/plain", cmd);

}

String RemoveCharAndSend(String cmd) {
  String temp = cmd; // don't modify cmd for it's a global.
  temp.remove(0 , 1); // remove 1 char at index 0
  temp.remove(temp.length() - 1 , 1); // remove 1 char at the end
  _server.send(200, "text/plain", temp);
}
