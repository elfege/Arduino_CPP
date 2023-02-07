/*NO LONGER STANDARDIZED */
void Open() {
  lastOperation = 1;  // saveToRTC() called in serverAndHubs()
  lastCmd = "open";
  sendHub("switch on");  // send a response so it is received through parse() method
  if (!opening) {
    pwm = motoropen;
    term.print("OPENING START @ ");
    term.println("LOC : " + timeClient.getFormattedTime());
    term.println("OPENING " + timeClient.getFormattedTime() + " - reset_counter = " + String(reset_counter) + " & already learned ? = " + String(openLearn ? "true" : "false") + " successfulOpenLearn ? " + String(successfulOpenLearn ? "true" : "false"));

    sendHub("switch on");  // for alexa
    sendHub("window opening");

    opening = true;
    closing = false;
    stopped = false;
    speedVal = ULTRASLOW;  // restart from base speed
    modifRequested = false;
    previousMillisMotors = millis();  // store time of last execution

    simpleOpen();

    analogWrite(pwm, ULTRASLOW + 100);  // push is 0 at first and will be adjusted if needed (in the while loop just below)
    unsigned long Start = millis();
    unsigned long lastPushIncrease = millis();
    while (millis() - Start < 120000 && !stopped && !StopAll && contactClosed)  // always starter push to give it inertia due to very low pwm + wait until open
    {
      if (millis() - Start > 50000 && contactClosed && millis() - lastPushIncrease > 10000) {
        int prev = PUSH;
        PUSH += 10;
        analogWrite(pwm, ULTRASLOW + PUSH);
        lastPushIncrease = millis();
        term.println("increasing PUSH from " + String(prev) + " to " + String(PUSH));
      }
      serverAndHub();
    }
    if (!speedOverride) {
      analogWrite(pwm, ULTRASLOW);  // default cruise speed
    } else {
      analogWrite(pwm, speedVal);
    }

    delay(500);  // prevent arm mvt false positive

    if (contactClosed) {
      /************************************************************************************/
      // LEARNING METHOD 1
      /************************************************************************************/
      //learnMethod_1(successfulOpenLearn);

      /************************************************************************************/
      // LEARNING METHOD 2, in case it gets stuck on the way
      /************************************************************************************/
      //learnMethod_2(openLearn, successfulOpenLearn); //// WORKS ONLY FOR CLOSING METHOD ! NO SENSOR TO END THE LOOP WHEN OPENING ! This would continuously raise speed...
    } else {
      term.println("WINDOW ALREADY OPEN ; NOT LEARNING");
    }


    if (!contactClosed) {
      sendHub("contact open");
      sendHub("window open");
      successfulOpenLearn = 1;
    }
    //Stop("OPEN"); Not necessary with actuators if we use the whole length. If you need to call stop here, beware of resetting EMstop, see signalFailure() beforehand, though.

    term.println("10 bits speed value = " + String(speedVal));
    term.println("END OF OPENING");
    //sendHub("END OF open() scope");
  } else {
    sendHub("ignoring Redundant command..");
  }
  previousMillisMotors = millis();  // store time of last execution
  opening = false;
  donotAcceptCmdsFromHubs = 0;
  speedOverride = false;
}
void Close() {
  pwm = motorclose;

  lastOperation = 2;  // saveToRTC() called in serverAndHubs()

  lastCmd = "close";
  sendHub("switch off");  // send a response so it is received through parse() method
  // to prevent it from stopping a closing procedure

  if (contactClosed) {
    term.println("ALREADY CLOSED");
    simpleClose();  // give extra push in case this is the entended request.
    analogWrite(pwm, FAST);
    return;
  }
  if (!closing) {

    term.print("CLOSING START @ ");
    term.println("LOC : " + timeClient.getFormattedTime());
    term.println("CLOSING " + timeClient.getFormattedTime() + " - reset_counter = " + String(reset_counter) + " & already learned ? = " + String(closeLearn ? "true" : "false") + " successfulCloseLearn ? " + String(successfulCloseLearn ? "true" : "false"));

    closingcmdMillis = millis();
    sendHub("switch off");  // for Alexa
    sendHub("window closing");

    closing = true;
    stopped = false;  // reset this value
    opening = false;
    modifRequested = false;
    NoTouchOpen = true;  // so touch doesn't reopen, but stops only

    simpleClose();

    if (!speedOverride) {
      analogWrite(pwm, ULTRASLOW + PUSH);  // push value is constant for closing (no fullyopen sensor on this device, can't learn from here as long as shaft motion can't be properly detected).
    } else {
      analogWrite(pwm, speedVal);
    }
    unsigned long Start = millis();
    unsigned long lastPushIncrease = millis();
    while (millis() - Start < 4000 && !stopped && !StopAll && isOpen()) {
      serverAndHub();
    }
    if (!speedOverride) {
      analogWrite(pwm, ULTRASLOW);  // default cruise speed
    } else {
      analogWrite(pwm, speedVal);
    }

    /************************************************************************************/
    // LEARNING METHOD 1
    /************************************************************************************/
    //learnMethod_1(successfulCloseLearn);

    /************************************************************************************/
    // LEARNING METHOD 2, in case it gets stuck on the way
    /************************************************************************************/
    //learnMethod_2(closeLearn, successfulCloseLearn);

    // once successful, those methods might be skipped, so in the closing case, we need to wait for contact
    unsigned long c = millis();
    unsigned long relativeStart = millis();
    Start = millis();

    while (!contactClosed && !stopped && !EMstop && millis() - Start < 1200000)  // 1,200,000 millis = 20 min
    {
      if (millis() - relativeStart > operationTime)  // 600,000
      {
        relativeStart = millis();
        ULTRASLOW += 50;  // seems it's not moving, so go a bit faster
        term.println("MIN SPEED VAL IS NOW: " + String(ULTRASLOW));
        if (!speedOverride) {
          analogWrite(pwm, ULTRASLOW);
        } else {
          analogWrite(pwm, speedVal);
        }
      }
      if (millis() - c > 5000) {
        term.print("closing... Elapsed time: ");
        term.print(String((millis() - Start) / 1000));
        term.print(" seconds and at a speed val of: ");
        term.println(String(speedVal));
        c = millis();
      }
      serverAndHub();
    }
    if (!contactClosed) {
      ULTRASLOW += 100;
      if (!speedOverride) {
        analogWrite(pwm, SLOW);
      } else {
        analogWrite(pwm, speedVal);
      }
    }

    term.println("CLOSING METHOD ENDED");
    donotAcceptCmdsFromHubs = 0;

    if (contactClosed) {
      sendHub("contact closed");
      sendHub("window closed");
      term.println("********* CLOSING SUCCESS!!!************");
      successfulCloseLearn = 1;
      if (!speedOverride) {
        analogWrite(pwm, ULTRASLOW + PUSH);  // final push
      } else {
        analogWrite(pwm, speedVal);
      }
    }

    //Stop("CLOSE"); Not necessary with actuators if we use the whole length. If you need to call stop here, beware of resetting EMstop, see signalFailure() beforehand, though.

    term.println("10 bits speed value = " + String(speedVal));
    term.println("END OF CLOSING");
  } else {
    sendHub("ignoring Redundant command..");
  }
  previousMillisMotors = millis();  // store time of last execution
  closing = false;
  donotAcceptCmdsFromHubs = 0;
  speedOverride = false;
}
void learnMethod_1(int successfulLearn) {
  term.println("METHOD 1 START");
  interruptChanged = false;  // will probably have been set to true by the above "analogWrite(pwm, ULTRASLOW + PUSH)" call

  term.println("EMstop = " + String(EMstop ? "true" : "false"));


  unsigned long s = millis();
  unsigned int ti = millis();
  if (armMovImplemented) {
    if (successfulLearn != 1) {
      term.println("INCREASING SPEED UNTIL ARM STARTS MOVING");
    } else {
      term.println("PREVIOUS LEARN SUCCESSFUL, GOING WITH CURRENT RTC VARS");
      return;
    }
    while (successfulLearn != 1 && !interruptChanged && millis() - s < 240000 && !stopped && !StopAll && !EMstop && method_1_Failed != 1 && !modifRequested)  // as long as arm is not moving, increase speed
    {
      if (closing && contactClosed) {
        term.println("CONTACT IS CLOSED, METHOD 1 STOPPED");
        return;
      }
      donotAcceptCmdsFromHubs = 1;
      if (millis() - ti > 10000) {
        int pr = ULTRASLOW;
        ULTRASLOW += 5;
        if (!speedOverride) {
          term.println("ULTRASLOW increased from " + String(pr) + " to " + String(ULTRASLOW));
          analogWrite(
            pwm, ULTRASLOW);
        } else {
          analogWrite(pwm, speedVal);
        }

        speedVal = ULTRASLOW;
        lastSpeedVal = speedVal;
        ti = millis();
      }
      if (ULTRASLOW >= 1023) {
        signalFailure();
      }
      serverAndHub();
    }
  } else {
    term.println("METHOD 1 SENSOR NOT YET IMPLEMENTED OR NOT FUNCTIONAL ON THIS DEVICE");
  }
  if (modifRequested) {
    term.println("MODIF REQUESTED");
    donotAcceptCmdsFromHubs = 0;
  }
  if (interruptChanged) {
    term.println("ARM IS MOVING");
    interruptChanged = false;
    sendHub("METHOD1 SUCCESS");
  }
  if (method_1_Failed == 1) {
    term.println("METHOD 1 FAILED SENSOR IS PROBABLY DEFECTIVE OR MOTOR HAS NO POWER");
    sendHub("METHOD1 FAILED");
  }
  term.println("METHOD 1 END");
}
void learnMethod_2(int learn, int successfulLearn) {
  term.println("METHOD 2 START");
  unsigned long c = millis();  // debug counter and ULTRASLOW learning method
  unsigned long UltraSlowSpeedAdjust = millis();
  boolean UltraSlowspeedAdjusted = false;
  int counterTime = method_1_Failed == 1 ? 1000 : 10000;
  counterTime = armMovImplemented ? counterTime : 10000;
  unsigned long Start = millis();
  unsigned long lastMovingTest = millis();
  boolean lastInterruptReset = false;
  boolean isMoving = method_1_Failed == 1 ? false : true;
  boolean interruptReset = false;
  boolean wasclosing = closing ? true : false;
  // THIS METHOD might still be used after successful method 1. If method 1 was succesful, this method will wait a much longer time before adjusting speed.

  // WORKS ONLY FOR CLOSING METHOD ! NO SENSOR TO END THE LOOP WHEN OPENING !
  while (!contactClosed && !opening && !stopped && !StopAll && !EMstop && !modifRequested && millis() - Start < METHOD_2_LONG_DELAY)  // continue to push as long as it's open
  {
    if (closing && contactClosed) {
      term.println("CONTACT IS CLOSED, METHOD 2 ABORTED AND MARKED AS SUCCESSFULL");
      return;
    }

    boolean UltraSlowNeedAdjustment = adjustmentNeeded(learn, successfulLearn, UltraSlowspeedAdjusted, UltraSlowSpeedAdjust, Start);
    donotAcceptCmdsFromHubs = method_1_Failed == 1 || learn == 0 ? 1 : 0;  // refuse external commands ONLY if  method_1_Failed == 1 or successful learn hasn't happened yet

    if (armMovImplemented) {
      if (method_1_Failed != 1 && millis() - lastMovingTest > 40000)  // every X seconds, reset the test, then test again 10 seconds later. ONLY If method 1 didn't fail
      {
        if (!interruptReset) {
          interruptChanged = false;  // every X * 2 reset this boolean to test if the arm is indeed moving
          interruptReset = true;
        } else  // if reset already happened
        {
          if (!interruptChanged) {
            term.println("It looks like the arm is not moving");
            ULTRASLOW += 10;
            isMoving = false;
          } else {
            term.println("Everything seems ok, arm is moving");
            isMoving = true;
          }
          interruptReset = false;
        }
        lastMovingTest = millis();
      }
    } else if (millis() - c > counterTime) {
      term.println("Meth. 2 part 1: ARM MOV. SENSOR NOT IMPLEMENTED YET ON THIS DEVICE (learning may take several minutes)");
    }

    /// METHOD 2 part 2
    if (!isMoving || !armMovImplemented) {
      if (UltraSlowNeedAdjustment) {
        counterTime = 1000;

        term.println("Seems that ULTRASLOW speed is too low. Adjusting accordingly");
        ULTRASLOW += 50;
        UltraSlowspeedAdjusted = true;
        term.println("ULTRASLOW default value is now: " + String(ULTRASLOW));
        if (!speedOverride) {
          analogWrite(pwm, ULTRASLOW);
        } else {
          analogWrite(pwm, speedVal);
        }
        speedVal = ULTRASLOW;
        lastSpeedVal = speedVal;
        if (ULTRASLOW >= 1023) {
          ULTRASLOW = ULTRASLOW_RESTORE;
          term.println("SOMETHING WENT WRONG, WINDOWS DID NOT CLOSE. Check the hardware");
        }
        UltraSlowSpeedAdjust = millis();
      }
    }
    if (millis() - c > counterTime)  // this is just for debug purpose
    {
      term.println("METHOD 2 loop " + String((millis() - Start) / 1000) + "s (MAX=" + String(METHOD_2_LONG_DELAY / 1000) + ")");
      term.println("method_1_Failed: " + String(method_1_Failed));
      term.println("operation type: " + String(closing ? "closing" : "opening"));

      c = millis();
    }
    // resend moror cmd in case a stop came around (in which case this loop would resume anyway but without motors...)
    simpleClose();

    serverAndHub();
    stopped = donotAcceptCmdsFromHubs ? false : stopped;
  }
  if (!modifRequested || EMstop) {  // if EMstop means user was satisfied with current motion. No need to learn more, arm was moving.
    //If modif, means user wasn't satisfied, learning might still be required.
    closeLearn = 1;
  } else if (modifRequested && !EMstop) {
    term.println("SPEED MODIFICATION WAS REQUESTED. LEARNING CANCELED. IT'LL RUN AGAIN AT A LATER TIME. LEARNING BOOLS UNCHANGED.");
  }
  modifRequested = false;
  term.println("METHOD 2 END");
}
void signalFailure() {
  term.println("There was a problem with method 1. Restoring ULTRASLOW default value (" + String(ULTRASLOW_RESTORE) + ")");
  ULTRASLOW = ULTRASLOW_RESTORE;
  if (!speedOverride) {
    analogWrite(pwm, ULTRASLOW);
  } else {
    analogWrite(pwm, speedVal);
  }
  EMstop = true;
  donotAcceptCmdsFromHubs = 0;
  method_1_Failed = 1;  // this means the device will no longer use this method, ever, until next cold reboot
}
boolean adjustmentNeeded(int learn, int successLearn, boolean UltraSlowspeedAdjusted, unsigned long UltraSlowSpeedAdjust, unsigned long Start) {
  if ((learn != 1 || successLearn != 1 || method_1_Failed == 1) && !UltraSlowspeedAdjusted && method_1_Failed == 1) {
    // in any case, if method_1_Failed is NOT TRUE, DO NOT START THIS LEARNING METHOD RIGHT AWAY.
    // but if method 1 failed and this learning hasn't happened before, don't wait, start learning right away.
    return millis() - Start > 0;
  } else if (!UltraSlowspeedAdjusted)  // if a previous adjustment hasn't happened yet and this nor first learn nor method 1 failed, then wait for
  //the entire time the window would need to close before infering that it needs a higher speed.
  {
    return millis() - Start > 4 * 60 * 1000;
  } else if (UltraSlowspeedAdjusted)  // if adjustment already happened, then wait another complete closing cycle before increasing the speed again
  {
    return millis() - UltraSlowSpeedAdjust > 4 * 60 * 1000;
  }
}
ICACHE_RAM_ATTR void handleArmMovInterrupt() {
  // no println, no delay, should always be short
  // otherwise events stack over each other and then the board crashes
  //term.println("************INTERRUPT CHANGE DETECTED*****************");
  interruptChanged = true;
}
ICACHE_RAM_ATTR void handleContactSensor() {

  if (digitalRead(contact_sensor)) {
    //Serial.println("O");
    contactClosed = false;
  } else {
    //Serial.println("C");
    contactClosed = true;
  }
}


void Refresh() {

  term.println("REFRESH CALLED!");
  String var = "ERROR";
  String var2 = "";

  if (!isOpen() && !closing && !opening) {
    term.println("reporting contact closed");
    var = "contact closed";
    sendHub(var);
    term.println("reporting window closed");
    var2 = "window closed";
  } else if (isOpen() && !closing && !opening) {
    term.println("reporting contact open");
    var = "contact open";
    sendHub(var);
    term.println("reporting window open");
    var2 = "window open";
  } else if (!isOpen() && opening) {
    term.println("reporting contact closed");
    var = "contact closed";
    sendHub(var);
    term.println("reporting window closed and opening");
    var2 = "window closed_BUT_Opening";
  }
  // if open,is it open AND opening ?
  else if (isOpen() && opening) {
    term.println("reporting contact open");
    var = "contact open";
    sendHub(var);
    term.println("reporting window opening");
    var2 = "window opening";
  }
  // or is it open AND closing ?
  else if (isOpen() && closing) {
    term.println("reporting contact open");
    var = "contact open";
    sendHub(var);
    term.println("reporting window closing");
    var2 = "window closing";
  }

  if (stopped) {
    term.println("reporting window stopped");
    var = "window stopped";
  }

  sendHub(var2);
  delay(100);
  RefreshSpeed();

  windowLastState = isOpen() ? "windows is open" : "window is closed";  // not the same syntax than hub event, we want the verb...
}


void RefreshSpeed() {
  int newSpeed = map(speedVal, ULTRASLOW, FAST, 1, 100);
  term.println("RefreshSpeed val =" + String(speedVal));
  String var = "level " + String(newSpeed);
  smartthing.send(var);
  term.println("newSpeed val =" + String(var));
  sendHub("10 bits speed value = " + String(speedVal));
}

void sendHub(String var)  // called within Arduino C
{
  _server.send(200, "text/html", var);
  if (millis() - lastHubRefresh > 500) {
    lastHubRefresh = millis();
    term.println(var + " at " + timeClient.getFormattedTime());
    if (var.startsWith("level")) {
      Serial.println("repetitive level event, skipping");
    } else {
      term.println("smartthing.send()");
      smartthing.send(var);
    }
  } else {
    if (logs) term.println("Too many events within 500ms");
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

  int newSpeed = map(speedVal, ULTRASLOW, FAST, 1, 100);
  //  term.println("map("+String(speedVal)+", "+String(ULTRASLOW)+", "+String(FAST)+", 1, 100");
  String var = String(newSpeed);
  //  term.println("current speed val = "+var);

  _server.send(200, "text/html", var);
  //  delay(1000);
  var = "level " + String(newSpeed);
  // sendHub(var); ; // useless verbose
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
  ESP.restart();
}

void hardReset() {
  resetRTCVars();
  unsigned long s = millis();
  unsigned long c = millis();
  term.println("Reset in 5 seconds..");
  while (millis() - s < 5000) {
    if (millis() - c >= 1000) {
      term.println(String((5000 - (millis() - s)) / 1000));
      term.println("");
      c = millis();
    }
    serverAndHub();  // needed for debug BUT ALSO to save the new RTC VARS
  }
  ESP.reset();  // esp.reset is hard reset
}

//*****************************************************************************
// SMARTTHINGS CALLOUT  | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                      V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void messageCallout(String message) {
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
    //if(my_integer_data >= 0){
    term.print("my_integer_data = ");
    term.println(my_integer_data);

    // now update and convert this integer into a pwm speed value
    speedVal = map(my_integer_data, 1, 100, ULTRASLOW, FAST); /***/
    speedOverride = true;

    // modulate speed accordingly in case it's already running
    modifRequested = true;
    term.println("donotAcceptCmdsFromHubs set to FALSE by HUB request");
    donotAcceptCmdsFromHubs = 0;

    if (closing || opening) {
      analogWrite(pwm, speedVal);
    }
    lastSpeedVal = speedVal;

    // inform ST that this value has been taken into consideration
    // reconvert to percentage
    int newSpeed = map(speedVal, ULTRASLOW, FAST, 1, 100);
    String var = "level " + String(newSpeed);
    sendHub(var);
    ;
  }

  if (donotAcceptCmdsFromHubs == 1) {
    term.println("NOT ACCEPTING COMMAND - DEVICE IN LEARNING MODE!");
    sendHub("DEVICE LEARNING");
    _server.send(200, "text/html", "LEARNING MODE");
    return;
  }

  else if (message.equals("open")) {
    term.println("opening");
    Open();
  } else if (message.equals("close")) {
    term.println("closing");
    Close();
  } else if (message.equals("stop")) {
    term.println("stopping");
    Stop("HUB");
  } else if (message.equals("refresh")) {
    term.println("refreshing");
    Refresh();
  } else if (message.equals("reset")) {
    term.println("reseting");
    Reset();
  }
}

boolean isOpen() {
  return digitalRead(contact_sensor);  // ISR is only for quick refresh, we still need to be able to call this boolean
}
void log() {
  //taken from windows minerva, some might be missing
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
  term.println("bootStart  = " + String(bootStart));
  term.println("lastTimeDisplay  = " + String(lastTimeDisplay));
  term.println("closingcmdMillis  = " + String(closingcmdMillis));
  term.println("previousMillisOpen  = " + String(previousMillisOpen));
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
  term.println("timeOfbootStart  = " + String(timeOfbootStart));
  term.println("lastMessage  = " + String(lastMessage));
  term.println("timerResult  = " + String(timerResult));
  term.println("lastCmd  = " + String(lastCmd));
  term.println("windowLastState  = " + String(windowLastState));
  term.println("bootStartDate  = " + String(bootStartDate));
  term.println("isDebugEnabled;  = " + String(isDebugEnabled));
  term.println("lastMaxOpDuration  = " + String(lastMaxOpDuration));
  term.println("shortDelayAdjust  = " + String(shortDelayAdjust));
  term.println("speedIncrement  = " + String(speedIncrement));
  term.println("speedAdjustmentOccurences  = " + String(speedAdjustmentOccurences));
  term.println("elapsed  = " + String(elapsed));
  term.println("TOUCH  = " + String(TOUCH));
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
  term.println("A  = " + String(A));
  term.println("B  = " + String(B));

  term.println("RTC vars are:");
  term.println("*************************************************************");
  term.print("CLOSINGSPEED =");
  term.println(CLOSINGSPEED);
  term.print("ULTRASLOW =");
  term.println(ULTRASLOW);
  term.print("PUSH =");
  term.println(PUSH);
  term.print("lastSpeedVal =");
  term.println(lastSpeedVal);
  term.print("openLearn =");
  term.println(String(openLearn == 1 ? "true" : "false"));
  term.print("closeLearn =");
  term.println(String(closeLearn == 1 ? "true" : "false"));
  term.print("successfulOpenLearn =");
  term.println(String(successfulOpenLearn == 1 ? "true" : "false"));
  term.print("successfulCloseLearn =");
  term.println(String(successfulCloseLearn == 1 ? "true" : "false"));
  term.print("donotAcceptCmdsFromHubs =");
  term.println(String(donotAcceptCmdsFromHubs == 1 ? "true" : "false"));
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

void OTAConfig() {
  ArduinoOTA.onStart([]() {
    term.println("Start");
    StopAll = true;
  });
  ArduinoOTA.onEnd([]() {
    term.println("\nEnd");
    digitalWrite(LED, 1);  //boot fails if pulled low, should be handeled by controller
    //hardReset(); // ESP8266 needs hard reset after flashing // deletes all RTCVars
    ESP.restart();  // soft reset instead
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    term.printf("Progress: %u%%\r\n", (progress / (total / 100)));
    term.handleClient();
    digitalWrite(LED, !digitalRead(LED));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    term.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) term.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) term.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) term.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) term.println("Receive Failed");
    else if (error == OTA_END_ERROR) term.println("End Failed");
    digitalWrite(LED, 1);  //boot fails if pulled low, should be handeled by controller though
    //hardReset();
  });
  ArduinoOTA.begin();
  term.println("ArduinoOTA Ready");
}

void RTCRegistration() {
  term.println("REGISTERING RTC VARIABLES");

  state.registerVar(&CLOSINGSPEED);  // we send a pointer to each of our variables
  state.registerVar(&lastOperation);
  state.registerVar(&reset_counter);
  state.registerVar(&PUSH);
  state.registerVar(&ULTRASLOW);
  state.registerVar(&openLearn);
  state.registerVar(&closeLearn);
  state.registerVar(&successfulOpenLearn);
  state.registerVar(&successfulCloseLearn);
  state.registerVar(&lastSpeedVal);
  state.registerVar(&donotAcceptCmdsFromHubs);
  state.registerVar(&failSafeBoot);
  state.registerVar(&resetClosingSpeed);
  state.registerVar(&ULTRASLOW_RESTORE);
  state.registerVar(&method_1_Failed);
}

void WiFiConfig() {
  WiFi.begin(ssid, password);
  WiFi.config(ip, dnsserver, gateway, subnet);
  Serial.println("Connecting to " + String(ssid) + " FAIL SAFE MODE!");
  unsigned long s = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - s < 10000) {
    delay(300);
    Serial.print(".");
  }
  if (WiFi.status() != WL_CONNECTED && millis() - s > 10000) {
    term.println("WIFI CONNEXION FAILED! RESETTING TO NORMAL MODE");
    resetRTCVars();
    ESP.reset();  // don't call hardReset() from here since it'd call master()
  }
}
/*END OF STANDARDIZED METHODS */