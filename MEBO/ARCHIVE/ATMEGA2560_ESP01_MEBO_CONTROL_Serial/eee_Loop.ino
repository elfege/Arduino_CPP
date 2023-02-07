

void loop()
{

  ArduinoOTA.handle();

  st::Everything::run();

  if (euristics && !allSet) {
    Serial << "EURISTICS ACTIVATED! ----------------------------------------------" << endl;
    irrecv.enableIRIn(); // Start the receiver
    allSet = true; // allow euristics to run
  }
  if (allSet && !tests) {
    stuck();

    fusup();
    StationInLine();
    BUMP();
  }

  if (logs && !AllowOTA) {

    //Serial.print(".");
    //// due to delay(1000) printings must happen outside OTA allowance
    //StationInLine();

    //    Serial.println("");
    //
    //    //    Serial << " analogRead(PW) ? " << analogRead(PW) << endl;
    //            Serial << " REAR RIGHT =  " << analogRead(RRIR) << endl;
    //            Serial << " REAR LEFT  =  " << analogRead(RLIR) << endl;
    //    //    Serial << " _________________________________________________ ? " << endl;
    //            Serial << " RIGHT =  " << analogRead(RIR) << endl;
    //            Serial << " LEFT  =  " << analogRead(LIR) << endl;
    //            Serial << " FRONT  =  " << analogRead(FIR) << endl;
    //    Serial << " _________________________________________________ ? " << endl;
    //    Serial << " analogRead(FIR) ? " << analogRead(FIR) << endl;
    //    Serial << " _________________________________________________ ? " << endl;
    //    Serial << " _________________________________________________ ? " << endl;
    Serial << " BUMP ? " << digitalRead(bumberPin) << endl;
    Serial << " bumped ? " << bumped << endl;
    //    Serial << " _________________________________________________ ? " << endl;
    //
    //    fusup();
    //    Serial << " FUSUP distance  =  " << distance << endl;
    //    Serial << " _________________________________________________ ? " << endl;
    //
    //    Serial << " _________________________________________________ ? " << endl;
    //    Serial << " BUMP ? " << digitalRead(bumberPin) << endl;
    //    Serial << " _________________________________________________ ? " << endl;
    //
    //    Serial << " rearOBS() ?  =  " << rearOBS() << endl;
    //    Serial << " OBS() ?  =  " << OBS() << endl;
    //    Serial << " leftOBS() ?  =  " << leftOBS() << endl;
    //    Serial << " rightOBS() ?  =  " << rightOBS() << endl;
    //    Serial << " _________________________________________________ ? " << endl;
    //




    Serial << " SeeLineLeft() ? " << SeeLineLeft() << endl;
    Serial << " SeeLineRight() ? " << SeeLineRight() << endl;
    //    Serial << " _________________________________________________ ? " << endl;
    //    Serial << " _________________________________________________ ? " << endl;
    //    Serial << " ANALOG READ F-LEFT ? " << analogRead(LineLeft) << endl;
    //    Serial << " ANALOG READ F-RIGHT ? " << analogRead(LineRight) << endl;
    //    Serial << " ANALOG READ IR SENSOR() ? " << analogRead(RECV_PIN) << endl;
    //
    //    if (stuck()) {
    //      Serial << " -- STUCK --- " << endl;
    //    }


    //    if (!tests || !euristics) {
    euristics = true;
    tests = true; // avoid redundencies of Signal and StationInLine()

  
//    if (StationInLine()) {
//      Serial << " -- STATION IN LINE ++++++++++++++++++++++++++++++++  " << endl;
//    }
//    if (Signal) {
//      Serial << " -- Signal -------------------------------------  " << endl;
//      Signal = false;
//    }
//    else {
//      Serial << " -- NO SIGNAL --- 0000000000000000000000000000000" << endl;
//    }
    //    stuck();
    //    fusup();
    //    StationInLine();

    //delay(1000);
  }
  else {
    tests = false; // reset this boolean
  }


  if (!disableCliff && !cliffOFF) {
    if (SeeLineLeft() || SeeLineLeft() && !flwln) {
      //      rvs(180, 0);
      //      delay(100);
      STOP();
    }
  }
  else {
    // Serial << "CLIFF SENSORS DISABLED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<
  }

  if (!AllowOTA && !doReset) {
    webserver();
  }
  else if (millis() - RstCtDwnStart > 1000) { // this delay allows for webserver to finish working before Reset() is called
    Serial << "RESET REQUESTED" << endl;
    delay(1000);
    Reset(); // reset requested
  }

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
  //  while (Serial.available()) {
  //    char c = Serial.read();  //gets one byte from serial buffer
  //    currentLine += c; //makes the String currentLine
  //    delay(2);  //slow looping to allow buffer to fill with next character
  //  }




}


void runATest() {
  euristics = true;
  stopcharge();
  delay(1000);
  STOPALL = false;
  //unstuck();
  SearchSignal();
  //unstuckRVS();
  //  RandomSearch();
}


