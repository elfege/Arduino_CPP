/* IDENTICAL TO THE OTHER WINDOW USING SAME THREADED ROD BASED ACTUATOR */
void Open() {

  // ULTRASLOW = ULTRASLOW_RESTORE; // for now we reset this value every time, learning process not being satisfactory

  lastOperation = 1;  // saveToRTC() called in safety()

  lastCmd = "open";

  boolean wasNotAlreadyFullyOpen = !isFullyOpen();  // prevent redundant pressure release at every command if already fully open

  if (!opening) {
    
    term.println("OPENING");
    updateHub("switch on");
    updateHub("window opening");

    opening = true;
    operationStartStamp = millis();                    // for the learning process, this must not change
    unsigned long operationStartStampTEMP = millis();  // for the debug counter, this will change so can't be used as learning reference
    closing = false;
    stopped = false;
    modifRequested = false;
    previousMillisMotors = millis();  // store time of last execution

    dir = "open";
    Move(dir);
    // starting push at SLOW speed instead of default ULTRASLOW

    int START_SPEED = !isOpen() ? LEARNING_SPEED_LIMIT + 100 : LEARNING_SPEED_LIMIT;
    analogWrite(pwm, START_SPEED);
    boolean wasclosed = false;
    unsigned long Start = millis();

    if (!isOpen()) {
      term.println("Waiting for state change");

      while (!isOpen() && !stopped && millis() - Start < 6000)  // when closed it'll need some extra force to pull the window out of its frame
      {
        wasclosed = true;
        backOffFromLimitSwitch("opening")
      }
    }
    
    // resume normal opening operation
    analogWrite(pwm, ULTRASLOW);
    int seconds = 0;
    unsigned long lastChange = millis();
    unsigned long secondChange = millis();
    boolean adjusted = false;
    while (!stopped && !isFullyOpen())  //&& stepcount < maxSteps)
    {
      serverAndHub();  // allow to call Stop() or new speed without throwing any time consuming events (thrown from master())
      stepcount++;

      if (millis() - operationStartStampTEMP >= 5000) {
        seconds += (millis() - operationStartStampTEMP) / 1000;
        term.println(String(seconds) + " seconds passed since cmd");
        if (!doneLearning && (millis() - lastChange > 120000 && !adjusted) || (millis() - secondChange > 60000 && adjusted)) {
          adjusted = true;
          ULTRASLOW = ULTRASLOW < LEARNING_SPEED_LIMIT ? ULTRASLOW + 10 : ULTRASLOW;
          term.println("ULTRASLOW is now " + String(ULTRASLOW));
          lastChange = millis();
          secondChange = millis();
        }
        if (!speedOverride) {
          analogWrite(pwm, ULTRASLOW);
        } else {
          analogWrite(pwm, speedVal);
        }
        operationStartStampTEMP = millis();
      }
    }
    speedOverride = false;
    doneLearning = 1;  // permit ULTRASLOW learning only once, otherwise it gets increased at every final closure.

    backOffFromLimitSwitch();

    if (isFullyOpen() && !stopped && wasNotAlreadyFullyOpen) {
      dir = "close";
      Move(dir);  // release pressure on limit switch
      analogWrite(pwm, SLOW);
      Start = millis();
      while (isFullyOpen() && !stopped && millis() - Start < 5000) {
        yield();  // prevent wdt delay crash
        // term.println("Releasing pressure " + String(isFullyOpen()));
      }

      delay(100);  // let it run a bit more away from the switch
      backOffFromLimitSwitch();
      delay(100);
      dir = "open";
      Move(dir);  // restore switch
      analogWrite(pwm, ULTRASLOW);
      Start = millis();
      while (!isFullyOpen() && !stopped && millis() - Start < 10000) {
        // yield(); // prevent wdt delay crash
        // term.println("restoring switch " + String(isOpen()));
        analogWrite(pwm, 0);
        delay(100);
        analogWrite(pwm, FAST);
        yield();
        delay(30);
      }
      analogWrite(pwm, 0);
    } else {
      updateHub("skipping pressure release: wasNotAlreadyFullyOpen true");
    }

    analogWrite(pwm, 0);
    term.println("stopped = " + String(stopped) + "   ***********************");
    Stop();
  } else {
    updateHub("ignoring Redundant command..");
  }
  previousMillisMotors = millis();  // store time of last execution
  opening = false;
  speedOverride = false;
}

void Close() {

  // ULTRASLOW = ULTRASLOW_RESTORE; // for now we reset this value every time, learning process not being satisfactory

  lastOperation = 2;  // saveToRTC() done in safety()

  lastCmd = "close";
  updateHub("switch off");  // send a response so it is received through updateHub() method

  boolean wasWellOpen = isOpen();  // prevent redundant pressure release at every command if already closed

  if (!closing)  // prevent it from stopping a closing procedure
  {
    previousMillisMotors = millis();  // store time of last execution
    closingcmdMillis = millis();

    term.println("CLOSING");

    updateHub("switch off");  // for Alexa
    updateHub("window closing");
    opening = false;
    closing = true;
    operationStartStamp = millis();
    unsigned long operationStartStampTEMP = millis();
    stopped = false;

    dir = "close";
    Move(dir);
    // starting push at SLOW speed instead of ULTRASLOW (which used to be default. Now default is SLOW anyway... )
    analogWrite(pwm, LEARNING_SPEED_LIMIT);
    boolean wasFullyOpen = false;
    unsigned long Start = millis();
    while (isFullyOpen() && !stopped && millis() - Start < 4000) {
      wasFullyOpen = true;
      stepcount++;
      yield();  // prevent wdt timeout crash
      serverAndHub();
    }
    if (isFullyOpen()) {
      // if still fully open that means speed is too slow, increase starting speed
      analogWrite(pwm, FAST);
      Start = millis();
      while (millis() - Start < 2000 && !stopped) {
        serverAndHub();
        yield();
      }
    }
    analogWrite(pwm, ULTRASLOW); 

    int seconds = 0;
    term.println("Waiting closed state...");
    unsigned long lastChange = millis();
    unsigned long secondChange = millis();
    boolean adjusted = false;
    analogWrite(pwm, ULTRASLOW);
    while (isOpen() && !stopped)  //&& stepcount < maxSteps)
    {
      stepcount++;
      if (millis() - operationStartStampTEMP >= 5000) {
        seconds += (millis() - operationStartStampTEMP) / 1000;
        term.println(String(seconds) + " seconds passed since cmd");
        if ((millis() - lastChange > 120000 && !adjusted) || (millis() - secondChange > 60000 && adjusted)) {
          adjusted = true;
          ULTRASLOW = ULTRASLOW < LEARNING_SPEED_LIMIT ? ULTRASLOW + 50 : ULTRASLOW;
          term.println("ULTRASLOW is now " + String(ULTRASLOW));
          lastChange = millis();
          secondChange = millis();
        }
        if (!speedOverride) {
          analogWrite(pwm, ULTRASLOW);
        } else {
          analogWrite(pwm, speedVal);
        }

        operationStartStampTEMP = millis();
      }

      serverAndHub();  // allow to call Stop() or new speed without throwing any time consuming events (thrown from master())
      delay(10);
    }
    speedOverride = false;

    backOffFromLimitSwitch();
    while (!isOpen() && !stopped && wasWellOpen) {
      
       // close to restore contact with limit switch
      dir = "close"; // closing
      Move(dir); 
      Start = millis();
      while (isOpen() && !stopped && millis() - Start < 1500) { // time value must remain way under watchdog timeout (about 6s if I remember correctly)
        analogWrite(pwm, 0);
        yield();
        delay(10);
        analogWrite(pwm, SLOW);
        delay(10);
      }
      simpleStop()
    } else {
      updateHub("skipping pressure release: wasWellOpen false");
    }
    // term.println("stopped = " + String(stopped) + "   ***********************");
    
    Stop();
  } else {
    updateHub("ignoring Redundant command..");
  }
  closing = false;
  speedOverride = false;
}

void calibrate() {
  term.print("CALIBRATING");
  while (isOpen()) {
    simpleClose();
  }
  term.println("CALIBRATION DONE!");
  stepcount = 0;
}
void simpleClose() {
  lastCmd = "close";
  dir = "close";
  analogWrite(pwm, speedVal);
  Move(lastCmd);
}

void Move(String dir) {
  if (dir == "open") {
    digitalWrite(C, 0);
    digitalWrite(D, 1);
  } else if (dir == "close") {
    digitalWrite(C, 1);
    digitalWrite(D, 0);
  } else {
    term.println("**************************UNDEFINED DIR!**************************");
  }
}

void backOffFromLimitSwitch(String operation) {

   
    if (operation == "opening" || operation = "backoofB4Closing")  
    {
      while (!isOpen() && !stopped && millis() - Start < 1500) {
        Move("open")
        analogWrite(pwm, SLOW);
        delay(5); // by small increments
        simpleStop()
        serverAndHub(); 
      }
      simpleStop()
      
    } else if (operation == "closing" || operation = "backoofB4FullyOpen")
    {
      while (isFullyOpen() && !stopped && millis() - Start < 1500) {
        Move("close") // back away from fullyOpen limit switch
        analogWrite(pwm, SLOW);
        delay(5); // by small increments
        simpleStop()
        serverAndHub(); 
      }
      simpleStop()
    }
    Stop()
}

void Stop() {

  analogWrite(pwm, 0);
  digitalWrite(C, 0);
  digitalWrite(D, 0);

  lastOperation = 0;  // saveToRTC() done in safety()

  lastCmd = "stop";
  closing = false;
  opening = false;
  stopped = true;
  //  term.println("*****************************************STOP**************************************");
}
void simpleStop() {
  digitalWrite(C, 0);
  digitalWrite(D, 0);
}

boolean isOpen() {                          // limit switch located at the end of the track
  return digitalRead(contact_sensor_open);  // if 1, then switch is released; return open true (window is open but not necessarily fully open)
}
boolean isFullyOpen() {                           // limit switch located near the motor
  return !digitalRead(contact_sensor_fullyOpen);  // if 0, then switch is pressed; return true (window fully open)
}

void Refresh() {

  int refresh_interval = 60000

    if (Millis() - last_refresh_millis > refresh_interval) {
    last_refresh_millis = Millis()
                            term.println("REFRESH CALLED!");
    String var = "ERROR";
    String var2 = "";

    if (!isOpen() && !closing && !opening) {
      term.println("reporting contact closed");
      var = "contact closed";
      smartthing.send(var);
      term.println("reporting window closed");
      var2 = "window closed";
    } else if (isOpen() && !closing && !opening) {
      term.println("reporting contact open");
      var = "contact open";
      smartthing.send(var);
      term.println("reporting window open");
      var2 = "window open";
    } else if (!isOpen() && opening) {
      term.println("reporting contact closed");
      var = "contact closed";
      smartthing.send(var);
      term.println("reporting window closed and opening");
      var2 = "window closed_BUT_Opening";
    }
    // if open,is it open AND opening ?
    else if (isOpen() && opening) {
      term.println("reporting contact open");
      var = "contact open";
      smartthing.send(var);
      term.println("reporting window opening");
      var2 = "window opening";
    }
    // or is it open AND closing ?
    else if (isOpen() && closing) {
      term.println("reporting contact open");
      var = "contact open";
      smartthing.send(var);
      term.println("reporting window closing");
      var2 = "window closing";
    }

    if (stopped) {
      term.println("reporting window stopped");
      var = "window stopped";
    }

    updateHub(var2);
    delay(100);
    RefreshSpeed();

    windowLastState = isOpen() ? "windows is open" : "window is closed";  // not the same syntax than hub event, we want the verb...
  }
  else {
    term.println("Last refresh was less than " + String(refresh_interval / 1000) seconds ago.Skipping...)
  }
}

void RefreshSpeed() {
  int newSpeed = map(speedVal, ULTRASLOW, FAST, 1, 100);
  String var = "level " + String(newSpeed);
  updateHub(var);
  updateHub("10 bits speed value = " + String(speedVal));
}

void updateHub(String var)  // called within Arduino C
{
  _server.send(200, "text/html", var);
  term.println(var);
  if (logs)
    term.println(var + " at " + String(TimeInfos()));

  if (var != previousDebugDataCmd && var != previousDebugDataCmdBits && previousDebugDataCmd != var) {

    if (var.startsWith("8 bits")) {
      previousDebugDataCmdBits = var;
    } else if (var.startsWith("'level")) {
      previousDebugDataCmdLevel = var;
    } else {
      previousDebugDataCmd = var;
    }
    // build the debug data string with timing values
    debugData.concat(var + " at " + String(TimeInfos()) + "\n");
  }

  // if var contains a ST's qualifying event attribute's name
  if (var.startsWith("switch") || var.startsWith("window") || var.startsWith("contact") || var.startsWith("level") || var.startsWith("boot OK")) {

    if (var != previousDebugDataCmd) {
      previousDebugDataCmd = var
      smartthing.send(var);
      delay(100);
      String ss = "'" + String(var) + "'" + " SENT TO ST";
      if (ss != previousDebugDataCmd_ss) {
        previousDebugDataCmd_ss = ss;
        debugData.concat(ss + " at " + String(TimeInfos()) + "\n");
      }
    }
  }
}

void getDataDebug()  // called by JS
{
  // send last updated value in getDebug()

  _server.send(200, "text/html", debugData);
  if (debugData.length() > 50000) {
    debugData = "";
    previousDebugData = "";
    previousDebugDataCmd = "";
    previousDebugDataCmdBits = "";
    previousDebugDataCmdLevel = "";
    logs = false;  // stop sensor logs
    updateHub("DATA RESET");
  }
}
void getMac() {
  String var = WiFi.macAddress();
  _server.send(200, "text/html", var);
}
void getIP() {
  String var = WiFi.localIP().toString();
  _server.send(200, "text/html", var);
}
void getSSID() {
  String var = String(WiFi.SSID());
  _server.send(200, "text/html", var);
}
void getRSSI() {
  String var = String(WiFi.RSSI());
  _server.send(200, "text/html", var);
}

void getSpeed() {
  int newSpeed = map(speedVal, ULTRASLOW, FAST, 1, 100);  // must use ULTRASLOW instead of ULTRASLOW as default minimal value
  String var = String(newSpeed);
  _server.send(200, "text/html", var);
  //  delay(1000);
  var = "level " + String(newSpeed);
  // updateHub(var); ;
}

void Blink(int times, int lapse) {
  int c = 0;
  while (c < times) {
    digitalWrite(LED, 1);
    delay(lapse);
    digitalWrite(LED, 0);
    delay(lapse);
    c++;
  }
}

//*****************************************************************************

void Reset() {
  //  ESP.restart();
  hardReset();
}

void hardReset() {
  ESP.reset();  // esp.reset is hard reset
}

//*****************************************************************************
// SMARTTHINGS CALLOUT  | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                      V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void messageCallout(String message) {

  // Blink(5, 50);

  String mess = "Received message: '" + String(message) + "' ";

  lastMessage = message;  // update global
  term.println(mess);

  // check if this message is a digit
  if (isDigit(message.charAt(0))) {  //   -> TRUE
    ///  see if there is a request for a different speed
    String thisMessage = message;
    char this_char[thisMessage.length() + 1];
    thisMessage.toCharArray(this_char, sizeof(this_char));
    int my_integer_data = atoi(this_char);
    // if(my_integer_data >= 0){
    term.print("my_integer_data = ");
    term.println(my_integer_data);

    // now update and convert this integer into a pwm speed value
    speedVal = map(my_integer_data, 1, 100, ULTRASLOW, FAST);  // must use ULTRASLOW instead of ULTRASLOW as default minimal value

    speedOverride = true;
    analogWrite(pwm, speedVal);

    // modulate speed accordingly in case it's already running
    modifRequested = true;
    if (speedVal == FAST) {
      emergencySpeed = true;
      emergencySpeedMillis = millis();
    }
    lastSpeedVal = speedVal;

    // inform ST that this value has been taken into consideration
    // reconvert to percentage
    int newSpeed = map(speedVal, ULTRASLOW, FAST, 1, 100);  // must use ULTRASLOW instead of ULTRASLOW as default minimal value
    String var = "level " + String(newSpeed);
    updateHub(var);
    ;
  } else if (message.equals("open")) {
    term.println("opening");
    Open();
  } else if (message.equals("close")) {
    term.println("closing");
    Close();
  } else if (message.equals("stop")) {
    term.println("stopping");
    Stop();
  } else if (message.equals("refresh")) {
    term.println("refreshing");
    Refresh();
  } else if (message.equals("reset")) {
    term.println("reseting");
    Reset();
  }
}

void log() {

  term.println("stopped  = " + String(stopped));
  term.println("STOP  = " + String(STOP));
  term.println("StopAll  = " + String(StopAll));
  term.println("closing  = " + String(closing));
  term.println("opening  = " + String(opening));
  term.println("modifRequested  = " + String(modifRequested));
  term.println("wasclosed  = " + String(wasclosed));
  term.println("NoTouchOpen  = " + String(NoTouchOpen));
  term.println("watchdogInitOk  = " + String(watchdogInitOk));
  term.println("consideredClosed  = " + String(consideredClosed));
  term.println("emergency  = " + String(emergency));
  term.println("logs  = " + String(logs));
  term.println("emergencySpeed  = " + String(emergencySpeed));
  term.println("userSpeedInputRequested  = " + String(userSpeedInputRequested));
  term.println("finalPush  = " + String(finalPush));
  term.println("speedAdjusted  = " + String(speedAdjusted));
  term.println("openingUpdateDone  = " + String(openingUpdateDone));
  term.println("closingUpdateDone  = " + String(closingUpdateDone));
  term.println("lastClosingReportWindowWasStillOpen  = " + String(lastClosingReportWindowWasStillOpen));
  term.println("lastOpeningReportWindowWasStillClosed  = " + String(lastOpeningReportWindowWasStillClosed));
  term.println("ntpFailed  = " + String(ntpFailed));
  term.println("stopNtp  = " + String(stopNtp));
  term.println("DELAY_REFRESH  = " + String(DELAY_REFRESH));
  term.println("previousMillis  = " + String(previousMillis));
  term.println("previousMillisReset  = " + String(previousMillisReset));
  term.println("ResetTime  = " + String(ResetTime));
  term.println("previousMillisBlink  = " + String(previousMillisBlink));
  term.println("LedOn  = " + String(LedOn));
  term.println("previousMillisMotors  = " + String(previousMillisMotors));
  term.println("previousMillisContactClosed  = " + String(previousMillisContactClosed));
  term.println("previousMillisContactOpen  = " + String(previousMillisContactOpen));
  term.println("previousMillisEvts  = " + String(previousMillisEvts));
  term.println("previousMillisST  = " + String(previousMillisST));
  term.println("previousMillisServer  = " + String(previousMillisServer));
  term.println("finalPushMillis  = " + String(finalPushMillis));
  term.println("loopTime  = " + String(loopTime));
  term.println("lastSpeedAdjustment  = " + String(lastSpeedAdjustment));
  term.println("lastBoot  = " + String(lastBoot));
  term.println("lastTimeDisplay  = " + String(lastTimeDisplay));
  term.println("closingcmdMillis  = " + String(closingcmdMillis));
  term.println("previousMillisOpen  = " + String(previousMillisOpen));
  term.println("emergencySpeedMillis  = " + String(emergencySpeedMillis));
  term.println("lastSpeedRequestMillis  = " + String(lastSpeedRequestMillis));
  term.println("RequestDelay  = " + String(RequestDelay));
  term.println("timeLastClosingUpdate  = " + String(timeLastClosingUpdate));
  term.println("timeLastOpeningUpdate  = " + String(timeLastOpeningUpdate));
  term.println("lastNTP  = " + String(lastNTP));
  term.println("lastLogMillis = " + String(lastLogMillis));
  term.println("millisBoot  = " + String(millisBoot));
  term.println("debugData  = " + String(debugData));
  term.println("previousDebugData  = " + String(previousDebugData));
  term.println("previousDebugDataCmd  = " + String(previousDebugDataCmd));
  term.println("previousDebugDataCmdBits  = " + String(previousDebugDataCmdBits));
  term.println("previousDebugDataCmdLevel  = " + String(previousDebugDataCmdLevel));
  term.println("timeOfLastBoot  = " + String(timeOfLastBoot));
  term.println("lastMessage  = " + String(lastMessage));
  term.println("timerResult  = " + String(timerResult));
  term.println("lastCmd  = " + String(lastCmd));
  term.println("windowLastState  = " + String(windowLastState));
  term.println("lastBootDate  = " + String(lastBootDate));
  term.println("isDebugEnabled;  = " + String(isDebugEnabled));
  term.println("lastMaxOpDuration  = " + String(lastMaxOpDuration));
  term.println("shortDelayAdjust  = " + String(shortDelayAdjust));
  term.println("speedIncrement  = " + String(speedIncrement));
  term.println("speedAdjustmentOccurences  = " + String(speedAdjustmentOccurences));
  term.println("elapsed  = " + String(elapsed));
  term.println("TOUCH  = " + String(TOUCH));
  term.println("itopen  = " + String(itopen));
  term.println("wtdpin;  = " + String(wtdpin));
  term.println("events  = " + String(events));
  term.println("ntpFailures  = " + String(ntpFailures));
  term.println("ULTRASLOW  = " + String(ULTRASLOW));
  term.println("ULTRASLOW_RESTORE  = " + String(ULTRASLOW_RESTORE));
  term.println("SLOW  = " + String(SLOW));
  term.println("FAST  = " + String(FAST));
  term.println("speedVal  = " + String(speedVal));
  term.println("lastSpeedVal  = " + String(lastSpeedVal));
  term.println("PUSH  = " + String(PUSH));
  term.println("frequency  = " + String(frequency));
}

void updateNtpTime() {

  term.println("UPDATING TIME CLIENT... please wait...");
  timeClient.update();
  unsigned long Start = millis();
  while (!timeClient.updated() && millis() - Start < 2000) {
    delay(1);
  }
  if (!timeClient.updated()) {
    ntpFailures++;
    if (ntpFailures < 10) {
      term.println("******TIME FAILED TO UPDATE FROM NTP SERVER******");
      ntpFailed = true;  // loop will call updateNtpTime() again
    } else {
      term.println("******TOO MANY FAILS - CANCELLING ALL NTP REQUESTS UNTIL RESET******");
      stopNtp = true;
    }
  } else {
    if (ntpFailed)  // if previous attempt failed, notify success
    {
      term.println("******TIME SUCCESSFULY UPDATED FROM NTP SERVER******");
    }
    ntpFailed = false;
  }

  return;

  term.println("UTC : " + timeClient.getFormattedUTCTime());
  term.println("UTC : " + timeClient.getFormattedUTCDateTime());
  term.println("LOC : " + timeClient.getFormattedTime());
  term.println("LOC : " + timeClient.getFormattedDateTime());
  term.println("UTC EPOCH : " + String(timeClient.getUTCEpochTime()));
  term.println("LOC EPOCH : " + String(timeClient.getEpochTime()));

  // Function test
  // Without leading 0
  term.println(String("UTC : ") + timeClient.getUTCHours() + ":" + timeClient.getUTCMinutes() + ":" + timeClient.getUTCSeconds() + " " + timeClient.getUTCDoW() + " " + timeClient.getUTCDay() + "/" + timeClient.getUTCMonth() + "/" + timeClient.getUTCYear() + " or " + timeClient.getUTCDay() + " " + timeClient.getUTCMonthStr() + " " + timeClient.getUTCYear());
  // With leading 0
  term.println(String("UTC : ") + timeClient.getUTCStrHours() + ":" + timeClient.getUTCStrMinutes() + ":" + timeClient.getUTCStrSeconds() + " " + timeClient.getUTCDoW() + " " + timeClient.getUTCDay() + "/" + timeClient.getUTCMonth() + "/" + timeClient.getUTCYear() + " or " + timeClient.getUTCDay() + " " + timeClient.getUTCMonthStr() + " " + timeClient.getUTCYear());
  // Without leading 0
  term.println(String("LOC : ") + timeClient.getHours() + ":" + timeClient.getMinutes() + ":" + timeClient.getSeconds() + " " + timeClient.getDoW() + " " + timeClient.getDay() + "/" + timeClient.getMonth() + "/" + timeClient.getYear() + " or " + timeClient.getDay() + " " + timeClient.getMonthStr() + " " + timeClient.getYear());
  // With leading 0
  term.println(String("LOC : ") + timeClient.getStrHours() + ":" + timeClient.getStrMinutes() + ":" + timeClient.getStrSeconds() + " " + timeClient.getDoW() + " " + timeClient.getDay() + "/" + timeClient.getMonth() + "/" + timeClient.getYear() + " or " + timeClient.getDay() + " " + timeClient.getMonthStr() + " " + timeClient.getYear());
}