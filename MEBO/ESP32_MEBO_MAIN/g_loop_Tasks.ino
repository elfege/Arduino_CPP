void MainFunctions() {
  digitalWrite(watchDogPin, 0);
//  UartReceivWTD();
  
  loopTime = millis(); // needs to be set here too because it's called by CPU_1 even when driving
  _server.handleClient();
  term.handleClient();// WiFi terminal
  termSteps();
  

  //st::Everything::run(); // useless for now and poses compatibility issues with WiFi Terminal (only with this sketch though)

  if ((MvtState == "Left" || MvtState == "Right") && millis() - MvtStateMillis > 500)
  {
    STOP();
  }
  if (doReset) {
    if (millis() - RstCtDwnStart > resetDelay) { // this delay allows for webserver to finish working before Reset() is called
      Reset(); // reset requested
    }
  }
  //term.println("aa10");
  if (docked && millis() > timeCountToPwOff + DelayMill && !poweroffDONE) {

    //meboOff();// just turning off mebo's cam and built-in controller
    poweroffDONE = true;
    // ST will take care of turning off Main Power
  }
  //term.println("aa11");
  if (runIRreceiver && !allSet) {
    term.println("runIRreceiver ACTIVATED! ----------------------------------------------");
    irrecv.enableIRIn(); // Start the receiver
    allSet = true; // prevent running this function again
  }
  if (runIRreceiver) {
    StationInSight();
  }
  //term.println("aa12");
  if (millis() - previousMillisBlink > 3000 && !noblink) {
    previousMillisBlink = millis();
    if (STOPALL) {
      Blink(5, 50);
    }
    else {
      Blink(1, 100);
      //term.println("loop");
      //TimeInfos();
    }
  }
  if (millis() - previousMillisLogs > 3000 && logs && logDone) {
    previousMillisLogs = millis();
    debug();
  }
  
  //term.println("aa13");
  digitalWrite(watchDogPin, 1);
  delay(1);// give time to core0 to detect the change of state
}
void termSteps() {
  char c;
  String cmd = "";

  if (term.available())
  {
    while (term.available())
    {
      c = term.read();
      cmd += c;
    }
    term.print("Ok I received : "); term.print(cmd);
    term.println();
    if (cmd == "reset")
    {
      term.println("RESETING ESP NOW");
      // give time for the terminal to display this message
      unsigned long s = millis();
      while (millis() - s < 100)
      {
        MainFunctions();
      }
      Reset();
    }
  }
}

void respondToWatchDog(){
  // watchdog run by AtMega
  
}
void checkWiFi() {
  if (WiFi.status() != WL_CONNECTED && !wifiLostAware)
  {
    Serial.println("******************* LOST WIFI CONNEXION ********************* ");
    WiFiLostMillis = millis();
    wifiLostAware = true;
  }
  if (millis() - WiFiLostMillis > 5000 && wifiLostAware)
  {
    WiFiConnect();
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("******************* ESP DID NOT RECONNECT ********************* ");
      Reset();
    }
    else
    {
      Serial.println("************************************************** ");
      Serial.println("       ESP SUCCESSFULLY RECONNECTED TO WIFI!       ");
      Serial.println("************************************************** ");
    }
    wifiLostAware = false;
  }
}
void debug() {

  logDone = false; // needed to prevent Pause() from getting into an infinite feedback loop
  term.println("START LOGS");
  //runIRreceiver = true;
  Blink(5, 30);
  term.print("analogRead(LineLeftRear) = "); term.println(analogRead(LineLeftRear));
  term.print("analogRead(LineRightRear) = "); term.println(analogRead(LineRightRear));

  Pause();

  term.println("-------------------------------------------------------------");
  term.print("SeeLineRightRear() = "); term.println(SeeLineRightRear());
  term.print("SeeLineLeftRear() = "); term.println(SeeLineLeftRear());
  term.println("-------------------------------------------------------------");
  term.print("SeeLineRightFront() = "); term.println(SeeLineRightFront());
  term.print("SeeLineLeftFront() = "); term.println(SeeLineLeftFront());
  term.println("-------------------------------------------------------------");
  Pause();
  term.println("-------------------------------------------------------------");
  term.print("fusupVAL_L() = "); term.println(fusupVAL_L());
  term.print("fusupVAL_R() = "); term.println(fusupVAL_R());
  Pause();
  term.print("fusupVAL() = "); term.println(fusupVAL());
  term.print("frontIR_LVal() = "); term.println(frontIR_LVal());
  term.print("frontIR_RVal() = "); term.println(frontIR_RVal());

  term.print("leftIRVal() = "); term.println(leftIRVal());
  term.print("rightIRVal() = "); term.println(rightIRVal());
  term.print("rearOBS() = "); term.println(rearOBS());
  term.println("-------------------------------------------------------------");
  term.print("RRIRVal() = "); term.println(RRIRVal());
  term.print("RLIRVal() = "); term.println(RLIRVal());

  term.println("-------------------------------------------------------------");
  Pause();

  term.print("fusLVAL() = "); term.println(fusLVAL());
  term.print("fusRVAL() = "); term.println(fusRVAL());
  Pause();
  term.println("-------------------------------------------------------------");

  term.println("Bump ? " + String(BUMP() ? "true" : "false"));
  term.println("-------------------------------------------------------------");
  Pause();
  term.print("Tensions is: "); term.print(VoltsCheckHasPower()); term.println(" Volts");
  term.print("Has Power? "); term.println(haspower());
  term.println("-------------------------------------------------------------");


  //term.println("-------------------------------------------------------------");
  //    StationInSight();
  //term.println("-------------------------------------------------------------");

  term.println("END OF LOGS");
  term.println("");
  term.println("");
  logDone = true;
}

void Pause() {
  unsigned long s = millis();
  while (millis() - s > 500)
  {
    MainFunctions();
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


  term.println("LOOP time = " + String(elapsed) + "ms");
  term.print("Time since last boot = ");
  if (dispalyDays == 1)
  {
    term.print(dispalyDays); term.print(" day ");
  }
  else if (dispalyDays > 1)
  {
    term.print(dispalyDays); term.print(" days ");
  }
  if (displayHours == 1)
  {
    term.print(displayHours); term.print(" hour " );
  }
  else if (displayHours > 1)
  {
    term.print(displayHours); term.print(" hours " );
  }
  if (displayMinutes == 1)
  {
    term.print(displayMinutes); term.print(" minute " );
  }
  else if (displayMinutes > 1)
  {
    term.print(displayMinutes); term.print(" minutes " );
  }
  if (displaySeconds == 1)
  {
    term.print(displaySeconds); term.println(" second");
  }
  else if (displaySeconds > 1)
  {
    term.print(displaySeconds); term.println(" seconds");
  }
}
