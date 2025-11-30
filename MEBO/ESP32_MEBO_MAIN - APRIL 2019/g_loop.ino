void loop() {


  ArduinoOTA.handle();


  if (!OTArequest) {

    SafetyChecks();

    MainFunctions();

    if (logs) {
      Runlogs(); //
    }
  }
  delay(1);
}


void SafetyChecks() {

  //    if (!disableCliff && !cliffOFF && Cliff() && !flwln) { // new IR distance sensor instead of prox sensors, so shouldn't need those exceptions any longer.
  if (Cliff() && MvtState == "Forward") {
    rvs(180, 0);
    STOP();
    Serial << "CLIFF!!! Val = " << analogRead(CliffSensor) << endl;
  }

  if (BUMP()) {
    STOP();
    if (!rearOBS()) {
      rvs(160, 0); // must not use math here because it resets bumped boolean value
      delay(100);
      STOP();
    }
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

  if (GotStuck && check && mvt && !ignoreobs) {
    simpleSTOP(); // without resetting values
    Serial << "STUCK!!! " << endl;
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

    //meboOff();// just turning off mebo's cam and built-in controller
    poweroffDONE = true;
    // ST will take care of turning off Main Power
  }

  if (runIRreceiver && !allSet) {
    Serial << "runIRreceiver ACTIVATED! ----------------------------------------------" << endl;
    irrecv.enableIRIn(); // Start the receiver
    allSet = true; // prevent running this function again
  }
  if (runIRreceiver) {
    StationInSight();
  }

  if (millis() - previousMillisBlink > 1000 && !noblink) {
    previousMillisBlink = millis();
    if (STOPALL) {
      Blink(5, 50);
    }
    else {
      Blink(1, 1);
      Serial.println("loop");
    }
  }
}

void Runlogs() {
  runIRreceiver = true;

  //retrieveLineFwd();



  if (millis() - lastMillis > 1000) {
    Blink(5, 30);
    lastMillis = millis();
    Serial.print("SeeLineRightRear() = "); Serial.println(SeeLineRightRear());
    Serial.print("SeeLineLeftRear() = "); Serial.println(SeeLineLeftRear());

    //     Serial.print("fusupVAL() = "); Serial.println(fusupVAL());
    //
    //     Serial.print("frontIRVal() = "); Serial.println(frontIRVal());
    //
    //     Serial.print("fusLVAL() = "); Serial.println(fusLVAL());
    //
    //     Serial.print("fusRVAL() = "); Serial.println(fusRVAL());
    //
    //     Serial.print("leftIRVal() = "); Serial.println(leftIRVal());
    //
    //     Serial.print("rightIRVal() = "); Serial.println(rightIRVal());
    //     Serial.print("rearOBS() = "); Serial.println(rearOBS());


  }
}

void runATest() {
  //runIRreceiver = true;
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

void Encoders()
{
 
}
