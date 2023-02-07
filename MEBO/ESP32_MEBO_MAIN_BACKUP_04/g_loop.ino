void loop() {

  ArduinoOTA.handle();


  if (!OTArequest) {

    SafetyChecks();

    MainFunctions();

    if (logs) {
      Runlogs(); //
    }
  }

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
      rvs(160, 2);
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
    STOP();
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

    meboOff();// just turning off mebo's cam and built-in controller
    poweroffDONE = true;
    // ST will take care of turning off Main Power
  }

  if (runIRreceiver && !allSet) {
    Serial << "runIRreceiver ACTIVATED! ----------------------------------------------" << endl;
    irrecv.enableIRIn(); // Start the receiver
    allSet = true; // prevent running this function again
  }
  if(runIRreceiver){
    StationInSight();
  }
  
  if (millis() - previousMillisBlink > 1000 && !noblink) {
    previousMillisBlink = millis();
    if (!STOPALL) {
      Blink(2, 200);
    }
    else {
      Blink(5, 50);
    }
  }
}

void Runlogs() {
runIRreceiver = true;

  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    //    Serial.print("SeeLineRightRear() = "); Serial.println(SeeLineRightRear());
    //    Serial.print("SeeLineLeftRear() = "); Serial.println(SeeLineLeftRear());
    
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
