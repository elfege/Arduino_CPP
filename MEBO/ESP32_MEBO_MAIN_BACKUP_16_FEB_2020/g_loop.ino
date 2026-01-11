void loop() {
  loopTime = millis();
  //digitalWrite(watchDogPin, 0);
  //    Serial.println("PIN TO 0 OK --> " + String(digitalRead(watchDogPin)));

  ArduinoOTA.handle();


  if (!OTArequest) {

    //    SafetyChecks(); // this fucks everything up ....

    MainFunctions();


  }

  //digitalWrite(watchDogPin, 1);
  delay(10);// give time to core0 to notice the change of state
  //    Serial.println("PIN TO 1 OK --> " + String(digitalRead(watchDogPin)));
  elapsed = millis() - loopTime;

}


void SafetyChecks() {

  //Serial.println("aa1");

  //Serial.println("aa2");
  if (BUMP() && !isSelfDriving) {
    //Serial.println("aa2b");
    STOP();
    //Serial.println("aa2c");
    if (!rearOBS()) {
      //Serial.println("aa2d");
      rvs(160, 0); // must not use math here because it resets bumped boolean value
      //Serial.println("aa2e");
      delay(100);
      STOP();
    }

    Serial << "BUMP!!! " << endl;
  }
  delay(10);
  //Serial.println("aa3");
  if (noturn && !docking && !ignoreobs && !isSelfDriving) {
    int FIRval = frontIR_RVal() + frontIR_LVal() / 2;
    int bumpval = BUMP();
    int fusupval = fusupVAL();
    //Serial.println("aa3b");
    if (FIRval > 500)
    {
      Serial.print("Auto Front Obs Prevention CAUSE = ");
      Serial.print("frontIRVal = "); Serial.println(FIRval);
      rvs(MEDIUM, 5);
    }
    else if (bumpval)
    {
      //Serial.println("aa3c");
      Serial.print("Auto Front Obs Prevention CAUSE = ");
      Serial.print("BUMP = "); Serial.println(bumpval);
      rvs(MEDIUM, 5);
    }
    else if (fusupval < 40)
    {
      //Serial.println("aa3d");
      Serial.print("Auto Front Obs Prevention CAUSE = ");
      Serial.print("fusupVAL = "); Serial.println(fusupval);
      if (fusupVAL() < 40)
      {
        rvs(MEDIUM, 5);
      }
      else
      {
        Serial.println("FALSE FUS POSITIVE");
      }
    }
    //Serial.println("aa4");
    if (rearOBS()) {
      Serial.println("Auto Rear Obs Prevention");
      fwd(MEDIUM, 0);
      delay(100);
      STOP();
    }
  }
  //Serial.println("aa5");
  stuck();
  delay(10);
  if (GotStuck && check && mvt && !ignoreobs) {
    simpleSTOP(); // without resetting values
    Serial << "STUCK!!! " << endl;
  }
  //Serial.println("aa6");
  delay(10);
}

void MainFunctions() {
  //Serial.println("aa7");
  _server.handleClient();
  //Serial.println("aa8");
  st::Everything::run();
  //Serial.println("aa9");

  if ((MvtState == "Left" || MvtState == "Right") && millis() - MvtStateMillis > 400)
  {
    STOP();
  }
  if (doReset) {
    Serial << "RESET REQUESTED" << endl;
    if (millis() - RstCtDwnStart > resetDelay) { // this delay allows for webserver to finish working before Reset() is called
      Reset(); // reset requested
    }
  }
  //Serial.println("aa10");
  if (docked && millis() > timeCountToPwOff + DelayMill && !poweroffDONE) {

    //meboOff();// just turning off mebo's cam and built-in controller
    poweroffDONE = true;
    // ST will take care of turning off Main Power
  }
  //Serial.println("aa11");
  if (runIRreceiver && !allSet) {
    Serial << "runIRreceiver ACTIVATED! ----------------------------------------------" << endl;
    irrecv.enableIRIn(); // Start the receiver
    allSet = true; // prevent running this function again
  }
  if (runIRreceiver) {
    StationInSight();
  }
  //Serial.println("aa12");
  if (millis() - previousMillisBlink > 3000 && !noblink) {
    previousMillisBlink = millis();
    if (STOPALL) {
      Blink(5, 50);
    }
    else {
      Blink(1, 100);
      Serial.println("loop");
      TimeInfos();
      if (logs) {
        log();
      }
    }
  }
  //Serial.println("aa13");
}

void log() {
  //runIRreceiver = true;

  //retrieveLineFwd();



  if (millis() - lastMillis > 1000) {
    Blink(5, 30);
    lastMillis = millis();
    //    Serial.print("SeeLineRightRear() = "); Serial.println(SeeLineRightRear());
    //      Serial.print("SeeLineLeftRear() = "); Serial.println(SeeLineLeftRear());
    //      Serial.println("-------------------------------------------------------------");
    //      Serial.print("SeeLineRightFront() = "); Serial.println(SeeLineRightFront());
    //      Serial.print("SeeLineLeftFront() = "); Serial.println(SeeLineLeftFront());
    //      Serial.println("-------------------------------------------------------------");
    //
    Serial.print("fusupVAL_L() = "); Serial.println(fusupVAL_L());
    Serial.print("fusupVAL_R() = "); Serial.println(fusupVAL_R());
    //
          Serial.print("fusupVAL() = "); Serial.println(fusupVAL());
//    //
//          Serial.print("frontIR_LVal() = "); Serial.println(frontIR_LVal());
//          Serial.print("frontIR_RVal() = "); Serial.println(frontIR_RVal());
    
          Serial.print("fusLVAL() = "); Serial.println(fusLVAL());
    
          Serial.print("fusRVAL() = "); Serial.println(fusRVAL());
//    
//          Serial.print("leftIRVal() = "); Serial.println(leftIRVal());
//    
//          Serial.print("rightIRVal() = "); Serial.println(rightIRVal());
//          Serial.print("rearOBS() = "); Serial.println(rearOBS());
    //
    //    Serial.println("VVVVVVVVVVVVVVVVVV");
    //    Serial.print("Tensions is: "); Serial.print(VoltsCheckHasPower()); Serial.println(" Volts");
    //    Serial.print("Has Power? "); Serial.println(haspower());
    //    Serial.println("//////////////////");

    //    StationInSight();

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

void TimeInfos()
{
  unsigned long totalTimeSeconds = previousMillisBlink / 1000;
  unsigned long totalTimeMinutes = previousMillisBlink / 1000 / 60;
  unsigned long TotalTimeHours = previousMillisBlink / 1000 / 60 / 60;
  unsigned long TotalTimeDays = previousMillisBlink / 1000 / 60 / 60 / 24;

  unsigned long displaySeconds = (previousMillisBlink - (totalTimeMinutes * 60 * 1000)) / 1000;
  unsigned long displayMinutes = (previousMillisBlink - (TotalTimeHours * 1000 * 60 * 60)) / 1000 / 60;
  unsigned long displayHours = (previousMillisBlink - (TotalTimeDays * 1000 * 60 * 60 * 24)) / 1000 / 60 / 60;
  unsigned long dispalyDays = TotalTimeDays;


  Serial.println("LOOP time = " + String(elapsed) + "ms");
  Serial.print("Time since last boot = ");
  if (dispalyDays == 1)
  {
    Serial.print(dispalyDays); Serial.print(" day ");
  }
  else if (dispalyDays > 1)
  {
    Serial.print(dispalyDays); Serial.print(" days ");
  }
  if (displayHours == 1)
  {
    Serial.print(displayHours); Serial.print(" hour " );
  }
  else if (displayHours > 1)
  {
    Serial.print(displayHours); Serial.print(" hours " );
  }
  if (displayMinutes == 1)
  {
    Serial.print(displayMinutes); Serial.print(" minute " );
  }
  else if (displayMinutes > 1)
  {
    Serial.print(displayMinutes); Serial.print(" minutes " );
  }
  if (displaySeconds == 1)
  {
    Serial.print(displaySeconds); Serial.println(" second");
  }
  else if (displaySeconds > 1)
  {
    Serial.print(displaySeconds); Serial.println(" seconds");
  }
}
