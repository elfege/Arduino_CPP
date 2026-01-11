void feed() {
  unsigned long Start;
  problem = false;
  // feeding = true; push() methods needs this value to be false
  stopped = false;
  canDetectedByLimitSwitchDuringLoad = false;

  term.println("FEED STARTED!");

  term.println("************************************************************************");
  term.println("                           CLEANING                                     ");
  term.println("************************************************************************");

  term.println("PULLING SMALL PUSHER...");
  pull();           // start pulling out master arm
  feeding = false;  // allow proper arm pusherRetractCheck (otherwise arm is stuck at the bottom)
  // cleanOpener();   // giving enough time for master arm to retract

  feeding = true;  // must not be true before cleaning is done or master arm will pull all the way
  smallPull();     // timed function

  ledcWrite(pwmChannelActuator, 0);  // stop the arm

  // it can happen  that a can is still loaded
  // Make sure this is taken care of, otherwise the push process might take this faulty

  stopped = false;
  masterisout = true;  // make sure boolean is set to true so smallpush can work
  term.println("CLEARING THE WAY");
  smallPush();  // clear the area as much as possible
  smallPull();
  term.println("LOADING AREA CLEARED!");

  term.println("*************************************************************************");
  term.println("                          LOADING A CAN                                  ");
  term.println("*************************************************************************");

  stopped = false;
  load();

  term.println("DONE LOADING");

  term.println("*************************************************************************");
  term.println("                     POSITIONING OPENER ONTO THE CAN                     ");
  term.println("*************************************************************************");
  stopped = false;

  if (!problem && (canInPosition() || canDetectedByLimitSwitchDuringLoad))  // || canDown())) /* || hasSpun)*/ canDown() is too early and wasn't meant for that initially=> intended for lid release (TODO)
  {
    term.println("CAN IN POSITION AND READY FOR OPENING OPERATION TO START");
    unlock();  // be on the safe side

    term.println("SETTING OPENER ONTO THE CAN");
    verticalPush();

    term.println("************************************************************************");
    term.println("                          OPENING A CAN                                 ");
    term.println("************************************************************************");
    openCan();
  } else {
    if (problem) {
      term.println("SOMETHING WENT HORRIBLY WRONG... A CAN MAY BE STUCK!");
    }
    term.println("CAN NOT DETECTED!");
    stop();
    problem = true;
    problemMillis = millis();
    return;
  }

  // give it 2 seconds for food content to rest inside
  term.println("2 SECONDS RESTING");
  Start = millis();
  while (!stopped && millis() - Start < 2000) {
    pauseHandler("");
    mainHandler();
  }

  /**************************************************************************/
  /*                          PULL UP                                       */
  /**************************************************************************/

  verticalPull();  // will unlock the can (can no longer allow holding it upward due to wheel's spring system)

  /**************************************************************************/
  /*                          EXTRACTION                                    */
  /**************************************************************************/
  extract();

  feeding = false;

  // pull(); // will be pulled at next feed
}
void load() {
  term.println("loading a can...");
  lastActionMillis = millis();
  stopped = false;
  loading = true;
  problem = false;

  unlock();  // make sure can opener is unlocked

  int attempt = 0;
  int maxattempts = 3;
  bool deformation_occured = false;
  deformOverride = true;

  int maxLoadTime = 55000;     // if reached, it's a fail => ONLY CONDITION WHEN REACHED is when there's been some serious deformation.
  int optimaLoadTime = 42000; //37700;  // first rule timer: if any of the 2 sensors fails, operation can go on. DEPRECATED: 42000 we now leave more time for canInPosition() to take precedence. 

  // make sure the master arm is in proper position
  initialize_Master_Arm();  // required for timer's precision.
  stopped = false;

  push();
  unsigned long load_duration = 0;
  unsigned long Start = millis();
  while (!canInPosition() && millis() - Start < maxLoadTime && millis() - Start < optimaLoadTime && !stopped)  // we use timer as discriminatory criterion (in case of sensors failure)
  {
    pauseHandler("push");

    if (millis() - load_duration >= 1000) {
      term.println("load push duration:" + String((millis() - Start) / 1000) + " seconds.");
      load_duration = millis();
    }
    // detect chassis deformation
    if (deformed()) {
      unsigned long deform_Duration = millis();  // record starting point
      deformation_occured = true;
      int deform_Time_Offset = 2000;  // time allowance from deformation detection until any action is taken. If deformation isn't felt after this time, resume normal operation and, otherwise, pull back.
      attempt++;
      term.println("----------------------------------------------------------------");
      term.println("       **         CHASSIS DEFORMATION DETECTED!        **       ");
      term.println("----------------------------------------------------------------");
      unsigned long s = millis();
      while (millis() - s < deform_Time_Offset && deformed() && !stopped)  // give it n seconds in case it goes through
      {
        //        pauseHandler(""); // NO PAUSE HERE, EVER... major safety concern!
        // mainHandler(); No handler of any kind either - any lagging network communication would make the arm crush the can completely!
        yield();
        delay(10);
      }
      delay(50);  // necessary to prevent false positives returned by deformed() due to Analog noise

      // if still deformed, proceed with retracting
      if (deformed() && !stopped) {
        mainArmPull();
      }
      s = millis();
      while (millis() - s < deform_Time_Offset && deformed() && !stopped) {
        pauseHandler("mainArmPull");
        mainHandler();
      }

      if (millis() - s >= deform_Time_Offset && attempt >= maxattempts) {
        stop();
        term.println("something's wrong, stopping...");
        problem = true;
        problemMillis = millis();
        return;
      }
      push();                               // once done, resume with push operation
      Start += millis() - deform_Duration;  // compensate for lost time (matters for max duration to be effective)
    }

    mainHandler();
  }
  deformOverride = false;

  if (canInPosition() || millis() - Start >= optimaLoadTime) {
    term.println(String(millis() - Start >= optimaLoadTime ? "optimal time reached! Everything seems ok" : "limit switch reached... "));
    canDetectedByLimitSwitchDuringLoad = true;
  }

  if (stopped) {
    term.println("OPERATION TERMINATED AT USER'S REQUEST...");
    problem = true;
    return;
  }
  if (millis() - Start >= maxLoadTime) {
    term.println("OPERATION FAILED! (passed max load time)");
  } else {
    term.println("PUSH DONE!");
  }

  // make sure to pull the master arm until release of the can limit switch or N seconds, whichever comes first
  // (timer needed due to possible presence of older cans pushing the limit switch)
  // this is needed to prevent the can from being too far away from the opener due to IR sensor fluctuations
  // the rotary sensor's arm's spring will push the can back into proper position
  pullMasterRelease();

  loading = false;
}
void openCan() {
  lastActionMillis = millis();
  canopening = true;
  term.println("OPENING A CAN");
  canForward();
  unsigned long Start = millis();
  unsigned long timer = millis();
  unsigned long interval = millis();  // interval between mandatory readjustments
  unsigned long lastSimulatedFailedSPin = millis();  // simulate failed spin every n seconds to ensure proper readjustment of the can opener's blade
  int simulatorInterval = 20000;

  bool simulator = false; 

  // int intervalLimit = 30000;         // minimum time allowed without readjusting // DEPRECATED
  int duration = 150000;  // needs long time to desold the lid, but also not too long to prevent damaging the blade too fast
  int absoluteMaxDuration = 240000;
  int noSpinErrors = 0;
  int maxSpinErrors = 7;  // beyond this number of no spin events, either the can is open (hence the no spin), or it's most likely that it has too many deformations, or the sensor is faulty.

  // Set to 3 if spin sensor fails and comment out '&& noSpinErrors < maxSpinErrors'
  while (!stopped && millis() - Start < duration && millis() - Start < absoluteMaxDuration /*&& noSpinErrors < maxSpinErrors*/) {
    pauseHandler("canForward");
    // wheelSpinning();
    mainHandler();

    // add '&& noSpinErrors < maxSpinErrors' here if the same statement above is commented out,
    // comment it out here if it's not commented out above.
    // When commented out in the line above, and not here, the device will stop checking for spin errors and continue
    // The spin sensor being a relatively fragile and exposed moving part, the system should be able to ignore it after a certain point
    // instead of throwing an error an uselessly losing remaining functionality.

    // simulate failed spin every n seconds
    if(millis() - lastSimulatedFailedSPin >= simulatorInterval){
      lastSimulatedFailedSPin = millis();
      simulator = true;
    }

    if (simulator || (!spin || millis() - timer > 2000 && noSpinErrors < maxSpinErrors)) {
      simulator = false; // reset this boolean 
      term.println("CHECKING SPIN STATUS...");
      unsigned long s = millis();
      term.println("total elapsed time: " + String((millis() - Start) / 1000) + " seconds");
      while (!spin && millis() - s < 1500 && !stopped)  // wait for N ms as confirmation test to avoid false positives due to can's deformations
      {
        mainHandler();
      }
      if (simulator || !spin)  // if still not spinning, execute the recovery process
      {
        noSpinErrors++;
        term.println("no spin events = " + String(noSpinErrors));
        unsigned long NoSpinTime = millis();
        unlock();
        verticalPull();
        verticalPush();
        lock();
        canForward();
        unsigned long elapsed = millis() - NoSpinTime;
        duration += elapsed;
        interval = millis();
      } else {
        term.println("FALSE ALARM, SPIN Ok!");
      }
      timer = millis();
      
    } else if (!spin && noSpinErrors >= maxSpinErrors) {
      term.println("No longer checking on spin errors and continuing operation while ignoring spin sensor");
    }
    
  }
  unlock();
  verticalUp();
  delay(500);
  verticalStop();

  // ERROR HANDLER. Comment it out if spin sensor fails
  if (millis() - Start >= absoluteMaxDuration + 500 || noSpinErrors >= maxSpinErrors) {
    problem = true;
    problemMillis = millis();
    term.println("PLEASE MAKE SURE TO USE UNDAMAGED CANS...");
  }
  canopening = false;
}
void open_free() {
  term.println("OPEN FREE!");
  canForward();
  unsigned long Start = millis();
  while (!stopped && millis() - Start < 60000) {
    mainHandler();
  }
}
void extract() {
  extractionStart = millis();

  term.println("EXTRACTING");

  lastActionMillis = millis();
  loading = false;
  stopped = false;
  extracting = true;

  unlock();  // free the can from the opener
  // freeCanLid();  // will attempt to detach lid by dropping the can, then unlock and pull back up.// too messy

  push();  // extract the can

  unsigned long Start = millis();
  // push for at least 30 seconds and then use limit switch (minimum of 30 seconds in case old can was still there)
  while (millis() - Start < 30000 && !stopped) {
    pauseHandler("push");
    mainHandler();
  }

  term.println("EXTRACTION DONE!");
  extracting = false;

  stop();  // confirm just in case some other process might have been in await mode

  pull();  // MUST be done before small pusher can take over
  Start = millis();
  while (!pusherRetracted() && !stopped && millis() - Start < 15000) {  // just 15 secs timer to free the way for the small pusher
    mainHandler();
  }
  // arm continues pulling and is managed by pusherRetractCheck() (called from mainhandler)

  smallPush();  // has its own timer
  // smallPull(); // has its own timer // KEEP IT OUT to support cats pushing the lid over.
}

/************* MASTER ARM **********/
void pullMasterRelease()  // pull master arm back from its maxed out and limit switched triggered position
{
  // if (canInPosition())
  // {
  term.println("PULLING ARM BACK 1 SECOND");
  mainArmPull();
  unsigned long Start = millis();
  while ((eatingZonePushed() || millis() - Start < 2500) && !stopped)  // never set the pull back to more than 4 seconds, we need the arm for the unload operation
  {
    if (paused) {
      pauseHandler("mainArmPull");
      Start = millis();
    }
    mainHandler();
  }
  simpleStop();
  // }
}
void initialize_Master_Arm()  // reset master arm to its starting point,  a trigger & release limit switch method.
{
  unsigned long Start;
  if (!pusherRetracted()) {
    adjusting = true;  // disable pusherRetractCheck() handler
    pull();
    term.println("RETRACTING");
    Start = millis();
    while (!pusherRetracted() && !stopped && millis() - Start < 60000) {
      pauseHandler("pull");
      mainHandler();
    }
  }
  push();
  Start = millis();
  while (pusherRetracted() && !stopped && millis() - Start < 60000) {
    pauseHandler("push");
    mainHandler();
  }
  adjusting = false;  // enable pusherRetractCheck() handler
}
void push() {
  term.println("PUSHING");
  lastActionMillis = millis();
  masterisout = true;
  pushing = true;
  pulling = false;
  stopped = false;
  ledcWrite(pwmChannelActuator, 256);
  digitalWrite(ACTUATOR_A, 0);
  digitalWrite(ACTUATOR_B, 1);
}
void mainArmPull() {
  term.println("mainArmPull()");
  stopped = false;
  lastActionMillis = millis();
  pull();
}
void pull() {
  term.println("PULLING");
  pulling = true;
  pushing = false;
  stopped = false;
  lastActionMillis = millis();
  // loading = false;
  ledcWrite(pwmChannelActuator, 256);
  digitalWrite(ACTUATOR_A, 1);
  digitalWrite(ACTUATOR_B, 0);
}

/************* SMALL ARM**********/
void smallPush() {
  term.println("Small Push Request");
  smallisout = true;
  lastActionMillis = millis();
  stopped = false;
  if (masterisout) {
    pwm.attachPin(servo_pwm, 10000);  // 10khz
    SMALL_PUSHER.attach(SMALL_PUSHER_PIN);
    SMALL_PUSHER.write(140);
    unsigned long Start = millis();
    while (millis() - Start < 8000 && !stopped) {
      if (paused) {
        SMALL_PUSHER.detach();
        pauseHandler("");
        Start = millis();
        SMALL_PUSHER.attach(SMALL_PUSHER_PIN);
        SMALL_PUSHER.write(140);
      }
      mainHandler();
    }
    // for (pos = 0; pos <= 180 && !stopped; pos += 1)
    // {
    //   SMALL_PUSHER.write(pos);
    //   term.println("pos:" + String(pos));
    //   mainHandler();
    //   delay(20);
    // }
    SMALL_PUSHER.detach();
    pwm.detachPin(servo_pwm);
  } else {
    term.println("PULL MASTER ARM FIRST!");
    Blink(10, 100);
  }
}
void smallPushNoTimer() {
  pwm.attachPin(servo_pwm, 10000);  // 10khz
  SMALL_PUSHER.attach(SMALL_PUSHER_PIN);
  SMALL_PUSHER.write(140);
}
void stopSmallPush() {
  SMALL_PUSHER.detach();
}
void smallPull() {
  term.println("Small Pull Request");
  smallisout = false;
  lastActionMillis = millis();
  stopped = false;

  pwm.attachPin(servo_pwm, 10000);  // 10khz
  SMALL_PUSHER.attach(SMALL_PUSHER_PIN);
  unsigned long Start = millis();
  SMALL_PUSHER.write(50);
  while (millis() - Start < 8000 && !stopped) {
    if (paused) {
      SMALL_PUSHER.detach();
      pauseHandler("");
      Start = millis();
      SMALL_PUSHER.attach(SMALL_PUSHER_PIN);
      SMALL_PUSHER.write(50);
    }
    mainHandler();
  }
  // for (pos = 180; pos >= 0 && !stopped; pos -= 1)
  // {
  //   SMALL_PUSHER.write(pos);
  //   term.println("pos:" + String(pos));
  //   mainHandler();
  //   delay(20);
  // }
  SMALL_PUSHER.detach();
  pwm.detachPin(servo_pwm);
}

/***********CAN OPENER*************/

void canForward() {
  term.println("CAN OPENER RUNNING FWD");
  lastActionMillis = millis();
  ledcWrite(pwmChannelCanOpener, speedValOpener);
  digitalWrite(OPENER_B, 0);
  digitalWrite(OPENER_A, 1);
}
void canBackWard() {
  ledcWrite(pwmChannelCanOpener, speedValOpener);
  digitalWrite(OPENER_B, 1);
  digitalWrite(OPENER_A, 0);
}
void canStop() {
  ledcWrite(pwmChannelCanOpener, 0);
  digitalWrite(OPENER_B, 0);
  digitalWrite(OPENER_A, 0);
}
void lock() {
  term.println("LOCKING");
  canForward();
  // ledcWrite(pwmChannelCanOpener, maxSpeedOpener);
  stopped = false;
  unsigned long Start = millis();
  while (millis() - Start < 2000 && !stopped) /*&& !canLocked() timer only is safer*/
  {

    mainHandler();
  }
  simpleStop();
}
void unlock() {
  term.println("UNLOCKING");
  canBackWard();
  ledcWrite(pwmChannelCanOpener, maxSpeedOpener);
  stopped = false;
  unsigned long Start = millis();
  while (millis() - Start < 1500 && !stopped) /*&& canLocked() timer only is safer*/
  {
    mainHandler();
  }
  simpleStop();
}
void slightUnlock() {
  term.println("SLIGHT UNLOCKING");
  canBackWard();
  stopped = false;
  unsigned long Start = millis();
  while (millis() - Start < 800 && !stopped) /*&& canLocked() timer only is safer*/
  {
    mainHandler();
  }
  simpleStop();
}
void freeCanLid() {

  for (int i = 0; i < 3; i++) {
    verticalPush();  // put it back down (HAS INNER TIMER)
    lock();

    verticalUpStep(200, 10);  // param1:steps, param2: millis // slightly pull up the can while holding it (locked) (NO INNER TIMER)
    wait(2000);
    slightUnlock();      // unlock just enough in a vague attempt to hold the lid and release the can
    smallPushNoTimer();  // push the can in an attempt to desolidarize it from the lid
    // verticalDownStep(200, 10); // push back downcase stuck upward

    delay(3500);  // not using wait, must be exactly n seconds
    smallPull();  // inner 8 seconds timer
  }

  verticalPull();  // retract (includes unlock() and therefore releases the lid when the can should be further away)
  cosinusPull();
}
/***********VERTICAL ACTUATORS*************/
void verticalPush() {

  term.println("VERTICAL PUSH");
  overrideImpact = true;  // disable impact handler
  lastActionMillis = millis();
  goingdown = true;
  problem = false;
  stopped = false;

  unsigned long Start = millis();
  term.println("VERTICAL PUSH DOWN... ");
  cosinusPush();  // always push a bit
  Start = millis();
  delay(600);  // no handler: precision required...
  cosinusStop();

  verticalDown();
  delay(1400); // not all the way down, we need to pull cosinus back a bit first
  verticalStop();
  cosinusPull();
  Start = millis();
  while (millis() - Start < 1500 && !stopped) {
    mainHandler();
  }
  cosinusStop();
  
  verticalDownStep(90, 30);

  goingdown = false;
  overrideImpact = false;  // enable impact Handler
}
void verticalPull() {
  term.println("VERTICAL PULL");
  unlock();  // always
  lastActionMillis = millis();
  goingdown = false;
  stopped = false;
  verticalUp();
  unsigned long Start = millis();
  while (millis() - Start < 500 && !stopped) {
    mainHandler();
  }
  verticalStop();
  // push cosinus outward a bit to free vertical puller off its socket and prevent resistance onto the vertical pulling process (which made the actuator dislodge itself before this implementation - feb 17 2024)
  cosinusPush();
  Start = millis();
  while (millis() - Start < 300 && !stopped) {
    delay(1);
  }
  cosinusStop();

  verticalUp();
  Start = millis();
  while (millis() - Start < 3000 && !stopped) {
    mainHandler();
  }
  cosinusStop();
  cosinusPull();
  Start = millis();
  while (millis() - Start < 1500) {
    delay(1);
  }
  verticalStop();
}
void verticalDownStep(int steps, int interval) {
  term.println("VERTICAL DOWN STEPS: " + String(steps));
  for (int i = 0; i < steps; i++) {
    verticalDown();
    delay(interval);
    verticalStop();
    delay(interval);
  }
  verticalStop();
}
void verticalUpStep(int steps, int interval) {
  term.println("VERTICAL UP STEPS: " + String(steps));
  for (int i = 0; i < steps; i++) {
    verticalUp();
    delay(interval);
    verticalStop();
    delay(interval);
  }
  verticalStop();
}
void verticalUp() {
  term.println("VERTICAL UP");
  goingdown = false;
  digitalWrite(vertPush_A, 1);
  digitalWrite(vertPush_B, 0);
}
void verticalDown() {
  goingdown = true;
  digitalWrite(vertPush_A, 0);
  digitalWrite(vertPush_B, 1);
}
void verticalStop() {
  goingdown = false;
  digitalWrite(vertPush_A, 0);
  digitalWrite(vertPush_B, 0);
}
void cosinusPush() {
  cosinuspushing = true;
  cosinuspulling = false;
  cosinusMillis = millis();
  term.println("COSINUS PUSH");
  ledcWrite(pwmChannelCos, speedValCosinus);
  digitalWrite(COSINUS_A, 1);
  digitalWrite(COSINUS_B, 0);
}
void cosinusPull() {
  cosinuspushing = false;
  cosinuspulling = true;
  cosinusMillis = millis();
  term.println("COSINUS PULL");
  ledcWrite(pwmChannelCos, speedValCosinus);
  digitalWrite(COSINUS_A, 0);
  digitalWrite(COSINUS_B, 1);
}
void cosinusPushStep(int steps, int length) {
  term.println("COSINUS PULL STEPS: " + String(steps));
  for (int i = 0; i < steps; i++) {
    cosinusPush();
    delay(length);
    cosinusStop();
    delay(length);
  }
  cosinusStop();
}
void cosinusPullStep(int steps, int length) {
  term.println("COSINUS PULL STEPS: " + String(steps));
  for (int i = 0; i < steps; i++) {
    cosinusPull();
    delay(length);
    cosinusStop();
    delay(length);
  }
  cosinusStop();
}
void cosinusStop() {
  ledcWrite(pwmChannelCos, 0);
  digitalWrite(COSINUS_A, 0);
  digitalWrite(COSINUS_B, 0);
  cosinuspulling = false;
  cosinuspushing = false;
  term.println("COSINUS STOP");
}

void stop() {
  term.println("STOP");
  stopped = true;
  pushing = false;
  pulling = false;
  extracting = false;
  loading = false;
  goingdown = false;
  feeding = false;
  loading = false;
  poppinglid = false;
  adjusting = false;
  canopening = false;
  gotimpact = false;
  overrideImpact = false;
  cosinuspushing = false;
  recording = false;

  ledcWrite(pwmChannelActuator, 0);
  ledcWrite(pwmChannelCanOpener, 0);
  ledcWrite(pwmChannelCos, 0);
  digitalWrite(ACTUATOR_A, 0);
  digitalWrite(ACTUATOR_B, 0);

  digitalWrite(OPENER_B, 0);
  digitalWrite(OPENER_A, 0);
  digitalWrite(vertPush_A, 0);
  digitalWrite(vertPush_B, 0);
  digitalWrite(COSINUS_A, 0);
  digitalWrite(COSINUS_B, 0);
}
void simpleStop() {
  ledcWrite(pwmChannelActuator, 0);
  ledcWrite(pwmChannelCanOpener, 0);
  ledcWrite(pwmChannelCos, 0);
  term.println("SIMPLE STOP");

  digitalWrite(ACTUATOR_A, 0);
  digitalWrite(ACTUATOR_B, 0);
  digitalWrite(OPENER_B, 0);
  digitalWrite(OPENER_A, 0);
  digitalWrite(vertPush_A, 0);
  digitalWrite(vertPush_B, 0);
  digitalWrite(COSINUS_A, 0);
  digitalWrite(COSINUS_B, 0);
}

void logs() {
  if (Logs && millis() - millisLogs >= 1000) {
    // term.println("oneCanLeft() returns " + String(oneCanLeft() == 1 ? "true" : "false"));
    term.println("canLocked() returns " + String(canLocked() == 1 ? "true" : "false"));
    term.println("canInPosition() returns " + String(canInPosition() == 1 ? "true" : "false"));
    term.println("A deformSensor => " + String(analogRead(deformSensor)));
    term.println("analogRead(IR1_pile) returns " + String(analogRead(IR1_pile)));
    term.println("analogRead(IR2_pile) returns " + String(analogRead(IR2_pile)));
    term.println("empty() returns " + String(empty() == 1 ? "true" : "false"));
    term.println("full() returns " + String(full() == 1 ? "true" : "false"));
    term.println("eatingZonePushed() " + String(eatingZonePushed() ? "true" : "false"));
    term.println("pusherRetracted() " + String(pusherRetracted() ? "true" : "false"));
    term.println("digitalRead(candown) " + String(digitalRead(candown)));
    term.println("wheelStateChange(1, 300) returns " + String(wheelStateChange(1, 300)));
    millisLogs = millis();
  }
}
