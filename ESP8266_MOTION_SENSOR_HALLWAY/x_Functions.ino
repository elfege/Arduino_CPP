void messageCallout(String message)
{

  Serial.print("Received message: '");
  Serial.print(message);
  Serial.println("' ");

  // if message contents equals to 'on' then call on() function
  // else if message contents equals to 'off' then call off() function

  if (message.equals("refresh"))
  {
    Refresh();
  }
  if (message.equals("reset"))
  {
    Reset();
  }
}

void buildDebug(String var)
{
  Serial.println("seding " + var + " to the Hubs");
  smartthing.send(var); // send the current value to smartthings
  Serial.println("building debug");
  if (var == preserved) // save time tag after fresh reboot
  {
    //    preserved.concat(" ---- " + String(ntpRequest()) + "\n"); // permanent time tag for that one
    debugData.concat(preserved + "\n"); // add it now to the main string
  }
  else if (debugData.length() == 0) // add reboot time info to the main string either after reboot or after string data reset
  {
    debugData.concat(preserved + "\n");
  }
  else
  {
    debugData.concat(var + " ---- " + String(ntpRequest()) + "\n"); // build the debug data string
  }
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
  String var = "Mac: " + WiFi.macAddress() + " IP:" + ip + " " + String(WiFi.SSID());
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
    // Serial.println(result);
  }
  else if (dispalyDays > 1)
  {
    result = result + dispalyDays + " days";
    // Serial.println(result);
  }
  if (displayHours == 1)
  {
    result = result + displayHours + " hour ";
    //Serial.println(result);
  }
  else if (displayHours > 1)
  {
    result = result + displayHours + " hours ";
    //Serial.println(result);
  }
  if (displayMinutes == 1)
  {
    result = result + displayMinutes + " minute ";
    //Serial.println(result);
  }
  else if (displayMinutes > 1)
  {
    result = result + displayMinutes + " minutes ";
    // Serial.println(result);
  }
  if (displaySeconds == 1)
  {
    result = result + displaySeconds + " second ";
    // Serial.println(result);
  }
  else if (displaySeconds > 1)
  {
    result = result + displaySeconds + " seconds ";
    //  Serial.println(result);
  }

  _server.send(200, "text/html", result);

  return result;
}

String TimingInfos()
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
  String timerResult = "";

  if (dispalyDays == 1)
  {
    timerResult = timerResult + dispalyDays + " day";
    Serial.println(result);
  }
  else if (dispalyDays > 1)
  {
    timerResult = timerResult + dispalyDays + " days";
    Serial.println(result);
  }
  if (displayHours == 1)
  {
    timerResult = timerResult + displayHours + " hour ";
    Serial.println(result);
  }
  else if (displayHours > 1)
  {
    timerResult = timerResult + displayHours + " hours ";
    Serial.println(result);
  }
  if (displayMinutes == 1)
  {
    timerResult = timerResult + displayMinutes + " minute ";
    Serial.println(result);
  }
  else if (displayMinutes > 1)
  {
    timerResult = timerResult + displayMinutes + " minutes ";
    Serial.println(result);
  }
  if (displaySeconds == 1)
  {
    timerResult = timerResult + displaySeconds + " second ";
    Serial.println(result);
  }
  else if (displaySeconds > 1)
  {
    timerResult = timerResult + displaySeconds + " seconds ";
    Serial.println(result);
  }

  return timerResult;
}

void Refresh() {

  if (motion())
  {
    String var = "motion active";
    buildDebug(var);
  }
  else
  {
    String var = "motion inactive";
    buildDebug(var);
  }
  // PING();
}

boolean motion()
{
  /*
     In “H” mode the output pin Dout will go high (3.3V) when a person is detected within range and
     goes low after a particular time (time is set by potentiometer).
     In this mode the output pin will go high irrespective of whether
     the person is still present inside the range or has left the area.
     In “I” mode the output pin Dout will go high (3.3V) when  a person is detected within range and will
     stay high as long as he/she stays within the limit of the Sensors range. Once the person has left the
     area the pin will go low after the particular time which can be set using the potentiometer.
  */

  val = analogRead(ASensor);
  bool result = val >= 800;
  //  Serial.println(val);
  return result;
}


void Blink(int times, int lapse) {
  int cblink = 0;
  while (cblink < times && !noblink) {
    digitalWrite(LED, 1);
    delay(lapse);
    digitalWrite(LED, 0);
    delay(lapse);
    cblink++;
  }
 
}

void glow(int Delay) {
  int s = 255;

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
  delay(Delay);

}

void Reset() {
  ESP.restart();
}

void hardReset() {
  ESP.reset(); // esp.reset is hard reset
}

void PING() {
  if (Ping.ping(remote_ip)) {
    Serial.println("ping successful!!");
    smartthing.send("ping successful");
  } else {
    Serial.println("Error :(");
    smartthing.send("ping failed");
    delay(1000);
    Reset();
  }

}
