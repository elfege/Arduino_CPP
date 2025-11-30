

boolean closed()
{
  if (analogRead(Asensor) >= 1000)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void Refresh() {

    _server.send(200, "text/html", currentState);
    buildDebug("contact " + currentState);

}


void buildDebug(String var)
{
  Serial.println("building debug");
  smartthing.send(var); // send the current value to smartthings
  var = debugData.concat(var + "\n"); // build the debug data string
  Serial.println("concatenate");
}

void getDataDebug() // called by JS ONLY
{
  Serial.println("sending debug data to XMLHTML web client");
  _server.send(200, "text/html", debugData);

  if (debugData.length() > 2000)
  {
    debugData = "";
    logs = false; // stop sensor logs
    buildDebug("DATA RESET");
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

//*****************************************************************************
// Local Functions  | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************


void messageCallout(String message)
{

  if (message.equals("reset")) {
    Reset();
  }
  else if (message.equals("refresh")) {
    Refresh();
  }

  buildDebug("received_message " + message);

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

void glow() {
  int s = 1024; //ESP8266 uses 10-bit resolution for PWM generation

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
  //  delay(5); // delay must be set in the scope where it's called from
}


void Reset() {

  buildDebug("RESET_CMD RECEIVED!");
  unsigned long stt = millis();
  while (millis() - stt < 1000) // give it time to send the debug message
  {
    loop();
  }
  ESP.reset(); // ESP.reset is hard reset
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
