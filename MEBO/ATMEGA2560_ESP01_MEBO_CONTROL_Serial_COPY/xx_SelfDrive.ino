
void selfdrive() {
  disableCliff = false;
  ignoreobs = true;
  STOPALL = false;
  boolean rvsdone = false;
  euristics = true;

  while (!STOPALL) {

    GotStuck = false;
    fwd(SPEED, 0);
    while (!STOPALL && !OBS() && !GotStuck) {
      SPEED = MatchD2SFRONT();
      rqSp = SPEED;

      loop();
    }
    if (GotStuck) {
      rvs(220, 0);
      delay(200);
      right(255);
      delay(200);
    }
    GotStuck = false;
    while (OBS() && !STOPALL && !GotStuck && !rearOBS()) {
      GotStuck = false;
      rvs(180, 10);
      GotStuck = false;
      if (rightOBS() > leftOBS()) {
        Start = millis();
        while ((analogRead(FIR) > 200 || distance < 100 ) && !STOPALL && !GotStuck && !rearOBS() && millis() - Start < 3000) {
          left(200);
          loop();
        }
      }
      else if (rightOBS() < leftOBS()) {
        while ((analogRead(FIR) > 200 || distance < 100 ) && !STOPALL && !GotStuck && !rearOBS() && millis() - Start < 3000) {
          right(200);
          loop();
        }
      }
      else {
        left(255);
        delay(400);
        STOP();
        loop();
      }
      if (GotStuck) {
        rvs(220, 0);
        delay(200);
        left(220);
        delay(200);
        fwd(220, 0);
        delay(100);
        left(220);
        delay(200);
        STOP();
      }
      loop();
    }
    while (rearOBS() && !STOPALL && !OBS()) {
      fwd(200, 5);
    }
    loop();
  }
  STOPALL = false;

}

