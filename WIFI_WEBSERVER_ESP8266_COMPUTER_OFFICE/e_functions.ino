void on() {
  ShutDown = false;

  var = "switch on";
  buildDebug(var); // inform ST HUB of the attempt
  if (!isOn()) {
    digitalWrite(switchPin, 1);
    delay(500);
    digitalWrite(switchPin, 0);
    Serial.println("");
    Serial.println("");
    Serial.println("COMPUTER TURNED ON");
  }
  else {
    Serial.println("ALREADY RUNNING");
  }

  delay(3000); // leave enough time for the device handler to change the state to "turningon"
  if (isOn()) {
    buildDebug("computer yes"); // tell device handler that computer is now on and running
  }
  else {
    buildDebug("error"); // tell hub that the computer failed to turnon
  }
}

void off() {
  buildDebug("switch off"); // inform ST HUB of the attempt
  if (isOn()) {
    digitalWrite(switchPin, 1);
    delay(1000);
    digitalWrite(switchPin, 0);
    unsigned int s = millis();
    while (isOn() && millis() - s < 30000)
    {
      loop();
    }
    // if still on after 30 seconds, means failed (probably switch turned on the screen
    if (isOn() && !ignorecommand)
    {
      digitalWrite(switchPin, 1);
      delay(1000);
      digitalWrite(switchPin, 0);
    }
    s = millis();
    while (isOn() && millis() - s < 30000)
    {
      loop();
    }
    if (isOn() && !ignorecommand) // one more attempt
    {
      digitalWrite(switchPin, 1);
      delay(1000);
      digitalWrite(switchPin, 0);
    }
    Serial.println("COMPUTER TURNED OFF");
  }
  else {
    Serial.println("ALREADY OFF");
  }

  delay(3000); // leave enough time for the device handler to change the state to "turningoff"
  if (isOn()) {
    buildDebug("computer yes"); // tell device handler that computer is now on and running
  }
  else {
    buildDebug("error"); // tell hub that the computer failed to turn off
  }
  ignorecommand = false; // reset this value
}


void Refresh() {
  if (!isOn() && !ShutDown) {
    Serial.println("reporting sleeping");
    buildDebug("computer sleeping");
  }
  else if (!isOn() && ShutDown) {
    Serial.println("reporting shutdown");
    buildDebug("computer shutdown");
  }
  else if (isOn())
  {
    Serial.println("reporting on");
    buildDebug("computer running");
  }
  else {
    buildDebug("ERROR");
  }
  //previousMillis = millis();
  Blink(5, 50);
}
/////////////////// BOOLEANS /////////////////////////
boolean isOn() {
  bool result;
  if (analogRead(ASensor) >= 1020) {
    result = true;
  }
  else {
    result = false;
  }
  //Serial.println(result);
  return result;
}

void buildDebug(String var)
{
  Serial.println("building debug");
  smartthing.send(var); // send the current value to smartthings
  debugData.concat(var + "\n"); // build the debug data string for the java text area
  Serial.println("SENDING " + var + "TO HUB");

}

void getDataDebug() // called by JS
{
  // send last updated value in getDebug()


  if (debugData.length() > 5)
  {
    _server.send(200, "text/html", debugData);
    if (debugData.length() > 2000)
    {
      debugData = "";
      logs = false; // stop sensor logs
      buildDebug("DATA RESET");
    }
  }
  else
  {
    Serial.println("debugData string empty");
  }
}
void getMac()
{
  String ip = WiFi.localIP().toString();
  var = "Mac: " + WiFi.macAddress() + " IP:" + ip + " " + String(WiFi.SSID());
  _server.send(200, "text/html", var);
}

void log()
{

  String debug1 = "";
  String debug2 = "";
  buildDebug(debug1);
  buildDebug(debug2);
}

String TimeInfos()
{
  unsigned long totalTimeSeconds = millis() / 1000;
  unsigned long totalTimeMinutes = millis() / 1000 / 60;
  unsigned long TotalTimeHours = millis() / 1000 / 60 / 60;
  unsigned long TotalTimeDays = millis() / 1000 / 60 / 60 / 24;

  unsigned long displaySeconds = (millis() - (totalTimeMinutes * 60 * 1000)) / 1000;
  unsigned long displayMinutes = (millis() - (TotalTimeHours * 1000 * 60 * 60)) / 1000 / 60;
  unsigned long displayHours = (millis() - (TotalTimeDays * 1000 * 60 * 60 * 24)) / 1000 / 60 / 60;
  unsigned long dispalyDays = TotalTimeDays;


  Serial.println("LOOP time = " + String(elapsed) + "ms");
  String result = "Time since last boot: ";

  if (dispalyDays == 1)
  {
    result = result + dispalyDays + " day";
    Serial.println(result);
  }
  else if (dispalyDays > 1)
  {
    result = result + dispalyDays + " days";
    Serial.println(result);
  }
  if (displayHours == 1)
  {
    result = result + displayHours + " hour ";
    Serial.println(result);
  }
  else if (displayHours > 1)
  {
    result = result + displayHours + " hours ";
    Serial.println(result);
  }
  if (displayMinutes == 1)
  {
    result = result + displayMinutes + " minute ";
    Serial.println(result);
  }
  else if (displayMinutes > 1)
  {
    result = result + displayMinutes + " minutes ";
    Serial.println(result);
  }
  if (displaySeconds == 1)
  {
    result = result + displaySeconds + " second ";
    Serial.println(result);
  }
  else if (displaySeconds > 1)
  {
    result = result + displaySeconds + " seconds ";
    Serial.println(result);
  }

  _server.send(200, "text/html", result);
  return result;
}

void glow(int DL) {
  int s = 1024;

  if (glw >= s) {
    goUp = false;
  }
  else if (glw < 1) {
    goUp = true;
  }

  if (goUp && glw < s) {
    glw++;
    analogWrite(LED, glw);
    //Serial.print("GLOW "); Serial.println(i);

  }
  else if (!goUp && glw > 0) {
    glw--;
    analogWrite(LED, glw);
    //Serial.print("GLOW "); Serial.println(i);
  }
  delay(DL);
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

void Reset() {
  ESP.reset();
}
