void armUp() {
  Serial.println("ARM UP");
  cmd = "+armUp-";
  int t = 0;
  String received;

  UartSend(cmd);
  Start = millis();
}

void armDown() {
  Serial.println("ARM DOWN");
  cmd = "+armDown-";
  int t = 0;
  String received;

  UartSend(cmd);
  Start = millis();
}

void foreArmUp() {
  Serial.println("FOREARM UP");
  cmd = "+foreArmUp-";
  int t = 0;
  String received;

  UartSend(cmd);
  Start = millis();
}

void foreArmDown() {
  Serial.println("FOREARM DOWN");
  cmd = "+foreArmDown-";
  int t = 0;
  String received;

  UartSend(cmd);
  Start = millis();

}

void wristMiddle() {
  Serial.println("WRIST MIDDLE");
  cmd = "+wristMiddle-";
  int t = 0;
  String received;

  UartSend(cmd);
  Start = millis();

}
void wristLeft() {
  Serial.println("WRIST LEFT");
  cmd = "+wristLeft-";
  int t = 0;
  String received;

  UartSend(cmd);
  Start = millis();

}

void wristRight() {
  Serial.println("WRIST RIGHT");
  cmd = "+wristRight-";
  int t = 0;
  String received;

  UartSend(cmd);
  Start = millis();

}

void wristleftstep(){
  Serial.println("WRIST STEP LEFT");
  cmd = "+wristleftstep-";
  int t = 0;
  String received;

  UartSend(cmd);
  Start = millis();
}
void wristrightstep(){
  Serial.println("WRIST STEP RIGHT");
  cmd = "+wristrightstep-";
  int t = 0;
  String received;

  UartSend(cmd);
  Start = millis();
}

void closeClamp() {
  Serial.println("CLOSE CLAMP");
  cmd = "+closeClamp-";
  int t = 0;
  String received;

  UartSend(cmd);
  Start = millis();

}

void openClamp() {
  Serial.println("OPEN CLAMP");
  cmd = "+openClamp-";
  int t = 0;
  String received;

  UartSend(cmd);
  Start = millis();

}
