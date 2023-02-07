void messageCallout(String message)
{
  // if debug is enabled print out the received message
  if (isDebugEnabled)
  {
    term.print("Received message: '");
    term.print(message);
    term.println("' ");
  }

  // if message contents equals to 'on' then call on() function
  // else if message contents equals to 'off' then call off() function

  if (message.equals("on"))
  {
    On();
    onRequestedByUser = true;
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
  if (!scheduledRunning) {
    term.println("reporting off");
    buildDebug("switch off");
  }
  else
  {
    term.println("reporting on");
    buildDebug("switch on");
  }
  if (wet())
  {
    buildDebug("water wet");
  }
  else
  {
    buildDebug("water dry");
  }

  //Blink(5, 50);
}

void On() {
  buildDebug("switch on");
  digitalWrite(Signal, 1);
  scheduledRunning = true;
  nonStopPurging = false;
  maxTimeout = millis();
  previousMillisRun = millis();
  pumping = true;
}

void nonStopPurge()
{
  nonStopPurgeMillis = millis();
  buildDebug("valve open");
  nonStopPurgeMillis = millis();
  scheduledRunning = false; // make sure it's not a scheduled run (prevents stopping after 45 secs)
  nonStopPurging = true;
  previousMillisRun = millis();
  digitalWrite(Signal, 1);
  pumping = true;
}

void Off() {
  if (!wet())
  {
    onRequestedByUser = false;
    scheduledRunning = false;
    nonStopPurging = false;
    pumping = false;
    digitalWrite(Signal, 0);// stop operations
    buildDebug("switch off");
  }
  else
  {
    buildDebug("status noturnOffBecauseStillWet");
  }
}

boolean wet()
{
  bool result = false;
  int m = 0; // number of measurements
  int c = 0; // number of loops

  int testDelay = 1500;
  unsigned long Start = millis();
  while (analogRead(A0) >= 800 && millis() - Start < testDelay) // must be consistent for testDelay duration to return positive
  {
    serverAndHub();
  }
  delay(1);
  if (millis() - Start < testDelay) { // if elapsed time is < to testDelay, then signal wasn't consistent
    result = false;
  }
  else
  {
    result = analogRead(A0) >= 1000; // else, check a last time for consistent positive signal
  }
  wetState = result ? "wet" : "dry";

  return result;
}


void buildDebug(String var)
{
  term.println(var);
  smartthing.send(var); // send the current value to smartthings
  debugData.concat(var + "\n"); // build the debug data string
  //term.println("concatenate");
}

void getDataDebug() // called by JS
{
  // send last updated value in getDebug()

  //term.println("sending debug data to server");
  //_server.send(200, "text/html", debugData);
  if (debugData.length() > 2000)
  {
    debugData = "";
    logs = false; // stop sensor logs
    //buildDebug("DATA RESET");
  }
}
void getMac()
{
  String var = WiFi.macAddress();
  _server.send(200, "text/html", var);
}
void getIP()
{
  String var = WiFi.localIP().toString();
  _server.send(200, "text/html", var);
}
void getSSID()
{
  String var = String(WiFi.SSID());
  _server.send(200, "text/html", var);
}
void getRSSI()
{
  String var = String(WiFi.RSSI());
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
  unsigned long totalTimeSeconds = (millis() - millisBoot) / 1000; // we want the time at the end of setup, not since last reset
  unsigned long totalTimeMinutes = (millis() - millisBoot) / 1000 / 60;
  unsigned long TotalTimeHours = (millis() - millisBoot) / 1000 / 60 / 60;
  unsigned long TotalTimeDays = (millis() - millisBoot) / 1000 / 60 / 60 / 24;

  unsigned long displaySeconds = ((millis() - millisBoot) - (totalTimeMinutes * 60 * 1000)) / 1000;
  unsigned long displayMinutes = ((millis() - millisBoot) - (TotalTimeHours * 1000 * 60 * 60)) / 1000 / 60;
  unsigned long displayHours = ((millis() - millisBoot) - (TotalTimeDays * 1000 * 60 * 60 * 24)) / 1000 / 60 / 60;
  unsigned long dispalyDays = TotalTimeDays;


  //term.println("LOOP time = " + String(elapsed) + "ms");
  String result = "Time since last boot: ";
  timerResult = "";

  if (dispalyDays >= 1)
  {
    result = result + dispalyDays + " d";
    timerResult = dispalyDays + " d";
    //term.print(result);
  }
  if (displayHours >= 1)
  {
    result = result + displayHours + " h ";
    timerResult = displayHours + " h";
    //term.print(result);
  }
  if (displayMinutes >= 1)
  {
    result = result + displayMinutes + " m ";
    timerResult = displayMinutes + " m";
    //term.print(result);
  }
  if (displaySeconds >= 1)
  {
    result = result + displaySeconds + " s ";
    timerResult = displaySeconds + " s";
    //term.print(result);
  }

  result += " " + lastBootDate; // add the boot date stamp


  return result;
}

void termSteps() {
  char c;
  String cmd = "";

  if (term.available())
  {
    while (term.available())
    {
      c = term.read();
      cmd += c;
    }
    term.print("Ok I received : "); term.print(cmd);
    term.println();
    if (cmd == "reset")
    {
      term.println("RESETING ESP NOW");
      // give time for the terminal to display this message
      unsigned long s = millis();
      while (millis() - s < 1000)
      {
        serverAndHub();
      }
      Reset();
    }
  }
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
    //term.print("GLOW "); term.println(i);
  }
  else if (!goUp && glw > 0) {
    glw--;
    analogWrite(LED, glw);
    //term.print("GLOW "); term.println(i);
  }
  //  delay(1);

}
//*****************************************************************************

void updateNtpTime() {

  timeClient.update();
  unsigned long Start = millis();
  while (!timeClient.updated() && millis() - Start < 2000)
  {
    delay(1);
    serverAndHub();
  }
  if (!timeClient.updated())
  {
    if (ntpFailures < 3)
    {
      term.println("******TIME FAILED TO UPDATE FROM NTP SERVER******");
      ntpFailed = true; // loop will call updateNtpTime() again
      ntpFailures++;
    }
    else
    {
      term.println("******TOO MANY FAILS - CANCELLING ALL NTP REQUESTS UNTIL RESET******");
      stopNtp = true;
    }
    serverAndHub();
  }
  else
  {
    if (ntpFailed) // if previous attempt failed, notify success
    {
      term.println("******TIME SUCCESSFULY UPDATED FROM NTP SERVER******");
    }
    ntpFailed = false;
  }

  return;

  term.println("UTC : " + timeClient.getFormattedUTCTime());
  term.println("UTC : " + timeClient.getFormattedUTCDateTime());
  term.println("LOC : " + timeClient.getFormattedTime());
  term.println("LOC : " + timeClient.getFormattedDateTime());
  term.println("UTC EPOCH : " + String(timeClient.getUTCEpochTime()));
  term.println("LOC EPOCH : " + String(timeClient.getEpochTime()));

  // Function test
  // Without leading 0
  term.println(String("UTC : ") + timeClient.getUTCHours() + ":" + timeClient.getUTCMinutes() + ":" + timeClient.getUTCSeconds() + " " +
               timeClient.getUTCDoW() + " " + timeClient.getUTCDay() + "/" + timeClient.getUTCMonth() + "/" + timeClient.getUTCYear() + " or " +
               timeClient.getUTCDay() + " " + timeClient.getUTCMonthStr() + " " + timeClient.getUTCYear());
  // With leading 0
  term.println(String("UTC : ") + timeClient.getUTCStrHours() + ":" + timeClient.getUTCStrMinutes() + ":" + timeClient.getUTCStrSeconds() + " " +
               timeClient.getUTCDoW() + " " + timeClient.getUTCDay() + "/" + timeClient.getUTCMonth() + "/" + timeClient.getUTCYear() + " or " +
               timeClient.getUTCDay() + " " + timeClient.getUTCMonthStr() + " " + timeClient.getUTCYear());
  // Without leading 0
  term.println(String("LOC : ") + timeClient.getHours() + ":" + timeClient.getMinutes() + ":" + timeClient.getSeconds() + " " +
               timeClient.getDoW() + " " + timeClient.getDay() + "/" + timeClient.getMonth() + "/" + timeClient.getYear() + " or " +
               timeClient.getDay() + " " + timeClient.getMonthStr() + " " + timeClient.getYear());
  // With leading 0
  term.println(String("LOC : ") + timeClient.getStrHours() + ":" + timeClient.getStrMinutes() + ":" + timeClient.getStrSeconds() + " " +
               timeClient.getDoW() + " " + timeClient.getDay() + "/" + timeClient.getMonth() + "/" + timeClient.getYear() + " or " +
               timeClient.getDay() + " " + timeClient.getMonthStr() + " " + timeClient.getYear());

}


void Reset() {
  ESP.reset();
}
