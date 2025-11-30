
void Open() {
  lastCmd = "open";
  buildDebug("switch on");// send a response so it is received through parse() method
  if (!opening)
  {
    Serial.println("OPENING START");

    buildDebug("switch on"); // for alexa
//    buildDebug("window opening");

    opening = true;
    closing = false;
    stopped = false;
    SpeedVal = ULTRASLOW; // restart from base speed
    modifRequested = false;



    analogWrite(pwm, SpeedVal);

    digitalWrite(MA,  1);
    digitalWrite(MB,  0);

    SpeedVal = SpeedVal + PUSH;
//    buildDebug("10 bits speed value = " + String(SpeedVal));
    if (!isOpen())
    {
      analogWrite(pwm, SpeedVal);

      while (!isOpen())
      {
        loop();

      }
    }
    SpeedVal = ULTRASLOW;
    analogWrite(pwm, SpeedVal);
    //buildDebug("10 bits speed value = " + String(SpeedVal));
    previousMillisMotors = millis(); // store time of last execution
    Serial.println("END OF OPENING");
    //buildDebug("END OF open() scope");
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
  // to prevent it from stopping a closing procedure
  if (!closing)
  {
    closingcmdMillis = millis();
    Serial.println("CLOSING");

    buildDebug("switch off"); // for Alexa
    buildDebug("window closing");

    closing = true;
    stopped = false; // reset this value
    opening = false;
    modifRequested = false;
    SpeedVal = ULTRASLOW; // reset to silent mode every new opeartion
    lastSpeedVal = SpeedVal;
    NoTouchOpen = true; // so touch doesn't reopen, but stops only


    analogWrite(pwm, FAST);
    digitalWrite(MA,  0);
    digitalWrite(MB,  1);

  }
  else
  {
    buildDebug("ignoring Redundant command..");
  }
  previousMillisMotors = millis(); // store time of last execution
}
void simpleClose()
{
  lastCmd = "close";

  analogWrite(pwm, SpeedVal);
  digitalWrite(MA,  0);
  digitalWrite(MB,  1);
}
void forceClose(int dur)
{

  buildDebug("window forceclose");
  lastCmd = "close";
  opening = false;
  closing = true;
  stopped = false;
  analogWrite(pwm, FAST);
  digitalWrite(MA,  0);
  digitalWrite(MB,  1);
  delay(dur);
  Stop();
}
void forceOpen(int dur)
{
  buildDebug("window forceopen");
  lastCmd = "open";
  opening = true;
  closing = false;
  stopped = false;
  Serial.println("Force Open");
  analogWrite(pwm, FAST);
  digitalWrite(MA,  1);
  digitalWrite(MB,  0);
  delay(dur);
  Stop();
}

void Stop()
{
  lastCmd = "stop";
  Serial.println("*****************************************STOP**************************************");
  buildDebug("window stop");

  if (closing && touch())
  {
    NoTouchOpen = true;
  }

  closing = false;
  opening = false;

  analogWrite(pwm, 0);
  digitalWrite(MA,  0);
  digitalWrite(MB,  0);


  if (!stopped) {
    Serial.println("STOP");
    Refresh();
  }
  stopped = true;
}
void simpleStop()
{
  analogWrite(pwm, 0);
  digitalWrite(MA,  0);
  digitalWrite(MB,  0);
}



boolean isOpen() {

  return digitalRead(contact_sensor);
}

boolean touch() {
  return analogRead(touch_sensor) >= 1023;
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
  //buildDebug("10 bits speed value = " + String(SpeedVal));
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
    SpeedVal = map(my_integer_data, 1, 100, ULTRASLOW, FAST);

    // modulate speed accordingly in case it's already running
    modifRequested = true;
    analogWrite(pwm, SpeedVal);
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

  delay(200);
  String debug1 = "CONTACT READ = " + String(digitalRead(contact_sensor));
  String debug2 = "IS OPEN? = " + String(isOpen());
  String debug3 = "touch() FUNCTION = " + String(touch());
  String debug4 = "touch Sensor Read returns " + String(analogRead(touch_sensor));
  Serial.println(debug1);
  Serial.println(debug2);
  Serial.println(debug3);
  Serial.println(debug4);
  buildDebug(debug1);
  buildDebug(debug2);
  buildDebug(debug3);
  buildDebug(debug4);
}
