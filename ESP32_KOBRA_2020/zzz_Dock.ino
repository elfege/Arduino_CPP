

void FollowLineFWD() {
  String state = "docking";
  server.send(200, "text/plain", state);

  docking = true;
  digitalWrite(MotorsPw, 1);
  chargeoff();
  servosdockposition();
  delay(1000);
  fwd();
  delay(200);
  STOP();

  int last = 0;
  STOPALL = false;
  rqSp = 200;
  int del = 50;
  unsigned long GStart = millis();//  set a time limit

  while (!STOPALL) { // all other breaking arguments are conditioned within the loop for debug purpose

    rqSp = 185; 
    fwd();
    delay(50);
    STOP();
    delay(50);
    execLineFwd(185);
    if (haspower())
    {
      Serial.println("POWER");
      break;
    }
    if (millis() - GStart > 10000)
    {
      Serial.println("TIME");
      break;
    }
    if (BothFrontSeen())
    {
      Serial.println("BOTH SEEN");
      break;
    }
    

    loop();

  }
  if (BothFrontSeen())
  {
    rqSp = 255;
    fwd();
    delay(100); // last push at least for 1 second
    while (!STOPALL && !haspower() && millis() - GStart < 2000) {
      loop();
    }
    if (haspower())
    {
      STOP();
      chargeon();
      docking = false;
      server.send(200, "text/plain", "DOCKED!"); // update webpage value
    }

  }
  STOP();
  STOPALL = false;
  Serial.println("END OF DOCKING");
  docking = false;

}

boolean haspower() {
  return getvoltsonly() > 1;

}

void execLineFwd(int SPEED) {
  int del = 50;
  rqSp = SPEED;
  while (SeeLineLeftFront() && !STOPALL && !haspower()) {
    left();
    delay(del);
    STOP();
    delay(del);
  }
  while (SeeLineRightFront() && !STOPALL && !haspower()) {
    right();
    delay(del);
    STOP();
    delay(del);
  }
}



boolean SeeLineLeftFront() {
  return analogRead(LineLeftFront) > 3500;
}
boolean SeeLineRightFront() {
  return analogRead(LineRightFront) > 3500;
}
boolean BothFrontSeen() {
  bool result = SeeLineLeftFront() && SeeLineRightFront();
  if (result) {
    // double check
    delay(30);
  }
  result = SeeLineLeftFront() && SeeLineRightFront();
  return result;
}
