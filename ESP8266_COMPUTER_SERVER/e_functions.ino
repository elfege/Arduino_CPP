void on() {
  shutingDown = false;

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
    buildDebug("computer running"); // tell device handler that computer is now on and running
    buildDebug("switch on");
  }
  else {
    buildDebug("error"); // tell hub that the computer failed to turnon
  }
}

void off() {

  if (!noSleep)
  {
    buildDebug("switch off"); // inform ST HUB that the cmd has been received
    userOverride = false; // reset this value

    previousMillis = millis(); // make sure refresh() isn't triggered during this operation
    previousMillis_override_RESET = millis(); // for the unlikely chance this might be triggered during an override of the failsafe operation

    unsigned long DELAY_SLEEP = 120000;
    unsigned long DELAY_FAILSAFE = 600000;
    unsigned long DELAY_NEWATTEMPT = 60000;

    if (isOn()) {
      digitalWrite(switchPin, 1);
      delay(1000);
      digitalWrite(switchPin, 0);
      unsigned long Start = millis();
      while (isOn() && millis() - Start < DELAY_SLEEP)
      {
        master();
      }
      // some configs might allow sleep mode but something wrong makes that the PWM of the board turns the computer back on
      delay(5000); // give time for a motherboard to wake back up a computer when IRQ or some driver is fucking around

      if (!isOn()) // if off, then it's a success,
      {
        Serial.println("COMPUTER SUCCESFULLY TURNED OFF");
        buildDebug("computer off"); // tell device handler that computer is now off
        buildDebug("switch off"); // tell device handler that computer is now off
        //so cancel any further operation
        return;
      }

      buildDebug("computer running"); //
      buildDebug("switch on"); //
      /*******************FAILSAFE*****************/
      // if still on after 120 seconds, means failed (might have simply turned on the screen)
      Start = millis();
      while (isOn() && !userOverride && millis() - Start < DELAY_FAILSAFE)
      {
        digitalWrite(switchPin, 1);
        delay(1000);
        digitalWrite(switchPin, 0);

        unsigned long s = millis();
        while (isOn() && millis() - s < DELAY_NEWATTEMPT)
        {
          master();
        }
        if (isOn() && !userOverride)
        {
          buildDebug("computer FAILEDtoTurnOff"); //
          buildDebug("switch FAILEDtoTurnOff"); //
          if (allowshutDownWhenFail)
          {
            shutDown();
            buildDebug("SHUTDOWN ALLOWED");
          }
        }
        else
        {
          Serial.println("COMPUTER TURNED OFF _ RETRIEVE SUCCESSFUL");
          buildDebug("computer off"); // tell device handler that computer is now off
          buildDebug("switch off"); // tell device handler that computer is now off
          buildDebug("computer retrieveOk");
          return;
        }
      }
    }
    else {
      Serial.println("ALREADY OFF");
    }

    if (isOn()) {
      buildDebug("computer off"); // tell device handler that computer is now off
      buildDebug("switch off");
    }
    else {
      buildDebug("computer running"); // tell hub that the computer failed to turn off
      buildDebug("switch on");
    }
  }
  else
  {
    String var = "NOSLEEP MODE";
    _server.send(200, "text/html", var);
    buildDebug(var);

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
    off();
  }
  else
  {
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
  if (!isOn() && !shutingDown) {
    Serial.println("reporting sleeping");
    buildDebug("computer sleeping");
  }
  else if (!isOn() && shutingDown) {
    Serial.println("reporting shutingDown");
    buildDebug("computer shutingDown");
  }
  else if (isOn())
  {
    Serial.println("reporting on");
    buildDebug("computer running");
  }
  else {
    buildDebug("ERROR");
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
