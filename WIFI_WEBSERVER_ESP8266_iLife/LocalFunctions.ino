
//*****************************************************************************
// Local Functions  | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
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


void messageCallout(String message)
{

  Serial.print("Received message: '");
  Serial.print(message);
  Serial.println("' ");


  // if message contents equals to 'on' then call on() function
  // else if message contents equals to 'off' then call off() function

  if (message.equals("stop"))
  {
    TurnOff();
  }
  else if (message.equals("reset"))
  {
    Reset();
  }
  else if (message.equals("start"))
  {
    TurnOn();
  }
  else if (message.equals("refresh"))
  {
    Refresh();
  }
  else if (message.equals("wakeup"))
  {
    WakeUp();
  }
  else if (message.equals("shutdown"))
  {
    ShutDown();
  }
}

///// LOCAL FUNCTIONS ////
void On() {

  Serial.println("");
  Serial.print("ON----------------ON");
  turnOffRequested = false;
  digitalWrite(switchPin, 0); // Turn On
  delay(200);
  digitalWrite(switchPin, 1);// Turn Off

  Blink(5, 50);
}

void TurnOn() {
  smartthing.send("starting");
  On();
  Serial.println("");
  Serial.println("ON COMMAND sent");
  delay(3000); // time for the analog port to read value
  if (!isAwake()) {
    Serial.println("NOT AWAKE!");

    Blink(20, 50);
    WakeUp();
  }
  if (!isAwake() && !turnOffRequested) {
    Serial.println("Robot didn't wake up! restarting it");
    restartRobot();
  }
  else if (turnOffRequested) {
    Serial.println("Turn Off Requested, cancelling operation");
  }
  else {
    Serial.println("AWAKE!");
    delay(5000);
    if (!isRunning()) {
      On();
    }
  }
  Serial.println("");
  Serial.println("DONE");
  delay(500);

  if (!isRunning() && !Alreadyrestarted && !turnOffRequested) {
    Serial.println("NOT RUNNING, RESTARTING ROBOT AND TRYING AGAIN");
    restartRobot();
    smartthing.send("restarting");
  }
  else if (Alreadyrestarted) {
    Serial.println("Restart Already Attempted, doing nothing");
  }
  else {
    Serial.println("NOW CLEANING");
    smartthing.send("cleaning");
  }
}

void restartRobot() {
  smartthing.send("restarting");
  digitalWrite(switchPin, 0);// press and hold
  Alreadyrestarted = true;
  delay(5000);
  digitalWrite(switchPin, 1);// release button
  delay(1000);
  //wake it up
  digitalWrite(switchPin, 0);// press and hold
  delay(300);
  digitalWrite(switchPin, 1);// release button
  delay(2000);
  TurnOn();
  Alreadyrestarted = true;

}
void rebootRobot() {
  //Simple reboot
  smartthing.send("restarting");
  digitalWrite(switchPin, 0);// press and hold

  delay(5000);
  digitalWrite(switchPin, 1);// release button
  delay(1000);
  //wake it up
  digitalWrite(switchPin, 0);// press and hold
  delay(300);
  digitalWrite(switchPin, 1);// release button
  delay(2000);
  Refresh();

}

void TurnOff() {
  // simple pause
  turnOffRequested = true;
  digitalWrite(switchPin, 0); // Turn On
  delay(200);
  digitalWrite(switchPin, 1);// Turn Off

  Blink(2, 1000);
  if (!isRunning()) {
    smartthing.send("stopped");
  }
  else {
    digitalWrite(switchPin, 0); // Turn On
    delay(200);
    digitalWrite(switchPin, 1);// Turn Off
    if (!isRunning()) {
      smartthing.send("stopped");
    } else {
      smartthing.send("error");
    }
  }
}
void ShutDown() { // DOESN't WORK
  // turn off neato, put it in deep sleep
  digitalWrite(switchPin, 0);// press and hold
  delay(5000);
  digitalWrite(switchPin, 1);// release button
}
void WakeUp() {

  wakingup = false;
  if (isAwake()) {
    smartthing.send("resume");
    On();
    delay(3000);
    smartthing.send("awake");
  }
  else {
    Serial.println("ALREADY ON");
    smartthing.send("awake");
  }
  Refresh();
}
void waitAndTryAgain() {
  delay(10000);
  PingTest();
}

void Refresh() {
  if (!isAwake()) {
    Serial.println("reporting sleeping");
    smartthing.send("sleeping");
  }
  else if (!isRunning() && isAwake()) {
    Serial.println("reporting awake");
    smartthing.send("awake");
  }
  else if (isRunning()) {
    Serial.println("reporting running");
    smartthing.send("running");
  }
  else {
    smartthing.send("error");
  }



  // measure current value
  int AwakeVal = analogRead(ASensor);
  if (AwakeVal <= 600) {
    smartthing.send("sleeping");
  }
  // build the string
  String VoltString = String(AwakeVal);
  //String strSend = String("voltage: ") + String(VoltString);
  smartthing.send("voltage: " + VoltString);

  previousMillis = millis();
}
void Reset() {

  Serial.println("RESETING ESP8266");
  Serial.println("");


  delay(100);


  //wdt_reset();
  prepareforOTA();
  setup();
}

void prepareforOTA() {
  // set GPIO#0(D3), 2(D4) and 15(D8) must be set to 1,1,0 to prevent failed reset
  wdt_reset();
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
  wdt_reset();


}

boolean PingSuccess() {
  bool ret = Ping.ping(iptoping);
  return ret;
}

void PingTest() {

  int avg_time_ms = Ping.averageTime();

  delay(100);

  if (PingSuccess()) {
    Serial.println("PING SUCCESS! Average time is: ");
    Serial.print(avg_time_ms);
    Serial.println("");
  }
  else {
    Serial.println("PING FAILED. Reseting board");
    Serial.println("");
    Reset();
  }

}

boolean isAwake() {

  bool resultAwake = false;
  //Serial.println("isAwake function called");
  //Serial.println("");
  //Serial.print("AwakeVal = "); Serial.print(AwakeVal);
  int AwakeVal = analogRead(ASensor);

  //smartthing.send("voltage:"); smartthing.send(VoltString);
  if (AwakeVal > AwakeVolt) {
    resultAwake = true;
  }
  else {
    resultAwake = false;
  }
  return resultAwake;
}

boolean isRunning() {
  isAwake();
}


