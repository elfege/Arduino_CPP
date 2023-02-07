/*UNIQUE SETTINGS BELOW DO NOT COPY PASTE TO WINDOW OFFICE NORTH OR OTHER ACTUATOR BASED DEVICE*/

void simpleOpen()
{
  analogWrite(motorclose, 0); // make sure the other port is at 0
  lastCmd = "close";
  pwm = motoropen;// pwm pin value must be updated so speed changes can happen while running
  term.println("analogWrite(" + String(pwm) + ", " + String(speedVal) + ";");
  analogWrite(pwm, speedVal);
}
void simpleClose()
{
  analogWrite(motoropen, 0); // make sure the other port is at 0
  lastCmd = "close";
  pwm = motorclose;// pwm pin value must be updated so speed changes can happen while running
  term.println("analogWrite(" + String(pwm) + ", " + String(speedVal) + ";");
  analogWrite(pwm, speedVal);
}
void forceClose(int dur)
{
  sendHub("window forceclose");
  lastCmd = "close";
  opening = true;
  closing = false;
  stopped = false;
  simpleClose();
  analogWrite(motorclose, FAST);
  delay(dur);
  if (dur == 0)
  {
    return;
  }
  Stop("FORCECLOSE");
}
void forceOpen(int dur)
{
  sendHub("window forceopen");
  lastCmd = "open";
  opening = true;
  closing = false;
  stopped = false;
  term.println("Force Open");
  simpleOpen();
  analogWrite(motoropen, FAST);
  delay(dur);
  if (dur == 0)
  {
    return;
  }
  Stop("FORCEOPEN");
}

void Stop(String cmdOrigin)
{
  term.println("STOP REQUESTED FROM: " + String(cmdOrigin));

  if (donotAcceptCmdsFromHubs == 1)
  {
    term.println("NOT ACCEPTING ANY STOP COMMAND, DEVICE IS LEARNING");
    return;
  }
  analogWrite(pwm, 0);
  digitalWrite(motorclose,  0);
  digitalWrite(motoropen,  0);

  lastOperation = 0; // saveToRTC() called in serverAndHubs()

  lastCmd = "stop";
  term.println("*****************************************STOP**************************************");
  sendHub("window stop");

  if (closing && touch())
  {
    NoTouchOpen = true;
  }

  closing = false;
  opening = false;



  if (!stopped) {
    term.println("STOP");
    Refresh();
  }
  stopped = true;
}
void simpleStop()
{
  EMstop = !EMstop;
  analogWrite(pwm, 0);
  digitalWrite(motorclose,  0);
  digitalWrite(motoropen,  0);
}


boolean touch() {
  return false; // analogRead(touch_sensor) >= 1023;
}

void pinConfig()
{
  pinMode(LED, OUTPUT);
  pinMode(pwm, OUTPUT);
  pinMode(motorclose, OUTPUT);
  pinMode(motoropen, OUTPUT);
  pinMode(armMov, INPUT_PULLUP); // interrupt pin
  attachInterrupt(digitalPinToInterrupt(armMov), handleArmMovInterrupt, CHANGE);
  pinMode(contact_sensor, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(contact_sensor), handleContactSensor, CHANGE);
  contactClosed = !isOpen();

  //pinMode(touch_sensor, INPUT_PULLUP);
}
