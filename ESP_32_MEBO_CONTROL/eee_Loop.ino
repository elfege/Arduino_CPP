

void loop()
{

  ArduinoOTA.handle();

  st::Everything::run();

  if (requireHeuristics && !allSet) {
    irrecv.enableIRIn(); // Start the receiver
    allSet = true; // allow heuristics to run 
  }
  if (allSet) {
    stuck();
    StationInView();
  }

  if (logs && !AllowOTA) {

    //Serial.print(".");
    //// due to delay(1000) printings must happen outside OTA allowance
    //StationInView();

    //    Serial.println("");
    //    Serial << " MatchD2S() ? " << MatchD2S(1500, 500, 160, 255) << endl;
    //
    //    Serial << " _________________________________________________ ? " << endl;
    //    Serial << " BothFrontSeen() ? " << BothFrontSeen() << endl;
    //    Serial << " OneSeesTheLine ? " << OneSeesTheLine() << endl;
    //    Serial << " _________________________________________________ ? " << endl;
    //
        Serial << " analogRead(PW) ? " << analogRead(PW) << endl;
    //    Serial << " analogRead(RLIR) ? " << analogRead(RLIR) << endl;
    //    Serial << " _________________________________________________ ? " << endl;
    //    Serial << " SeeLineRearLeft ? " << SeeLineRearLeft() << endl;
    //    Serial << " SeeLineRearRight ? " << SeeLineRearRight() << endl;
    //    Serial << " SeeLineLeft() ? " << SeeLineLeft() << endl;
    //    Serial << " SeeLineRight() ? " << SeeLineRight() << endl;
    //    Serial << " _________________________________________________ ? " << endl;
    //    Serial << " _________________________________________________ ? " << endl;
    //    Serial << " ANALOG READ F-LEFT ? " << analogRead(LineLeft) << endl;
    //    Serial << " ANALOG READ F-RIGHT ? " << analogRead(LineRight) << endl;
    //    Serial << " ANALOG READ R-LEFT() ? " << analogRead(LineRearLeft) << endl;
    //    Serial << " ANALOG READ R-RIGHT() ? " << analogRead(LineRearRight) << endl;

    Serial << " REAR RIGHT =  " << analogRead(RRIR) << endl;
    Serial << " REAR LEFT  =  " << analogRead(RLIR) << endl;

    // Serial << " ANALOG READ IR SENSOR() ? " << analogRead(RECV_PIN) << endl;

    if (stuck()) {
      Serial << " -- STUCK --- " << endl;
    }

        delay(1500);
  }


  if (!AllowOTA && !doReset) {
    webserver();
  }
  else {
    Reset(); // reset requested by webserver
  }

  // check power value every X minutes ////////////////////////////// YET TOO RISKY
  //  if (millis() > previousMillisPowerTest + DELAYpowertest) {
  //    previousMillisPowerTest = millis();
  //    stopcharge();// stop charge so next commands can check if stillc connected
  //    delay(500);
  //  }
  //
  //  if (docked && !haspower() && !testAlreadyDone) {
  //    testAlreadyDone = true; // so it doesn't get stuck trying to do it forever if failed
  //    Start = millis();
  //    rvs(180,0);
  //    while(!haspower() && millis() < Start + 5000 && !STOPALL){
  //      loop();
  //    }
  //  }
  //  else if (haspower()) {
  //    testAlreadyDone = false;
  //  }



  if (noturn && !docking && !ignoreobs) {
    if (analogRead(FIR) > 2000 || BUMP()) {
      rvs(MEDIUM, 0);
      delay(200);
      STOP();
    }
    if (rearOBS()) {
      fwd(MEDIUM, 0);
      delay(100);
      STOP();
    }
  }
  if (docked && millis() > timeCountToPwOff + DelayMill && !poweroffDONE) {

    poweroff();// just turning off mebo's cam and built-in controller
    poweroffDONE = true;
    // ST will take care of turning off Main Power
  }

  //delay(1);

}
