
void cool() {
  currentMode = "COOL";
  irsend.sendRaw(COOL, sizeof(COOL) / sizeof(int), khz);
  delay(DELAY_CMD);
  indexArray = getIndex("62");
  newTemp = TempArray[indexArray - 1][99];
  SetTemp(newTemp, "");// this won't send a heatingSetpoint event

  onrequested = true;
  requestTurbo = true;
  requestTurboMillis = millis();
  buildDebug("thermostatMode cool"); //send a coolingSetpoint event

}
void heat() {
  currentMode = "HEAT";
  irsend.sendRaw(HEAT, sizeof(HEAT) / sizeof(int), khz);
  delay(DELAY_CMD);
  indexArray = getIndex("85");
  newTemp = TempArray[indexArray - 1][99];
  SetTemp(newTemp, "");// this won't send a heatingSetpoint event

  onrequested = true;
  requestTurbo = true;
  requestTurboMillis = millis();
  buildDebug("thermostatMode heat"); //send a heatingSetpoint event
}
void turbo() {

  if (!turboIsOn)
  {
    turboIsOn = true;
    irsend.sendRaw(TURBO, sizeof(TURBO) / sizeof(int), khz);
    delay(DELAY_CMD);
    onrequested = true;
    //buildDebug("thermostatFanMode TURBO"); //Don't update this mode so the Hub's driver doesn't interprete as other than cool/heat
  }
  requestTurbo = false;
}

void Auto() {
  //acON();

  currentMode = "AUTO";
  irsend.sendRaw(AUTO, sizeof(AUTO) / sizeof(int), khz);
  delay(DELAY_CMD);
  //irsend.sendSAMSUNG(autoData, 32); /// SHOULD BE SENT AS HEX, not int... To be tried again later!

  onrequested = true;
  buildDebug("thermostatMode auto");
}

void acON() {

  toggleTheled = true;

  SetTocurrentMode(); // will either set to last recorded mode, or to default "heat"
  setToLastTemp();
  //newTemp = TempArray[12][99]; ; // 72 default for now...
  //SetTemp(newTemp, cmd);
  onrequested = true;
  currentMode = "ON";
  buildDebug("switch on");

}

void acOff() {

  irsend.sendRaw(OFF, sizeof(OFF) / sizeof(int), khz);
  //delay(2000);
  currentMode = "OFF";
  buildDebug("switch off");
  turboIsOn = false;
  onrequested = false;
  delay(100);
}

void _ToggleLED() {

  irsend.sendRaw(ToggleLED, sizeof(ToggleLED) / sizeof(int), khz);
  delay(DELAY_CMD);
  buildDebug("LED TOGGLED");


}

void SetTemp(uint32_t * val, String cmd) {

  irsend.sendRaw(val, 99, khz);
  delay(DELAY_CMD);
  // don't send any event from here

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
  else
  {
    buildDebug("ERROR");
  }

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

  setToLastFanMode();
  SetTocurrentMode();
  setToLastTemp();

}

void setToLastTemp() {

  if (lastRecordedTemp > 0) {
    indexArray = lastRecordedTemp;
    Serial.print("set to last recorded TEMPERATURE mode: "); Serial.println(lastRecordedTemp);

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
    Serial.println("set to last recorded FAN mode: LOW");
  }
  if (lastFanMode == "FANMED") {
    fanMed();
    Serial.println("set to last recorded FAN mode: MEDIUM");
  }
  if (lastFanMode == "FANHIGH") {
    fanHigh();
    Serial.println("set to last recorded FAN mode: HIGH");
  }

}

void SetTocurrentMode() {

  if (currentMode == "FAN") {
    fanOnly();
    Serial.println("set to last recorded mode: FAN ONLY");
    buildDebug("thermostatMode fanOn");
  }
  else if (currentMode == "AUTO") {
    Auto();
    Serial.println("set to last recorded mode: AUTO");
    buildDebug("thermostatMode auto");
  }
  else if (currentMode == "HEAT") {
    heat();
    Serial.println("set to last recorded mode: HEAT");
    buildDebug("thermostatMode heat");
  }
  else if (currentMode == "COOL") {
    cool();
    Serial.println("set to last recorded mode: COOL");
    buildDebug("thermostatMode cool");
  }
  else {
    Serial.println("No recorded mode, setting to default: heat");
    //    Auto();
    buildDebug("thermostatMode heat");
  }

}

void buildDebug(String var)
{
  Serial.println("building debug");
  smartthing.send(var); // send the current value to smartthings
  var = debugData.concat(var + "\n"); // build the debug data string
  Serial.println("concatenate");
}

void getDataDebug() // called by JS
{
  // send last updated value in getDebug()


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
  /* while (c < times) {
     digitalWrite(LED, 1);
     delay(lapse);
     digitalWrite(LED, 0);
     delay(lapse);
     c++;
    }
  */
}

//*****************************************************************************



void Reset() {

  buildDebug("reset reset");
  unsigned long stt = millis();
  while (millis() - stt < 1000) // give it time to send the debug message
  {
    loop();
  }
  ESP.restart(); // if pin failed... soft reset (contrary to ESP.restart())
}


void messageCallout(String message)
{
  // if debug is enabled print out the received message
  Blink(5, 50);
  Serial.print("Received message: '");
  Serial.print(message);
  //buildDebug(message);
  Serial.println("' ");
  String var = "Received message: '" + message + "' ";
  buildDebug(var);

  if (message.startsWith("setHeatingSetpoint") || message.startsWith("setCoolSetpoint") || message.startsWith("level") )
  {
    if (message.startsWith("setHeatingSetpoint")) {
      cmd = "heatingSetpoint";
    }
    else  if (message.startsWith("setCoolSetpoint")) {
      cmd = "coolingSetpoint";
    }
    Serial.println("");
    Serial.print("received SetPoint command: "); Serial.println(message);

    String thisMessage = message;
    char this_char[thisMessage.length() + 1];
    thisMessage.toCharArray(this_char, sizeof(this_char));

    String New = "";

    int i = 0;
    int s = message.length() + 1;
    Serial.println("extracting digits");
    for (s > 0; i < s; i++)
    {
      if (isDigit(thisMessage[i])) {
        New += thisMessage[i];
        Serial.print("New = "); Serial.println(New);
      }
    }

    indexArray = getIndex(New);
    Serial.print("Indexation done.. found index value: "); Serial.println(indexArray);
    newTemp = TempArray[indexArray - 1][99]; // now use this index to find the proper IR command to send

    Serial.print("newTemp is: "); Serial.println((uint32_t)newTemp);

    SetTemp(newTemp, cmd);

  }
  else if (message.equals("on"))
  {
    buildDebug("---2");
    if (currentMode != "" && lastRecordedTemp != 0) {
      Serial.println("setting last modes values");
      setLast(); // set the last recorded values
    }
    else {
      Serial.println("on");
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
    buildDebug("---3");
    indexArray++;
    if (indexArray > sizeof(TempArray)) {
      indexArray = sizeof(TempArray);
    }
    newTemp = TempArray[indexArray - 1][99];
    SetTemp(newTemp, "heatingSetpoint");
  }
  else if (message.equals("lowerHeatingSetpoint")) {
    buildDebug("---4");
    indexArray--;
    if (indexArray < 0) {
      indexArray = 0;
    }

    newTemp = TempArray[indexArray - 1][99];

    SetTemp(newTemp, "heatingSetpoint");

  }
  else if (message.equals("raiseCoolSetpoint") || message.equals("raiseSetpoint")) {
    buildDebug("---5");
    indexArray++;
    if (indexArray > sizeof(TempArray)) {
      indexArray = sizeof(TempArray);
    }

    newTemp = TempArray[indexArray - 1][99];

    SetTemp(newTemp, "coolingSetpoint");
  }
  else if (message.equals("lowerCoolSetpoint") || message.equals("lowerSetpoint")) {
    buildDebug("---6");
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
  Serial.println("String converted to integer");
  // search for the integer value as an index

  int i = 0;
  int s = 99;
  Serial.println("Indexation..");
  while ( IntTempArray[i] != n && i < s) {
    i++;
    Serial.println("");
    Serial.print("found: "); Serial.print(IntTempArray[i]);
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

void getTemp() // get the current temperature to display on webpage
{
  int IntTemp = IntTempArray[indexArray];
  String var = String(IntTemp);
  var = "level " + String(IntTemp);
  buildDebug(var); ;

}
