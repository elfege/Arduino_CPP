
void Open() {
  lastCmd = "open";
  buildDebug("switch on");// send a response so it is received through parse() method
  previousMillisMotors = millis();// reset the 5 minutes opening timer


  if (!opening)
  {
    opening = true;
    closing = false;
    stopped = false;
    Serial.println("OPENING");

    digitalWrite(Dir, HIGH); //Rotate stepper motor counterclockwise
    while (!isOpen())
    {
      Move();
      // no call for master() here
      stepcount++;
    }

    while (!stopped && stepcount < maxSteps)
    {
      Move();
      stepcount++;
      master();

      //Serial.println(digitalRead(contact_sensor_fullyOpen));
      //      Serial.println(stepcount);
    }
  }
  else
  {
    buildDebug("ignoring Redundant command..");
  }
}

void Close()
{
  lastCmd = "close";
  buildDebug("switch off");// send a response so it is received through parse() method
  previousMillisMotors = millis();// reset the 5 minutes opening timer
  // to prevent it from stopping a closing procedure
  if (!closing)
  {
    Serial.println("CLOSING");
    opening = false;
    closing = true;
    stopped = false;

    digitalWrite(Dir, LOW); //Rotate stepper motor in clockwise

    while (isFullyOpen())
    {
      Move();
      // no call for master() here
      stepcount--;
    }

    while (!stopped)
    {
      Move();
      master();
      stepcount--;
    }
  }
  else
  {
    buildDebug("ignoring Redundant command..");
  }

}
void calibrate()
{
  Serial.print("CALIBRATING");
  while (isOpen())
  {
    simpleClose();
  }
  Serial.println("CALIBRATION DONE!");
  stepcount = 0;
}
void simpleClose()
{
  lastCmd = "close";
  digitalWrite(Dir, LOW);
  Move();
}
void forceClose(int dur)
{

  buildDebug("window forceclose");
  lastCmd = "close";
  opening = true;
  closing = false;
  stopped = false;
  digitalWrite(Dir, LOW);
  unsigned long Start = millis();
  while (millis() - Start < dur /*&& !stopped*/)
  {
    Move();
  }
  Stop();
}
void forceOpen(int dur)
{
  buildDebug("window forceopen");
  lastCmd = "open";
  opening = true;
  closing = false;
  stopped = false;
  int i = 0;
  digitalWrite(Dir, HIGH); //Rotate stepper motor in clock wise direction
  unsigned long Start = millis();
  while (millis() - Start < dur && !stopped)
  {
    Move();
    stepcount++;
  }
  Serial.println(stepcount);
  Stop();
}

void Move()
{
  digitalWrite(Step, HIGH);
  delay(1);
  digitalWrite(Step, LOW);
  delay(1);

}
void Stop()
{

  digitalWrite(Step, LOW);

  lastCmd = "stop";
  //  Serial.println("*****************************************STOP**************************************");

  closing = false;
  opening = false;

  stopped = true;
}
void simpleStop()
{
  digitalWrite(Step, LOW);
}



boolean isOpen() {
  return digitalRead(contact_sensor_open);
}
boolean isFullyOpen() {
  return digitalRead(contact_sensor_fullyOpen);
}



void Refresh() {

  Serial.println("REFRESH CALLED!");
  String var = "ERROR";
  String var2 = "ERROR";
  String var3 = "ERROR";

  if (!isOpen()) {
    Serial.println("reporting closed");
    var = "contact closed";
    var2 = "window closed";
    var3 = "switch off";
  }
  if (isOpen()) {
    Serial.println("reporting closed");
    var = "contact open";
    var3 = "switch on";
  }
  else if (isOpen() && opening)
  {
    Serial.println("reporting opening");
    var = "window opening";
    var2 = "contact open";
    var3 = "switch on";
  }
  else if (isOpen() && closing)
  {
    Serial.println("reporting closing");
    var = "window closing";
  }
  else if (isOpen())
  {
    Serial.println("reporting open");
    var = "contact open";
    var2 = "window open";
    var3 = "switch on";
  }
  else if (stopped) {
    Serial.println("reporting stopped");
    var = "window stopped";
  }


  buildDebug(var); //st::Everything::sendSmartString(var); sent there...
  delay(500);
  buildDebug(var2);
  delay(500);
  buildDebug(var3);
  delay(500);
  RefreshSpeed();
  delay(500);

  // Blink(5, 50);

}


void RefreshSpeed()
{
  int newSpeed = map(SpeedVal, ULTRASLOW, FAST, 1, 100);
  String var = "level " + String(newSpeed);
  buildDebug(var);
  buildDebug("10 bits speed value = " + String(SpeedVal));
}

void buildDebug(String var) // called within Arduino C
{
  if (var != previousDebugDataCmd && var != previousDebugDataCmdBits && previousDebugDataCmd != var)
  {

    if (var.startsWith("8 bits")) {
      previousDebugDataCmdBits = var;
    }
    else if (var.startsWith("'level")) {
      previousDebugDataCmdLevel = var;
    }
    else {
      previousDebugDataCmd = var;
    }
    // build the debug data string with timing values
    debugData.concat(var + " at " +  String(TimingInfos()) + "\n");
  }

  // if var contains a ST's qualifying event attribute's name
  if (var.startsWith("switch") || var.startsWith("window") || var.startsWith("contact") || var.startsWith("level") || var.startsWith("boot OK"))
  {
    smartthing.send(var);
    delay(100);
    if (var != previousDebugDataCmd)
    {
      String ss = "'" + String(var) + "'" + " SENT TO ST";
      if (ss != previousDebugDataCmd)
      {
        previousDebugDataCmd = ss;
        debugData.concat(ss + " at " +  String(TimingInfos()) + "\n");
      }
    }
  }
}

void getDataDebug() // called by JS
{
  // send last updated value in getDebug()

  _server.send(200, "text/html", debugData);
  if (debugData.length() > 50000)
  {
    debugData = "";
    previousDebugData = "";
    previousDebugDataCmd = "";
    previousDebugDataCmdBits = "";
    previousDebugDataCmdLevel = "";
    logs = false; // stop sensor logs
    buildDebug("DATA RESET");
  }
}
void getMac()
{
  String ip = WiFi.localIP().toString();
  String var = "Mac: " + WiFi.macAddress() + " IP:" + ip + " " +  String(WiFi.SSID());
  _server.send(200, "text/html", var);
}

void getSpeed()
{
  int newSpeed = map(SpeedVal, ULTRASLOW, FAST, 1, 100);
  String var = String(newSpeed);
  _server.send(200, "text/html", var);
  //  delay(1000);
  var = "level " + String(newSpeed);
  buildDebug(var); ;

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
  ESP.reset(); // esp.reset is hard reset
}

//*****************************************************************************
// SMARTTHINGS CALLOUT  | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                      V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void messageCallout(String message)
{

  //Blink(5, 50);

  String mess = "Received message: '" + String(message) + "' ";

  lastMessage = message; // update global
  Serial.println(mess);

  // check if this message is a digit
  if (isDigit(message.charAt(0))) { //   -> TRUE
    ///  see if there is a request for a different speed
    String thisMessage = message;
    char this_char[thisMessage.length() + 1];
    thisMessage.toCharArray(this_char, sizeof(this_char));
    int my_integer_data = atoi(this_char);
    //if(my_integer_data >= 0){
    Serial.print("my_integer_data = "); Serial.println(my_integer_data);

    // now update and convert this integer into a pwm speed value
    //    SpeedVal = map(my_integer_data, 1, 100, ULTRASLOW, FAST);

    // modulate speed accordingly in case it's already running
    modifRequested = true;
    //    analogWrite(pwm, SpeedVal);
    if (SpeedVal == FAST)
    {
      emergencySpeed = true;
      emergencySpeedMillis = millis();
    }
    lastSpeedVal = SpeedVal;

    // inform ST that this value has been taken into consideration
    // reconvert to percentage
    int newSpeed = map(SpeedVal, ULTRASLOW, FAST, 1, 100);
    String var = "level " + String(newSpeed);
    buildDebug(var); ;

  }
  else if (message.equals("open"))
  {
    Serial.println("opening");
    Open();
  }
  else if (message.equals("close"))
  {
    Serial.println("closing");
    Close();
  }
  else if (message.equals("stop"))
  {
    Serial.println("stopping");
    Stop();
  }
  else if (message.equals("refresh"))
  {
    Serial.println("refreshing");
    Refresh();
  }
  else if (message.equals("reset"))
  {
    Serial.println("reseting");
    Reset();
  }
}

void log()
{

  if (millis() - previousMillis >= 1000)
  {
    previousMillis = millis();
    String debug1 = "contact_sensor_open digital READ        = " + String(digitalRead(contact_sensor_open));
    String debug2 = "contact_sensor_fullyOpen digital READ = " + String(digitalRead(contact_sensor_fullyOpen));
    String debug3 = "IS OPEN? = " + String(isOpen());
    String debug4 = "IS FULLY OPEN? = " + String(isFullyOpen());
    Serial.println(debug1);
    Serial.println(debug2);
    Serial.println(debug3);
    Serial.println(debug4);
    //    Serial.println(debug4);
    //  buildDebug(debug1);
    //  buildDebug(debug2);
    //  buildDebug(debug3);
    //      buildDebug(debug4);
  }
}
