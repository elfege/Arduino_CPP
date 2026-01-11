void loop() {

  ArduinoOTA.handle();


  if (millis() - lastMillis > 1000 && !noblink) {
    lastMillis = millis();
    StationInSight();
  }

  if (!OTArequest) {

    SafetyChecks();

    MainFunctions();

    if (logs && !AllowOTA) {
      Runlogs();
    }
    else {
      tests = false; // reset this boolean
    }
  }

  delay(1);
}

void SafetyChecks() {

  if (thereIsMvt()) {

    //    if (!disableCliff && !cliffOFF && Cliff() && !flwln) { // new IR distance sensor instead of prox sensors, so shouldn't need those exceptions any longer.
    if (Cliff()) {
      rvs(180, 0);
      STOP();
      Serial << "CLIFF!!! Val = " << analogRead(CliffSensor) << endl;
    }

    if (BUMP()) {
      STOP();
      ignoreNextRq = true;
      previousMillisCanceledRequests = millis();
      Serial << "BUMP!!! " << endl;
    }

    if (noturn && !docking && !ignoreobs) {
      if (frontIRVal() > 600 || BUMP() || fusupVAL() > 60) {
        Serial.println("Auto Front Obs Prevention");
        rvs(MEDIUM, 10);
      }
      if (rearOBS()) {
        Serial.println("Auto Rear Obs Prevention");
        fwd(MEDIUM, 0);
        delay(100);
        STOP();
      }
    }
    stuck();

    if (GotStuck && check) {
      STOP();
      breakIt = true;
      //STOPALL = true;
      previousMillisCanceledRequests = millis();
      //GotStuck = false; // must be reset within functions using this...
      Serial << "STUCK!!! Val = " << endl;
    }
  }
}


void MainFunctions() {

  _server.handleClient();
  st::Everything::run();

  if (doReset) {
    Serial << "RESET REQUESTED" << endl;
    if (millis() - RstCtDwnStart > resetDelay) { // this delay allows for webserver to finish working before Reset() is called
      Reset(); // reset requested
    }
  }


  if (docked && millis() > timeCountToPwOff + DelayMill && !poweroffDONE) {

    meboOff();// just turning off mebo's cam and built-in controller
    poweroffDONE = true;
    // ST will take care of turning off Main Power
  }

  if (euristics && !allSet) {
    Serial << "EURISTICS ACTIVATED! ----------------------------------------------" << endl;
    irrecv.enableIRIn(); // Start the receiver
    allSet = true; // allow euristics to run
  }
  if (allSet && !tests) {
    //Serial.println("--");
    //fusup();
    StationInSight();
    //    BUMP(); // made this one unconditional
  }
  if (millis() - lastMillis > 1000 && !noblink) {
    lastMillis = millis();
    if (!STOPALL) {
      Blink(2, 200);
    }
    else {
      Blink(5, 50);
    }
  }
  //  if(millis() - previousCurrentMeasure > 5000){
  //    getvolts();
  //    previousCurrentMeasure = millis();
  //  }
}

void Runlogs() {
  allSet = false;
  euristics = false;

  //  Serial.println("------------------------------------------------");
  //  Serial.println("");
  //  Serial.print("BUMP = "); Serial.println(BUMP());
  //  Serial.print("BUMP RIGHT = "); Serial.println(BUMPR());
  //  Serial.print("BUMP LEFT = "); Serial.println(BUMPL());
  //
  //  Serial.println("------------------------------------------------");
  Serial.print("fusupVAL = "); Serial.println(fusupVAL());
  Serial.print("fusup = "); Serial.println(fusup());
  //  Serial.println("------------------------------------------------");
  Serial.print("frontIRVal = "); Serial.println(frontIRVal());
  Serial.print("frontIR = "); Serial.println(frontIR());
  //  Serial.println("------------------------------------------------");
  Serial.print("OBS = "); Serial.println(OBS());
  //
  //  Serial.println("------------------------------------------------");
  //
  Serial.print("sideOBS = "); Serial.println(sideOBS());
  //  Serial.print("rightOBS = "); Serial.println(rightOBS());
  //  Serial.print("leftOBS = "); Serial.println(leftOBS());
  Serial.println("------------------------------------------------");
  Serial.print("rightIRVal = "); Serial.println(rightIRVal());
  Serial.print("leftIRVal = "); Serial.println(leftIRVal());
  Serial.println("------------------------------------------------");
  Serial.print("rearOBS = "); Serial.println(rearOBS());
  //    Serial.print("rearOBS = "); Serial.println(RearFrontOK());
  Serial.print("RearLeftIR = "); Serial.println(RearLeftIR());
  Serial.print("RearRightIR = "); Serial.println(RearRightIR());
  Serial.println("------------------------------------------------");
  //
  //
  Serial.print("haspower() = "); Serial.println(haspower());
  Serial.print("LineLeftRear = "); Serial.println(digitalRead(LineLeftRear));
  Serial.print("LineRightRear = "); Serial.println(digitalRead(LineRightRear));
  //
  Serial.print("fusRVAL = "); Serial.println(fusRVAL());
  Serial.print("fusLVAL = "); Serial.println(fusLVAL());
  //  Serial.print("RLIRVal = "); Serial.println(RLIRVal());
  //  Serial.print("RRIRVal = "); Serial.println(RRIRVal());

  Serial.println("------------------------------------------------");
  //
  //
  //  Serial.println("");
  //  Serial.println("");
  //  Serial.println("----------------------LOCAL FUNCTIONS:--------------------------");
  //  Serial.println("");
  //  Serial << "CLIFF Val = " << analogRead(CliffSensor) << endl;
  //  Serial.print("SeeLineRightFront = "); Serial.println(SeeLineRightFront());
  //  Serial.print("SeeLineLeftFront = "); Serial.println(SeeLineLeftFront());
  //
  //  fusup();
  //  Serial.println("");
  //  Serial.println("");
  //  Serial.println("");
  // delay(200);
  //
  //  //    if (!tests || !euristics) {
  //  euristics = true;
  //  tests = true; // avoid redundencies of Signal and StationInSight()

  //Serial << " ANALOG READ IR SENSOR() ? " << analogRead(RECV_PIN) << endl;

  //  if (StationInSight()) {
  //    Serial << " -- STATION IN LINE ++++++++++++++++++++++++++++++++  " << endl;
  //    found = false;
  //  }
  //  if (Signal) {
  //    Serial << " -- Signal SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS  " << endl;
  //    Signal = false;
  //  }
  //    else {
  //      Serial << " -- NO SIGNAL --- 0000000000000000000000000000000" << endl;
  //    }
  //    stuck();
  //    fusup();
  //    StationInSight();

  //delay(500);
}

void runATest() {
  //euristics = true;
  //stopcharge();
  //delay(1000);

  STOPALL = false;

  Start = millis();
  while (millis() - Start < 10000) {
    if (!OBS()) {
      rvs(180, 0);
    }
    else {
      fwd(180, 0);
    }
  }
}
