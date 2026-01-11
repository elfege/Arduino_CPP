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

  if (message.equals("on"))
  {
    On();
  }
  else if (message.equals("off"))
  {
    Off();
  }
  else if (message.equals("open")) // triggers nonStopPurge from ST or HE
  {
    nonStopPurge();
  }
  else if (message.equals("refresh"))
  {
    Refresh();
  }
}

void Refresh() {
  if (!Running) {
    Serial.println("reporting off");
    buildDebug("switch off");
  }
  else
  {
    Serial.println("reporting on");
    buildDebug("switch on");
  }

  Blink(5, 50);
}

void On() {
  buildDebug("switch on");
  cancel = false;
  digitalWrite(Signal, 1);

  Running = true;

  occ = 1;
  lapse = 100;
  DELAY = 100;

  //  timeRemain = 0;// reset countdown
  unsigned long start = millis();
  //  unsigned long elapsed = 0;
  previousMillisPURGEduration = millis();
  unsigned long lastPrint = 0;
  int printTimer = 1; // in seconds

  unsigned long timeRemain = 0;
  int pumptime = 45; // pumping time in seconds

  while (millis() - start < pumptime * 1000 && !cancel) {
    loop();// allow http interruption command
    elapsed = millis() - start;
    timeRemain = ((pumptime * 1000) - elapsed) / 1000;

    if (millis() - lastPrint >= printTimer * 1000) {
      Serial.println("");
      Serial.print("time remaining = "); Serial.print(timeRemain); Serial.print(" seconds");
      lastPrint = millis();

    }

  }
  occ = 1;
  lapse = 200;
  DELAY = 500;
  digitalWrite(Signal, 0);
  Running = false;

}

void nonStopPurge()
{
  buildDebug("valve open");
  digitalWrite(Signal, 1);


}

void Off() {
  Running = false;
  cancel = true;// cancel any current pumping operation

  digitalWrite(Signal, 0);// stop operations
  buildDebug("switch off");

}

boolean wet()
{

  bool result = false;
  //    int m = 0; // number of measurements
  //    int c = 0; // number of loops
  //
  //    int testDelay = 5;
  //    unsigned long Start = millis();
  //    while (analogRead(A0) >= 500 && millis() - Start < testDelay) // must be consistent for testDelay millis
  //    {
  //      //    delay(1);
  //      ;
  //    }
  result = analogRead(A0) >= 800; //millis() - Start >= testDelay; // if elapsed time is equal or more than testDelay millis, then signal has been consistent so return true.
  wetState = result ? "wet" : "dry";

  return result;
}


void buildDebug(String var)
{
  Serial.println("building debug");
  smartthing.send(var); // send the current value to smartthings
  debugData.concat(var + "\n"); // build the debug data string
  Serial.println("concatenate");
}

void getDataDebug() // called by JS
{
  // send last updated value in getDebug()

  Serial.println("sending debug data to server");
  _server.send(200, "text/html", debugData);
  if (debugData.length() > 2000)
  {
    debugData = "";
    logs = false; // stop sensor logs
    //buildDebug("DATA RESET");
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
void glow() {
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
  //  delay(1);

}
//*****************************************************************************



void Reset() {
  ESP.restart();
}
