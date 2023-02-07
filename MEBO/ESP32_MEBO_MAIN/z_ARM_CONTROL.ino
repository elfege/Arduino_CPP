void armUp() {
  term.println("ARM UP");
  cmd = "+armUp-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);
}

void armDown() {
  term.println("ARM DOWN");
  cmd = "+armDown-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);
}
void armUpStep() {
  term.println("ARM UP STEP");
  cmd = "+armUpStep-";
  int t = 0;
  String received;

  _server.send(200, "text/plain", cmd);
  UartSend(cmd);

}

void armDownStep() {
  term.println("ARM DOWN STEP");
  cmd = "+armDownStep-";
  int t = 0;
  String received;

  _server.send(200, "text/plain", cmd);
  UartSend(cmd);

}

void foreArmUpStep() {
  term.println("ARM UP STEP");
  cmd = "+foreArmUp-";
  int t = 0;
  String received;

  _server.send(200, "text/plain", cmd);
  UartSend(cmd);
  delay(200);
  stopArm();

}

void foreArmDownStep() {
  term.println("ARM DOWN STEP");
  cmd = "+foreArmDown-";
  int t = 0;
  String received;

  _server.send(200, "text/plain", cmd);
  UartSend(cmd);
  delay(200);
  stopArm();

}

void armCenter() {
  term.println("ARM DOWN STEP");
  cmd = "+armCenter-";
  int t = 0;
  String received;

  _server.send(200, "text/plain", cmd);
  UartSend(cmd);

}



void foreArmUp() {
  term.println("FOREARM UP");
  cmd = "+foreArmUp-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);

}

void foreArmDown() {
  term.println("FOREARM DOWN");
  if (fusRVAL() < 4) {
    term.println("Forearm less than 4 cm from obstacle/ground");
  }
  else {
    cmd = "+foreArmDown-";
    int t = 0;
    String received;

    UartSend(cmd);
    _server.send(200, "text/plain", cmd);
  }

}


void wristMiddle() {
  term.println("WRIST MIDDLE");
  cmd = "+wristMiddle-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);

}
void wristLeft() {
  term.println("WRIST LEFT");
  cmd = "+wristLeft-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);
}

void wristRight() {
  term.println("WRIST RIGHT");
  cmd = "+wristRight-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);
}

void wristleftstep() {
  term.println("WRIST STEP LEFT");
  cmd = "+wristleftstep-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);
}
void wristrightstep() {
  term.println("WRIST STEP RIGHT");
  cmd = "+wristrightstep-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);
}

void closeClamp() {
  term.println("CLOSE CLAMP");
  cmd = "+closeClamp-";
  int t = 0;
  String received;

  UartSend(cmd);
  _server.send(200, "text/plain", cmd);
}

void openClamp() {
  term.println("OPEN CLAMP");
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
