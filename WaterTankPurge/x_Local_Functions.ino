void messageCallout(String message)
{
  // if debug is enabled print out the received message
  if (isDebugEnabled)
  {
    Serial.print("Received message: '");
    Serial.print(message);
    Serial.println("' ");
  }

  // if message contents equals to 'on' then call on() function
  // else if message contents equals to 'off' then call off() function

  if (message.equals("switch on"))
  {
    On();
  }
  else if (message.equals("switch off"))
  {
    Off();
  }
  else if (message.equals("switch refresh"))
  {
    Refresh();
  }
}

void Refresh() {

  if (!Running) {
    Serial.println("reporting off");
    smartthing.send("pump off");
  }
  else
  {
    Serial.println("reporting on");
    smartthing.send("pump on");
  }

  Blink(5, 50);
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

void On() {
  cancel = false;
  digitalWrite(Signal, 1);

  Running = true;
  smartthing.send("switch on");
  occ = 1;
  lapse = 100;
  DELAY = 100;

  //  timeRemain = 0;// reset countdown
  unsigned long start = millis();
  //  unsigned long elapsed = 0;
  previousMillisPURGEduration = millis();
  unsigned long lastPrint = 0;
  int printTimer = 1; // in seconds

  unsigned long timeRemain = 0;
  int pumptime = 10; // pumping time in seconds

  while (millis() - start < pumptime * 1000 && !cancel) {
    loop();// allow http interruption command
    elapsed = millis() - start;
    timeRemain = ((pumptime * 1000) - elapsed) / 1000;

    if (millis() - lastPrint >= printTimer * 1000) {
      Serial.println("");
      Serial.print("time remaining = "); Serial.print(timeRemain); Serial.print(" seconds");
      lastPrint = millis();

    }

  }
  occ = 1;
  lapse = 200;
  DELAY = 500;
  digitalWrite(Signal, 0);
  Running = false;

}
void Off() {
  Running = false;
  cancel = true;// cancel any current pumping operation

  digitalWrite(Signal, 0);// stop operations
  smartthing.send("switch off");

}


void Reset() {
  smartthing.send("controller reset");
  pinMode(0, OUTPUT);
  delay(500);
  pinMode(2, OUTPUT);
  delay(500);
  pinMode(15, OUTPUT);
  delay(500);
  digitalWrite(0, HIGH);
  delay(500);
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(15, LOW);
  delay(500);
  ESP.restart();
}

