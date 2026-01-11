void loop() {
  //Serial.println(".");

  WebSocketsServer.handleClient();

  ArduinoOTA.handle();

  webSocket.loop();

  st::Everything::run();


  if (millis() - lastMillis > 1000) {
    //Serial.println(rqSp);
    lastMillis = millis();
  }


  /*
    if (!OTArequest) {
    //SafetyChecks();

    MainFunctions();

    if (logs && !AllowOTA) {
      Runlogs();
    }
    else {
      tests = false; // reset this boolean
    }
    }
  */
  yield();
}

void SafetyChecks() {

  if (!disableCliff && !cliffOFF && Cliff() && !flwln) {
    rvs(180, 0);
    delay(100);
    STOP();
    //Serial << "CLIFF!!! Val = " << analogRead(CliffSensor) << endl;

  }

  stuck();

  if (GotStuck && check) {
    STOP();
    breakIt = true;
    //STOPALL = true;

    //GotStuck = false;
  }

}

void MainFunctions() {
  //  ArduinoOTA.handle();
  //  st::Everything::run();

  if (!AllowOTA && !doReset) {

    //webserver();
  }
  else {
    Serial << "RESET REQUESTED" << endl;
    if (millis() - RstCtDwnStart > resetDelay) { // this delay allows for webserver to finish working before Reset() is called
      Reset(); // reset requested
    }
  }

  if (noturn && !docking && !ignoreobs) {
    if (frontIRVal() > 600 || BUMP() || fusupVAL() > 60) {
      rvs(MEDIUM, 10);
    }
    if (rearOBS()) {
      fwd(MEDIUM, 0);
      delay(100);
      STOP();
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
    StationInLine();
    BUMP();
  }
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
  //
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
  //  Serial.print("SeeLineRight = "); Serial.println(SeeLineRight());
  //  Serial.print("SeeLineLeft = "); Serial.println(SeeLineLeft());
  //
  //  fusup();
  //  Serial.println("");
  //  Serial.println("");
  //  Serial.println("");
  // delay(200);
  //
  //  //    if (!tests || !euristics) {
  //  euristics = true;
  //  tests = true; // avoid redundencies of Signal and StationInLine()

  //Serial << " ANALOG READ IR SENSOR() ? " << analogRead(RECV_PIN) << endl;

  //  if (StationInLine()) {
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
  //    StationInLine();

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
