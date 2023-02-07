
void cool() {
  currentMode = "COOL";
  term.println("cool()");
  irsend.sendRaw(COOL, sizeof(COOL) / sizeof(int), khz);
  delay(3000);

  indexArray = getIndex("66"); // set to a lower default value
  newTemp = TempArray[indexArray - 1][99];
  SetTemp(newTemp, "");// this won't send a coolingSetpoint event
  
  

  //  requestTurbo = true; // reset turbo() cmd
  //  turboIsOn = false;
  //  requestTurboMillis = millis();

  //buildDebug("coolingSetpoint 74"); // send a cooling setpoint so app can adjust if 74 is not good
  buildDebug("thermostatMode cool"); //send a thermostatMode event
  buildDebug("thermostatOperatingState cooling");

}
void heat() {
  currentMode = "HEAT";
  irsend.sendRaw(HEAT, sizeof(HEAT) / sizeof(int), khz); //sets to auto 74
  delay(DELAY_CMD);
  indexArray = getIndex("85");
  newTemp = TempArray[indexArray - 1][99];
  SetTemp(newTemp, "");// this won't send a heatingSetpoint event

  //  requestTurbo = true; // reset turbo() cmd
  //  turboIsOn = false;
  //  requestTurboMillis = millis();

  //buildDebug("heatingSetpoint 74"); // send a cooling setpoint so app can adjust if 72 is not good
  buildDebug("thermostatMode heat"); //send a heatingSetpoint event
  buildDebug("thermostatOperatingState heating");
}

void turbo() {

  if (!turboIsOn)
  {
    turboIsOn = true;
    onrequested = true;

    irsend.sendRaw(TURBO, sizeof(TURBO) / sizeof(int), khz); // For some reason this command doesn't always go through
    delay(DELAY_CMD);
    buildDebug("TurboCmd Sent");

    //    newTurbo(); // this is a workaround... that we gave up for now

    lastTurboActive = millis();
  }
  requestTurbo = false;
}

void newTurbo() // not in use for now (problem avoiding it running too often or not often enough) / might have to work with a sensor!
{
  if (currentMode == "HEAT") // Use static values for heat and cool if defined
  {
    indexArray = getIndex("85");
    newTemp = TempArray[indexArray - 1][99];
    irsend.sendRaw(newTemp, 99, khz);
    delay(DELAY_CMD);
    buildDebug("turbo newMethod");
  }
  else if (currentMode == "COOL")
  {
    indexArray = getIndex("62");
    newTemp = TempArray[indexArray - 1][99];
    irsend.sendRaw(newTemp, 99, khz);
    delay(DELAY_CMD);
    buildDebug("turbo newMethod");
  }
  else {
    // if no condition is met yet, use classic turbo cmd
    irsend.sendRaw(TURBO, sizeof(TURBO) / sizeof(int), khz); // For some reason this command doesn't always go through
    delay(DELAY_CMD);
    buildDebug("turbo classic");
  }
}

void Auto() {
  //acON();

  currentMode = "AUTO";
  term.println("auto()");
  irsend.sendRaw(AUTO, sizeof(AUTO) / sizeof(int), khz);
  delay(2000);
  //irsend.sendSAMSUNG(autoData, 32); /// SHOULD BE SENT AS HEX, not int... To be tried again later!

  onrequested = true;
  //  requestTurbo = true; // reset turbo() cmd
  //  turboIsOn = false;
  //  requestTurboMillis = millis(); // so it is sent again wit

  buildDebug("thermostatMode auto");
}

void acON() {

  isOff = false;
  term.println("acON()");
  irsend.sendRaw(ON, sizeof(ON) / sizeof(int), khz);
  delay(3000);

  onrequested = true;
  requestTurbo = true; // reset turbo() cmd
  requestTurboMillis = millis(); // so it is sent again within seconds

  currentMode = "ON";

  buildDebug("switch on");

}

void acOff() {

  irsend.sendRaw(OFF, sizeof(OFF) / sizeof(int), khz); //works on C&H...
  //  delay(3000);
  //  irsend.sendRaw(OFFA, sizeof(OFF) / sizeof(int), khz);
  //  delay(100);
  //  irsend.sendRaw(OFFB, sizeof(OFF) / sizeof(int), khz);
  //  delay(2000);


  //irsend.sendSAMSUNG(offHex, 32);
  //delay(2000);

  currentMode = "OFF";
  buildDebug("switch off");
  buildDebug("thermostatMode off");
  turboIsOn = false;
  onrequested = false;
  ledToggleRequested = false;
  isOff = true;
  delay(100);
}

void _ToggleLED() {

  if (!ledToggleRequested)
  {
    irsend.sendRaw(ToggleLED, sizeof(ToggleLED) / sizeof(int), khz);
    delay(DELAY_CMD);
    ledToggleRequested = true;
    buildDebug("LED TOGGLED");
  }
}

void SetTemp(uint32_t * val, String cmd) {

  irsend.sendRaw(val, 199, khz);
  delay(DELAY_CMD);

  // every new setpoint cmd cancels the turbo() operation due to the fact that we use raw cmd data
  onrequested = true; // every setpoint request turns on the AC

  // DEPRECATED DOESN'T DO GOO AT MID SEASON AUTO WILL HEAT UP EVEN WHEN SET POINT IS 71
  //  requestTurbo = true; // reset turbo() cmd
  //  turboIsOn = false;
  //  requestTurboMillis = millis(); // so the turbo() method is called within seconds

  newTurbo();

  // the device driver will need to know every setpoint in order for the app to correct if, for example, IR signal didn't go through
  buildDebug(cmd + " " + lastSetPoint); // lastSetPoint is a global variable / it's the set point requested through ajax or Hub and recorded then

}


void Refresh() {

  //  if (currentMode == "TURBO") {
  //    buildDebug("thermostatFanMode turbo");
  //  }
  //  else
  if (currentMode == "AUTO") {
    buildDebug("thermostatMode auto");
  }
  else if (currentMode == "HEAT") {
    buildDebug("thermostatMode heat");
  }
  else if (currentMode == "COOL") {
    buildDebug("thermostatMode cool");
  }
  else if (currentMode == "OFF") {
    buildDebug("thermostatMode off");
  }
  
  String turboState = turboIsOn ? "turbo on" : "turbo off";
  buildDebug(turboState);

}

void refreshFanState() {

  if (lastFanMode == "FANLOW") {
    buildDebug("fanSpeed low");
  }
  else if (lastFanMode == "FANMED") {
    buildDebug("fanSpeed medium");
  }
  else if (lastFanMode == "FANHIGH") {
    buildDebug("fanSpeed high");
  }
  else {
    fanMed();
    //buildDebug("fanSpeed ERROR");
  }

}

void refreshSetpoint() {
  String space = " ";
  String thisCmd;

  if (currentMode == "HEAT") {
    thisCmd = "heatingSetpoint";
    buildDebug(thisCmd + space + StringTemp);
  }
  else if (currentMode == "COOL") {
    thisCmd = "collingSetpoint";
    buildDebug(thisCmd + space + StringTemp);
  }


}

/////////// fan speed commands

void fanLow() {
  lastFanMode = "FANLOW";
  onrequested = true;

  // FanSpeed = FANREQUEST_LOW();
  // irsend.sendRaw(FanSpeed, sizeof(FanSpeed) / sizeof(int), khz);

  //delay(DELAY_CMD);
  //SetTemp(newTemp, cmd); // reinstate proper temperature
  buildDebug("fanSpeed low");
  //delay(DELAY_CMD);

  lastFanMode = "FANLOW";

}

void fanMed() {

  lastFanMode = "FANMED";
  onrequested = true;
  // FanSpeed = FanSpeed = FANREQUEST_MED();
  // irsend.sendRaw(FanSpeed, sizeof(FanSpeed) / sizeof(int), khz);

  //delay(DELAY_CMD);
  //SetTemp(newTemp, cmd); // reinstate proper temperature
  //delay(DELAY_CMD);
  buildDebug("fanSpeed medium");

}

void fanHigh() {

  lastFanMode = "FANHIGH";
  onrequested = true;

  // FanSpeed = FANREQUEST_HIGH();
  // irsend.sendRaw(FanSpeed, sizeof(FanSpeed) / sizeof(int), khz);

  //delay(DELAY_CMD);
  //SetTemp(newTemp, cmd); // reinstate proper temperature
  //delay(DELAY_CMD);

  buildDebug("fanspeed high");

}


////////////////// commands modes
void fanOnly() {
  //acON();

  currentMode = "FAN";
  irsend.sendRaw(FAN, sizeof(FAN) / sizeof(int), khz);
  delay(DELAY_CMD);
  onrequested = true;
  buildDebug("thermostatMode fan");


}



void setLast() {

  //setToLastFanMode();
  SetTocurrentMode();
  setToLastTemp();

}

void setToLastTemp() {

  if (lastRecordedTemp > 0) {
    indexArray = lastRecordedTemp;
    term.print("set to last recorded TEMPERATURE mode: "); term.println(lastRecordedTemp);

    newTemp = TempArray[indexArray - 1][99]; // now use this index to find the proper IR command to send

    SetTemp(newTemp, cmd);
  }
  else
  {
    buildDebug("NO LASTTEMP");
  }
}

void setToLastFanMode() {
  if (lastFanMode == "FANLOW") {
    fanLow();
    term.println("set to last recorded FAN mode: LOW");
  }
  if (lastFanMode == "FANMED") {
    fanMed();
    term.println("set to last recorded FAN mode: MEDIUM");
  }
  if (lastFanMode == "FANHIGH") {
    fanHigh();
    term.println("set to last recorded FAN mode: HIGH");
  }

}

void SetTocurrentMode() {

  if (currentMode == "FAN") {
    fanOnly();
    term.println("set to last recorded mode: FAN ONLY");
    buildDebug("thermostatMode fanOn");
  }
  else if (currentMode == "AUTO") {
    Auto();
    term.println("set to last recorded mode: AUTO");
    buildDebug("thermostatMode auto");
  }
  else if (currentMode == "HEAT") {
    heat();
    term.println("set to last recorded mode: HEAT");
    buildDebug("thermostatMode heat");
  }
  else if (currentMode == "COOL") {
    cool();
    term.println("set to last recorded mode: COOL");
    buildDebug("thermostatMode cool");
  }
  else {
    term.println("No recorded mode, setting to default: heat");
    //    Auto();
    buildDebug("thermostatMode heat");
  }

}

void buildDebug(String var)
{
  _server.send(200, "text/html", var);
  if (logs)term.println(var + " at " +  String(TimeInfos()));
  smartthing.send(var); // send the current value to smartthings
}

void getMac()
{
  String ip = WiFi.localIP().toString();
  String var = "Mac: " + WiFi.macAddress() + " IP:" + ip + " " + String(WiFi.SSID());
  _server.send(200, "text/html", var);
}


void log()
{
  term.println("data is too big to show here. Use Arduino IDE instead");
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
  String result = "Up time: ";
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

//*****************************************************************************



void Reset() {

  buildDebug("reset reset");
  unsigned long stt = millis();
  while (millis() - stt < 1000) // give it time to send the debug message
  {
    serverAndHub();
  }
  ESP.reset();
}


void messageCallout(String message)
{
  // if debug is enabled print out the received message
  //Blink(5, 50);
  term.print("Received message: '");
  term.print(message);
  //buildDebug(message);
  term.println("' ");
  String var = "Received message: '" + message + "' ";
  //buildDebug(var);

  if (message.startsWith("setHeatingSetpoint") || message.startsWith("setCoolSetpoint") || message.startsWith("level") )
  {
    if (message.startsWith("setHeatingSetpoint")) {
      cmd = "heatingSetpoint";
    }
    else  if (message.startsWith("setCoolSetpoint")) {
      cmd = "coolingSetpoint";
    }
    term.println("");
    term.print("received SetPoint command: "); term.println(message);

    String thisMessage = message;
    char this_char[thisMessage.length() + 1];
    thisMessage.toCharArray(this_char, sizeof(this_char));

    String New = "";

    int i = 0;
    int s = message.length() + 1;
    term.println("extracting digits");
    for (s > 0; i < s; i++)
    {
      if (isDigit(thisMessage[i])) {
        New += thisMessage[i];
        //term.print("New = "); term.println(New);
      }
    }

    lastSetPoint = New; // remember this as last requested set point for future reference
    indexArray = getIndex(New);
    //term.print("Indexation done.. found index value: "); term.println(indexArray);
    newTemp = TempArray[indexArray - 1][99]; // now use this index to find the proper IR command to send

    term.print("newTemp is: "); term.println((uint32_t)newTemp);

    SetTemp(newTemp, cmd);
    String var = cmd + " " + String(New); // "New" is a String while newTemp is a charArray
    buildDebug(var); // parse event

  }
  else if (message.equals("on"))
  {
    ////buildDebug("---2");
    if (currentMode != "" && lastRecordedTemp != 0) {
      term.println("setting last modes values");
      setLast(); // set the last recorded values
    }
    else {
      term.println("on");
      acON();
    }
  }
  else if (message.equals("off"))
  {
    acOff();
  }
  else if (message.equals("refresh"))
  {
    Refresh();
  }
  else if (message.equals("Ledtoggle")) {

    _ToggleLED();
  }
  else if (message.equals("raiseHeatingSetpoint")) {
    ////buildDebug("---3");
    indexArray++;
    if (indexArray > sizeof(TempArray)) {
      indexArray = sizeof(TempArray);
    }
    newTemp = TempArray[indexArray - 1][99];
    SetTemp(newTemp, "heatingSetpoint");
  }
  else if (message.equals("lowerHeatingSetpoint")) {
    ////buildDebug("---4");
    indexArray--;
    if (indexArray < 0) {
      indexArray = 0;
    }

    newTemp = TempArray[indexArray - 1][99];

    SetTemp(newTemp, "heatingSetpoint");

  }
  else if (message.equals("raiseCoolSetpoint") || message.equals("raiseSetpoint")) {
    //buildDebug("---5");
    indexArray++;
    if (indexArray > sizeof(TempArray)) {
      indexArray = sizeof(TempArray);
    }

    newTemp = TempArray[indexArray - 1][99];

    SetTemp(newTemp, "coolingSetpoint");
  }
  else if (message.equals("lowerCoolSetpoint") || message.equals("lowerSetpoint")) {
    //buildDebug("---6");
    indexArray--;
    buildDebug("indexing cmd");
    if (indexArray < 0) {
      indexArray = 0;
    }

    newTemp = TempArray[indexArray - 1][99];

    SetTemp(newTemp, "coolingSetpoint");

  }
  else if (message.equals("cool")) {
    cool();
  }
  else if (message.equals("heat")) {
    heat();
  }
  else if (message.equals("auto")) {
    //acON();
    Auto();
  }
  else if (message.equals("fan")) {
    fanOnly();

  }
  else if (message.equals("fanlow")) {
    fanLow();
  }
  else if (message.equals("fanmed")) {
    fanMed();
  }
  else if (message.equals("fanhigh")) {
    fanHigh();
  }
  else if (message.equals("turbo")) {
    turbo();
  }
  else if (message.equals("reset"))
  {
    Reset();
  }

  else if (message.equals("refresh")) {
    Refresh();
  }

  delay(100);
}

int getIndex(String val)
{
  int n = val.toInt();  //convert String into a number
  term.println("String converted to integer");
  // search for the integer value as an index

  int i = 0;
  int s = 99;
  term.println("Indexation..");
  while ( IntTempArray[i] != n && i < s) {
    i++;
    //term.println("");
    //term.print("found: "); term.println(IntTempArray[i]);
  }
  return i;
}

boolean isOn() {
  bool result = false;
  if (analogRead(Asensor) > 800 || onrequested) {
    result = true;
  }
  return result;
}

void glow() {
  int s = 256; //ESP8266 uses 10-bit resolution for PWM generation / 1024 is max. 256 allows for faster glow

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
  //  delay(5); // delay must be set in the scope where it's called from
}

void getTemp() // get the current temperature to display on webpage
{
  int IntTemp = IntTempArray[indexArray];
  String var = String(IntTemp);
  var = "level " + String(IntTemp);
  buildDebug(var); ;

}
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
