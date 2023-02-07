void on() {
  OnOffRunning = true;
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
  updateState();
  
  OnOffRunning = false;
}

void off() {
  // NB: NEVER USE A RETURN STATEMENT IN THIS METHOD
  OnOffRunning = true;
  if (!noSleep)
  {
    Serial.println("Putting computer to sleep");

    if (isOn())
    {
      digitalWrite(switchPin, 1);
      delay(500);
      digitalWrite(switchPin, 0);
      unsigned long s = millis();
      while (millis() - s < 70000 && !STOP && isOn())
      {
        master();
      }
      if (millis() - s > 70000 && !STOP && isOn() && !userOverride)
      {
        // failed so try again
        digitalWrite(switchPin, 1);
        delay(500);
        digitalWrite(switchPin, 0);
      }
      STOP = false; // reset this boolean after using it
    }
    else
    {
      Serial.println("Computer already off or sleeping");
    }
  }
  else
  {
    Serial.println("SLEEP OVERRIDE - noSleep BOOL TRUE");
  }
  updateState();
  OnOffRunning = false;
}
void updateState() {
  if (isOn())
  {
    lastState = "on";
    buildDebug("switch on");
    buildDebug("computer running");
  }
  else
  {
    lastState = "off";
    buildDebug("switch off");
    buildDebug("computer sleeping");
  }
}
void getPowerState()
{
  togglePower();
  String state = isOn() ? "computer running" : "computer sleeping";
  _server.send(200, "text/plain", state);
  buildDebug(state);
}

void getPowerStateOnly()
{
  String state = isOn() ? "computer running" : "computer sleeping";
  _server.send(200, "text/plain", state);
  buildDebug(state);
}


void togglePower()
{
  if (isOn())
  {
    STOP = true; // in case while() loop is running
    off();
  }
  else
  {
    STOP = true; // in case while() loop is running
    on();
  }
}

void hardreboot()
{
  digitalWrite(switchPin, 1);
  delay(5000);
  digitalWrite(switchPin, 0);
  delay(5000);
  digitalWrite(switchPin, 1);
  delay(500);
  digitalWrite(switchPin, 0);
}
void shutDown()
{
  digitalWrite(switchPin, 1);
  delay(5000);
  digitalWrite(switchPin, 0);
}

void refresh() {
  Serial.println("REFRESHING");

  lastState = isOn() ? "on" : "off";

  if (!isOn()) {
    Serial.println("reporting 'computer sleeping'");
    buildDebug("switch off");
    buildDebug("computer sleeping");
    buildDebug("status sleeping");
  }
  else
  {
    Serial.println("reporting 'computer on'");
    buildDebug("switch on");
    buildDebug("computer running");
    buildDebug("status running");
  }

  String var = "";
  var = allowshutDownWhenFail ? "SHUTDOWN ALLOWED" : "SHUTDOWN FORBIDDEN";
  buildDebug(var);
  var = userOverride ? "Override Active" : "Override Inactive";
  //  _server.send(200, "text/html", var); // has to be called from js
  buildDebug(var);
  noSleep = false; // only place where this value is reset


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
  _server.send(200, "text/html", debugData);
  debugData.concat(var + "\n"); // build the debug data string for the java text area
  Serial.println("SENDING " + var + " TO HUB");

}

void getDataDebug() // called by JS
{
  // send last updated value in getDebug()


  if (debugData.length() > 5)
  {
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
