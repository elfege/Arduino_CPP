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

  if (message.equals("on") || message.equals("open"))
  {
    On();
  }
  else if (message.equals("off"))
  {
    Off();
  }
  else if (message.equals("stop"))
  {
    Stop();
  }
  else if (message.equals("refresh"))
  {
    Refresh();
  }
  else if (message.equals("reset"))
  {
    Reset();
  }
}

void Refresh() {
  String var = "";

  if (CurtainsAreOpen) {
    var = "curtains open";
  }
  if (!CurtainsAreOpen) {
    var = "curtains closed";
  }

  buildDebug(var);
}




void On() {

  digitalWrite(OpenPin, 1);
  delay(500);
  digitalWrite(OpenPin, 0);
  CurtainsAreOpen = true;
  // String var = "switch on";
  //  buildDebug(var);
  //  delay(500);
  String var = "curtains open";
  buildDebug(var);


}
void Off() {
  digitalWrite(ClosePin, 1);
  delay(500);
  digitalWrite(ClosePin, 0);
  CurtainsAreOpen = false;
  // String var = "switch off";
  //  buildDebug(var);
  //  delay(500);
  String var = "curtains closed";
  buildDebug(var);

}

void Stop() {
  digitalWrite(StopPin, 1);
  delay(500);
  digitalWrite(StopPin, 0);
  CurtainsAreOpen = false;
  String var = "curtains stopped";
  buildDebug(var);
}

void Reset() {
  String var = "curtains reset";
  buildDebug(var);
  ESP.restart();
}

void hardReset()
{
  ESP.reset();
}

void buildDebug(String var)
{
  Serial.println("building debug");
  smartthing.send(var); // send the current value to smartthings
  debugData.concat(var + "\n"); // build the debug data string
  Serial.println("SENDING " + var + " TO HUB");

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
  String var = "Mac: " + WiFi.macAddress() + " IP:" + ip + " " + "SSID: " + String(WiFi.SSID()) + " " + "RSSI: " + String(WiFi.RSSI());
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
    Serial.print(result);
  }
  else if (dispalyDays > 1)
  {
    result = result + dispalyDays + " days";
    Serial.print(result);
  }
  if (displayHours == 1)
  {
    result = result + displayHours + " hour ";
    Serial.print(result);
  }
  else if (displayHours > 1)
  {
    result = result + displayHours + " hours ";
    Serial.print(result);
  }
  if (displayMinutes == 1)
  {
    result = result + displayMinutes + " minute ";
    Serial.print(result);
  }
  else if (displayMinutes > 1)
  {
    result = result + displayMinutes + " minutes ";
    Serial.print(result);
  }
  if (displaySeconds == 1)
  {
    result = result + displaySeconds + " second ";
    Serial.print(result);
  }
  else if (displaySeconds > 1)
  {
    result = result + displaySeconds + " seconds ";
    Serial.print(result);
  }

  _server.send(200, "text/html", result);
  return result;
}


void glow() {
  int s = 256;

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
  delay(5);
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
