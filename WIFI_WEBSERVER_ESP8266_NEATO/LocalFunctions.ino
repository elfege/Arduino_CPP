
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
  else if (message.equals("rebootrobot"))
  {
    rebootRobot();
  }
  else if (message.equals("wakeup"))
  {
    WakeUp();
  }
  else if (message.equals("shutdown"))
  {
    ShutDown();
  }
  
   if (message.equals("webserver"))
  { 
    Serial.println("Starting webserver");
    activateWeb = true; 
  }
  else if (message.equals("NOwebserver"))
  {
    
    activateWeb = false; 
  }
}

///// LOCAL FUNCTIONS ////
void On() {

  Serial.println("");
  Serial.print("ON----------------ON");
  turnOffRequested = false;
  digitalWrite(switchPin, OFF); // Turn On
  delay(200);
  digitalWrite(switchPin, ON);// Turn Off

  //Blink(5, 50);
}

void TurnOn() {
  smartthing.send("starting");
  On();
  Serial.println("");
  Serial.println("ON COMMAND sent");
  delay(3000); // time for the analog port to read value
  if (!isAwake()) {
    Serial.println("NOT AWAKE!");

    //Blink(20, 50);
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
  digitalWrite(switchPin, OFF);// press and hold
  Alreadyrestarted = true;
  delay(5000);
  digitalWrite(switchPin, ON);// release button
  delay(1000);
  //wake it up
  digitalWrite(switchPin, OFF);// press and hold
  delay(300);
  digitalWrite(switchPin, ON);// release button
  delay(2000);
  TurnOn();
  Alreadyrestarted = true;

}
void rebootRobot() {
  //Simple reboot
  smartthing.send("restarting");
  digitalWrite(switchPin, OFF);// press and hold

  delay(5000);
  digitalWrite(switchPin, ON);// release button
  delay(1000);
  //wake it up
  digitalWrite(switchPin, OFF);// press and hold
  delay(300);
  digitalWrite(switchPin, ON);// release button
  delay(2000);
  Refresh();

}

void TurnOff() {
  // simple pause
  turnOffRequested = true;
  digitalWrite(switchPin, OFF); // Turn On
  delay(200);
  digitalWrite(switchPin, ON);// Turn Off

  //Blink(2, 1000);
  if (!isRunning()) {
    smartthing.send("stopped");
  }
  else {
    digitalWrite(switchPin, OFF); // Turn On
    delay(200);
    digitalWrite(switchPin, ON);// Turn Off
    if (!isRunning()) {
      smartthing.send("stopped");
    } else {
      smartthing.send("error");
    }
  }
}
void ShutDown() { 
  //  deep sleep
  digitalWrite(switchPin, OFF);// press and hold
  delay(3000);
  digitalWrite(switchPin, ON);// release button
}
void WakeUp() {

  wakingup = false;
  if (!isAwake()) {
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
  //PingTest();
}

void Refresh() {
  if (isRunning()) {
    Serial.println("reporting running");
    smartthing.send("running");
  }
  else if (!isRunning() && isAwake()) {
    Serial.println("reporting awake");
    smartthing.send("awake");
  }
  else if (!isAwake()) {
    Serial.println("reporting sleeping");
    smartthing.send("sleeping");
  }
  else {
    smartthing.send("error");
  }

  if(activateWeb && !websent){
    smartthing.send("webison");
    websent = true; 
  }
  else if(!websent) {
    smartthing.send("webisoff");
    websent = true; 
  }

  // measure current value
  int AwakeVal = analogRead(ASensor);
  // build the string
  String VoltString = String(AwakeVal);
  //String strSend = String("voltage: ") + String(VoltString);
  smartthing.send("voltage: " + VoltString);

  previousMillis = millis();
}
void Reset() {
  smartthing.send("reset");
  Serial.println("RESETING ESP8266");
 
  digitalWrite(RESET_PIN, LOW);

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
  bool resultRun;
  //Serial.println("isRunning function called");
  int isRunVal = digitalRead(Run);
  //Serial.println("");
  //Serial.print("isRunVal = "); Serial.print(isRunVal);
  if (isRunVal == 1) {
    resultRun = true;
  }
  else {
    resultRun = false;
  }
  return resultRun;
}


