
void RUN()
{
  On();
  smartthing.send("switch on");
  delay(500);
  smartthing.send("feeder feeding");

  unlock();
  loadAcan();
  if (loaded)
  {
    if (!testRun)
    {
      openCan();
    }
    else
    {
      Serial.println("*******TEST RUN! SKIPPING CAN OPENING TASK***********");
    }

  }
  else
  {
    Serial.println("END OF EXECUTION");
    return;
  }
  canOpUp();

  unload();

  //removeLid(); // will also reset the receiver into flat position // deprecated

  Serial.println("END OF FEEDING");
  smartthing.send("feeding done");

  Off(); // turn off the power once this is done

}

void openCan()
{
  int relockDone = 0;
  unlock();
  lp.write(keepCanFromInward); // prevent the can from sliding away inward // happens when a can has bumps
  delay(3000);
  lock();

  // release pressure from the can opener onto the can
  //canopArm.write(holdDown + 20);
  digitalWrite (M1, HIGH);
  digitalWrite (M2, LOW);

  unsigned long Start = millis();
  int pos = canopArm.read();
  while (millis() - Start < 60000 && !STOP) { // excessive time contributes greatly to detaching the lid !
    master();
  }

  stopOpener();

  unlock(); // unlock the can opener

}
void compressCan(boolean a) {
  closegate();
  Serial.println("COMPRESSING CAN");
  if (a == true)
  {
    lp.write(90);
  }
  digitalWrite (LA1, LOW);
  digitalWrite (LA2, HIGH);
}
void holdCanWithCompressors() {

  digitalWrite (LA1, LOW);
  digitalWrite (LA2, HIGH);
  delay(5000);
  digitalWrite (LA1, LOW);
  digitalWrite (LA2, LOW);
}
void decompressCan() {
  Serial.println("DECOMPRESSING CAN");
  lp.write(pullIn);
  digitalWrite (LA1, HIGH);
  digitalWrite (LA2, LOW);
}
void redo()
{
  unlock();
  delay(500);
  canopArm.write(55);
  delay(500);
  canopArm.write(holdDown);
  delay(500);
  lock();
  delay(500);
  digitalWrite (M1, HIGH);
  digitalWrite (M2, LOW);
}

void simpleopencan()
{
  digitalWrite (M1, HIGH);
  digitalWrite (M2, LOW);
  unsigned long Start = millis();
  while (millis() - Start < 10000 && !STOP) {

    master();

  }
  stopOpener();
}

void gateholdcan()
{
  gate.write(gateHold);
}
void opengate()
{
  gate.write(gateOpen);
}
void opengateSlowly() {
  unsigned long Start = millis();
  int cur = gate.read();
  while (gate.read() > gateOpen && millis() - Start < 10000)
  {
    gate.write(cur--); // carry the can out of slide, slowly
    delay(100);
  }

  if (millis() - Start > 10000)
  {
    gate.write(gateOpen);
  }
}


void loadAcan()
{
  Serial.println("STARTING loadAcan()");
  On();// for when we run tests
  delay(1000);
  STOP = false;
  loaded = false;
  isBusy = true;
  canOpUp(); // make sure the can opener is out of the way

  unsigned long Start = millis();


  opengate();

  if (canLoaded())
  {
    Serial.println("GETTING RID OF OLD CAN");
    lp.write(extractCan);

    while (millis() - Start < 12000 && !STOP)
    {
      master();
    }
    lp.write(pullIn);

    while (millis() - Start < 5000 && !STOP)
    {
      master();
    }
  }

  Serial.println("Pulling out lower pusher to hold the can that'll fall from C2");
  lp.write(pullOut - 20); // leave room for gateholdcan() to be effective
  Start = millis();
  while (millis() - Start < 8000 && !STOP)
  {
    master();
  }
  gateholdcan(); // Gate serves here to hold the can that falls into chamber 1 // prevents upper can from falling to far down

  Serial.println("Pulling in upper holder to relase a can");
  uh.write(pullIn); // pull in the upper holder to release can from C2
  Start = millis();
  while (millis() - Start < 8000 && !STOP) // give enough to gravity to do its job
  {
    master();
  }
  boolean pulledIn = true; // after retracupperholder ran, pulledIn has to be true

  Serial.println("Pulling out upper holder to hold the other cans");
  uh.write(pullOut); // make sure upper holder is out
  Start = millis();
  while (millis() - Start < 4000 && !STOP)
  {
    master();
  }
  pulledIn = false; // reset this value accordingly

  Start = millis();
  if (C1empty())
  {
    Serial.println("FIRST ATTEMPT TO LOAD CAN INTO C1 FAILED... Trying again...");


    /****failsafe last attempt*****/
    gateholdcan(); // put the gate back into can holding position
    uh.write(pullIn); // just to give a nudge to the lower can in C1 so it ends up well on top of lp hole
    Start = millis();
    while (millis() - Start < 6000 && !STOP) // give enough to gravity to do its job
    {
      master();
    }
    Start = millis();
    uh.write(pullOut);
    while (millis() - Start < 8000 && !STOP) // give enough time for  gravity to do its job
    {
      master();
    }
    if (C1empty())
    {
      Serial.println("FAILED TO LOAD A CAN IN CHAMBER 1 -- distance = " + String(distance) + "cm");
      Off(); // no need to keep the device on, turn it off
      STOP = true;
      return; // cancel operation
    }
  }
  else
  {
    Serial.println("CHAMBER 1 " + String(C1empty() ? "EMPTY" : "LOADED") + " -- distance = " + String(distance) + "cm");
    Serial.println("NOW LOADING TRAY");
  }




  /**************************************************/
  /*                 TRAY LOADING                   */
  /**************************************************/

  lp.write(pullIn); // pull in lower holder to free the can from C1
  pulledIn = true; // update this value
  Serial.println("RELEASING CAN FROM C1");
  delay(4000);
  opengateSlowly(); // use gate opening to carry the can out of the arm's tip

  Serial.println("RELEASING CAN FROM C1");
  Start = millis();
  while (millis() - Start < 6000 && !STOP) // give it some time without doing anything else to fully withdraw the holder
  {
    master();
  }

  /***PUSHING OUT OF C1 WHILE MAYBE HOLDING THE CAN ONTO THE ROD***/
  Serial.println("PUSHING CAN OUT OF C1");
  lp.write(pullOut); // push the can onto the tray
  pulledIn = false; // update this value
  Start = millis();
  while (millis() - Start < 6000 && !STOP) // give it some time without doing anything else to fully withdraw the holder
  {
    master();
  }

  /***DROPPING BY PULLING THE ROD COMPLETELY***/
  Serial.println("DROPPING CAN ONTO THE TRAY");
  lp.write(pullIn); // pull back in to let the can drop onto the tray
  pulledIn = true; // update this value
  Start = millis();
  while (millis() - Start < 6000 && !STOP) // give it some time without doing anything else to fully withdraw the holder
  {
    master();
  }
  /***PUSHING TOWARD THE GATE*****/
  Serial.println("PUSHING CAN TOWARDS THE GATE");
  lp.write(pullOut + 10); // push the can toward the gate
  pulledIn = false; // update this value
  Start = millis();
  while (millis() - Start < 6000 && !STOP) // give it some time without doing anything else
  {
    master();
  }

  /*** FULLY RETRACT THE LOWER PUSHER ****/
  Serial.println("FINAL RETRACT OF LOWER PUSHER");
  lp.write(pullIn); // pull back in
  pulledIn = true; // update this value
  Start = millis();
  while (millis() - Start < 6000 && !STOP) // give it some time without doing anything else to fully withdraw the holder
  {
    master();
  }

  //
  //push can backward back onto the tray
  //using compressors for a short time
  // in case old can is in the way preventing closedgate() to
  // operate properly
  compressCan(false); // without using lower pusher, of course
  Start = millis();
  while (millis() - Start < 4000 && !STOP) // give it some time
  {
    master();
  }
  decompressCan();
  Start = millis();
  while (millis() - Start < 1500 && !STOP) // give it some time
  {
    master();
  }
  closegateSlow(); //push the can back into proper position, using the gate, slowly

  if (canLoaded())
  {
    pulledIn = false;
    Serial.println("CAN SUCCESSFULLY LOADED ONTO THE TRAY!");
    delay(1000);
    canOpDown(); // bring the can opener down
    Serial.println("CAN LOADED AND LOCKED!");
  }
  else
  {
    Serial.println("FAILED TO LOAD A CAN ONTO THE TRAY!");
    Off(); // no need to stay on
    return;
  }

  loaded = true;
  isBusy = false;
  Serial.println("END OF loadAcan()");
}

void unload()
{
  isBusy = true;
  unlock(); // unlock the lid again, just in case that was a test run
  pushupperholder(); // make sure to hold the can above
  delay(500);
  canOpUp(); // bring the arm up

  detachTheLid();

  gate.write(65);

  lp.write(extractCan);
  Serial.println("WAITING 15 SECONDS FOR EXTRACTION (1st time)");
  unsigned long Start = millis();
  while (millis() - Start < 15000 && !STOP)
  {
    master();
  }
  lp.write(pullIn);
  Start = millis();
  while (millis() - Start < 5000 && !STOP)
  {
    master();
  }
  lp.write(extractCan); // do it a second time in case rod went under the can
  Serial.println("WAITING 10 SECONDS FOR EXTRACTION (2nd time)");
  Start = millis();
  while (millis() - Start < 10000 && !STOP)
  {
    master();
  }
  lp.write(pullIn);
  Start = millis();
  while (millis() - Start < 5000 && !STOP)
  {
    master();
  }


  /// third attempt, just in case
  Start = millis();
  while (millis() - Start < 5000 && !STOP)
  {
    master();
  }
  lp.write(extractCan); // do it a third time in case rod went under the can
  Serial.println("WAITING 10 SECONDS FOR EXTRACTION (3rd time)");
  Start = millis();
  while (millis() - Start < 10000 && !STOP)
  {
    master();
  }
  lp.write(pullIn);
  Start = millis();
  while (millis() - Start < 5000 && !STOP)
  {
    master();
  }

  opengate();

  Serial.println("END OF UNLOAD");
  isBusy = false;

}
void detachTheLid()
{

  Serial.println("Executing detachTheLid");
  int i = 0;
  int pos;

  unlock();
  canOpUp();
  closegate();
  compressCan(true);
  unsigned long Start = millis();
  while (millis() - Start < 10000 && !STOP)
  {
    master();
  }

  i =  0;
  for (!STOP; i < 4 && !STOP; i++) {
    //      gate.write(gateClosed + 50); //  compress the can // DEPRECATED DAMAGES THE GATE HOLD TO THE SERVO
    //      delay(500);
    //      gate.write(gateClosed);
    //      delay(800);

    decompressCan();
    delay(1500);
    compressCan(true);
    delay(1500);
  }

  decompressCan();
  opengate();
  unlock();
  canOpUp();
  opengate();
}

void closegate()
{
  //  int c = 0;
  //  while (c < 3) {
  //    gate.write(gateSligtOpen);
  //    delay(300);
  //    gate.write(gateClosed);
  //    delay(300);
  //    c++;
  //  }
  closegateSlow();

}

void closegateSlow()
{
  int pos = gate.read();

  while (pos != gateClosed)
  {
    if (pos < gateClosed)
    {
      gate.write(pos++);
    }
    else if (pos > gateClosed)
    {
      gate.write(pos--);
    }
    delay(50);
  }
  gate.write(gateClosed - 5);// don't leave it fully closed to prevent pressing against the can during opening operation
}

void testLoadAllCans()
{

  int c = 0;
  while (c < 7 && !STOP)
  {
    c++;
    Serial.println("Test Load #" + String(c) + " is running");
    loadAcan();
    while (isBusy && !STOP)
    {
      master();
    }
    Serial.println("Test Load #" + String(c) + " done!... Now unloading can");

    unload();
    while (isBusy && !STOP)
    {
      master();
    }
  }
}
void canOpUp() {
  int cur = canopArm.read();
  unlock();
  //Serial.println("arm position before lift = " + String(cur) + " " + "holdUp = " + String(holdUp));
  if (cur != holdUp)
  {
    Serial.println("exececuting canOpUp");
    opengate();
    delay(200);
    unsigned long Start = millis();

    canopArm.write(holdUp);

  }
  else
  {
    Serial.println("ARM ALREADY UP!");
  }
  //Serial.println("arm position after lift = " + String(canopArm.read()));
}
void canOpDown() {

  int cur = canopArm.read();
  lp.write(keepCanFromInward); // prevent the can from sliding away inward // happens when a can has bumps
  delay(2000); // time for the pusher to get to requested position
  if (canopArm.read() != holdDown)
  {
    unlock();
    delay(100);
    Serial.println("exec canOpDown");
    unsigned long Start = millis();
    while (canopArm.read() < holdDown && millis() - Start < 5000)
    {
      canopArm.write(cur++);
      delay(30);
    }
    Start = millis();
    while (canopArm.read() > holdDown - 40 && millis() - Start < 5000) // redo
    {
      canopArm.write(cur--); // go back up a nudge
      delay(30);
    }
    Start = millis();
    while (canopArm.read() < holdDown && millis() - Start < 5000)
    {
      canopArm.write(cur++);
      delay(10);
    }
    Start = millis();

    if (millis() - Start > 5000)
    {
      canopArm.write(holdDown);
    }
  }
  else
  {
    Serial.println("ARM ALREADY DOWN!");
  }
}

void retractlowerpusher()
{
  pushupperholder();// always make sure upper pusher is in the right position
  Serial.println("retracting lower pusher");
  lp.write(pullInLP);
}

void pushupperholder()
{
  Serial.println("pushing upper holder");
  //pushlowerpusher(); // make sur can doesn't fall to far down
  uh.write(pullOut);
}

void pushlowerpusher()
{
  Serial.println("pushing lower pusher");
  lp.write(pullOut);
}
void fullPushLowerPusher()
{
  Serial.println("lower pusher 180");
  lp.write(180);
}
void fullyRetractLowerPusher()
{
  Serial.println("lower pusher 0");
  lp.write(pullIn);
}
void fullRetractUpperHolder()
{
  Serial.println("Upper Holder 0");
  uh.write(pullIn);
}
void lpplus()
{
  Serial.println("Lower Pusher Plus 5");
  lp.write(lp.read() + 5);
}
void uhplus()
{
  Serial.println("Lower Pusher Plus 5");
  uh.write(uh.read() + 5);
}
void armPlus()
{
  Serial.println("Arm +");
  canopArm.write(canopArm.read() + 1);
}
void armMinus()
{
  Serial.println("Arm -");
  canopArm.write(canopArm.read() - 1);
}

void extractcan()
{
  unload();
}

void retractupperholder()
{
  Serial.println("retracting upper holder");
  //gateholdcan();
  pushlowerpusher(); // make sur can doesn't fall to far down

  delay(200);

  uh.write(pullOut);


  // before pulling back in make sure c1 does its job
  c1.write(120);
  uh.write(pullIn); // pull back in

}


void lock()
{
  opengate();
  digitalWrite (M1, HIGH);
  digitalWrite (M2, LOW);
  unsigned long Start = millis();
  while (isUnlocked() && millis() - Start < 1500 && !STOP) { // isUnlocked() is true when unlocked
    delay(1); //master();
  }
  stopOpener();
}
void unlock()
{
  digitalWrite (M1, LOW);
  digitalWrite (M2, HIGH);
  unsigned long Start = millis();

  while (!isUnlocked() && millis() - Start < 2000) {
    delay(1); // don't run master() or it might overdrive the bus
  }
  stopOpener();
}
void startOpener()
{
  digitalWrite (M1, HIGH);
  digitalWrite (M2, LOW);
}

void stopOpener()
{
  digitalWrite (M1, LOW);
  digitalWrite (M2, LOW);

}

int ultrasound(int echo, int trig)
{
  digitalWrite(echo, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH);
  return duration * 0.034 / 2; // distance in cm
}

void canreceiverflat()
{
  Serial.println("receiver back to flat position");
  int i = canReceiver.read();
  int p = receiverflat;
  int del = 10;

  canreceiver(i, p, del);
}

void canreceiverup() // set can into receiving position
{

  Serial.println("receiver up");
  int i = canReceiver.read();
  int p = receiverup;
  int del = 100; // this one must be slow

  canreceiver(i, p, del);
}

void canreceivertrash() {

  Serial.println("receiver trash");
  forceRemoveCan();
  delay(100);
  gate.write(gateClosed + 30);

  if (!canLoaded()) // no opeartion of the lidremover as long as there's still a can onto the tray
  {

    // first, make sure gate is fully closed
    gate.write(gateClosed + 30);
    delay(600);
    Lid(180); // open position
    delay(600);
    int i = canReceiver.read();
    int p = receivertrash;
    int del = 10;

    canreceiver(i, p, del);
    delay(2000);
    shakeCanReceiver();
    canreceiverflat();
    delay(500);
    gate.write(gateOpen); // make sure gate is back to normal

    trashactionOk = true;
  }
  else
  {
    Serial.println("CAN ON THE TRAY, TRASH OPERATION IMPOSSIBLE");
    trashactionOk = false;
  }
}

void canreceiver(int i, int p, int del)
{

  if (i < p)
  {
    for (p != 0; i < p; i++)
    {
      canReceiver.write(i);
      delay(del);
    }

  }
  else {
    for (p != 0; i > p; i--)
    {
      canReceiver.write(i);
      delay(del);
    }
  }
}

void shakeCanReceiver()
{
  int i = canReceiver.read();
  int c = 0;
  unsigned long Start = millis();
  while (millis() - Start < 2000)
  {
    canReceiver.write(i);
    delay(50);
    canReceiver.write(i - 20);
    delay(50);
  }
  canreceiverflat();
}

void openlidremover()
{
  if (!canLoaded()) // no opeartion of the lidremover as long as there's still a can onto the tray
  {
    gate.write(gateClosed + 30); //
    delay(500);
    Lid(180); // open position
  }
  else
  {
    Serial.println("CAN ON THE TRAY, LID REMOVER OPERATION IMPOSSIBLE");
  }
  delay(500);
  gate.write(gateOpen); // make sure gate is now open again

}

void forceRemoveCan()
{
  unsigned long str = millis();
  gate.write(gateClosed + 30); // make sure gate is closed
  while (millis() - str < 5000)
  {
    Lid(0);
    delay(100);
    Lid(40);
    delay(100);
  }
  gate.write(gateOpen);

}
void Lid(int p)
{
  int del = 0;
  int i = lidRemover.read();

  if (i < p)
  {
    for (p != 0; i < p; i++)
    {
      lidRemover.write(i);
      delay(del);
    }

  }
  else {
    for (p != 0; i > p; i--)
    {
      lidRemover.write(i);
      delay(del);
    }
  }
}

/*****************************************************
***********************   BOOLEANS *******************
******************************************************/

boolean C1empty()
{
  distance = ultrasound(chamber1echo, chamber1trig);
  if (debug)
  {
    Serial.println("distance: " + String(distance) + "cm");
  }
  return distance > 9;
}

boolean isUnlocked() {
  return digitalRead(pressed);
}

boolean canLoaded()
{
  if (debug)
  {
    Serial.println("IR signal = " + String(analogRead(canLoad)));
  }
  return analogRead(canLoad) >= 200;
}



/**************************************************************************************/

/// BOARD MANAGEMENT FUNCTIONS

void Blink(int times, int lapse) {
  int c = 0;

  while (c < times) {
    digitalWrite(LED, 0); // led off
    delay(lapse);
    digitalWrite(LED, 1); // led on
    delay(lapse);
    c++;


  }
  digitalWrite(LED, 1); // led on
}

void On()
{
  digitalWrite(power, 0);
  pwstate = "on";
  smartthing.send("switch " + pwstate);
  lastState = pwstate;
  lastOnMillis = millis();
}
void Off()
{
  digitalWrite(power, 1);
  pwstate = "off";
  smartthing.send("switch " + pwstate);
  lastState = pwstate;
}


void Reset() {
  Serial.println("RESET!");
  delay(10);
  digitalWrite(rst, LOW);
  delay(1000);
  Serial.println("HARDWARE RESET FAILED!");
  delay(10);
  resetFunc();
}
