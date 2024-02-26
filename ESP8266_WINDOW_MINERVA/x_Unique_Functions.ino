/*UNIQUE SETTINGS BELOW DO NOT COPY PASTE TO WINDOW LIVING OR OTHER ACTUATOR BASED DEVICE*/
void simpleClose()
{
  lastCmd = "close";
  digitalWrite(MA,  A);
  digitalWrite(MB,  B);
  digitalWrite(FAN, 1);
}
void simpleOpen()
{
  lastCmd = "Open";
  digitalWrite(MA,  B);
  digitalWrite(MB,  A);
  digitalWrite(FAN, 1);
}
void forceClose(int dur)
{

  sendHub("window forceclose");
  lastCmd = "close";
  opening = true;
  closing = false;
  stopped = false;
  analogWrite(pwm, FAST);
  digitalWrite(MA,  A);
  digitalWrite(MB,  B);
  digitalWrite(FAN, 1);
  delay(dur);
  if (dur == 0)
  {
    return;
  }
  Stop("forceClose");
}
void forceOpen(int dur)
{
  sendHub("window forceopen");
  lastCmd = "open";
  opening = true;
  closing = false;
  stopped = false;
  term.println("Force Open");
  analogWrite(pwm, FAST);
  digitalWrite(MA,  B);
  digitalWrite(MB,  A);
  digitalWrite(FAN, 1);
  delay(dur);
  if (dur == 0)
  {
    return;
  }
  Stop("forceOpen");
}

void Stop(String cmdOrigin)
{
  term.println("STOP REQUESTED FROM: " + String(cmdOrigin));

  if (donotAcceptCmdsFromHubs == 1)
  {
    term.println("NOT ACCEPTING ANY STOP COMMAND, DEVICE IS LEARNING");
    return;
  }
  lastOperation = 0; // saveToRTC() called in serverAndHubs()

  lastCmd = "stop";
  events = 0;
  term.print("STOPPED @ "); term.println("LOC : " + timeClient.getFormattedTime());
  sendHub("window stop");

  if (closing && touch())
  {
    NoTouchOpen = true;
  }

  closing = false;
  opening = false;
  closingUpdateDone = false;
  openingUpdateDone = false;

  analogWrite(pwm, 0);
  digitalWrite(MA,  0);
  digitalWrite(MB,  0);
  digitalWrite(FAN, 0);


  if (!stopped) {
    term.println("STOP");
    Refresh();
  }
  stopped = true;
}
void simpleStop()
{
  EMstop = !EMstop;
  closing = false;
  opening = false;
  analogWrite(pwm, 0);
  digitalWrite(MA,  0);
  digitalWrite(MB,  0);
  digitalWrite(FAN, 0);
}


boolean touch() {
  return analogRead(touch_sensor) >= 1023;
}

void pinConfig()
{
  pinMode(LED, OUTPUT);
  pinMode(pwm, OUTPUT);
  pinMode(MA, OUTPUT);
  pinMode(MB, OUTPUT);
  pinMode(FAN, OUTPUT);
  digitalWrite(FAN, 0);
  //  pinMode(armMov, INPUT_PULLUP); // interrupt pin // NOT IMPLEMENTED YET
  //  attachInterrupt(digitalPinToInterrupt(armMov), handleArmMovInterrupt, CHANGE);
  pinMode(contact_sensor, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(contact_sensor), handleContactSensor, CHANGE);
  contactClosed = !isOpen();
  pinMode(touch_sensor, INPUT_PULLUP);
  delay(1);
}
