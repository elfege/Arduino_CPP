


void acON() {


  isOn = true;
  SetToLastMode(); // will either set to last recorded mode, or to default "AUTO"
  delay(100);
  _ToggleLED();
  delay(100);
  setToLastTemp();
  Serial.println("AC ON "); Serial.print(ONdata); Serial.print(" SENT");
  smartthing.send("switch on");


}

void acOFF() {
  isOn = false;
  irsend.sendRaw(OFF, sizeof(OFF) / sizeof(int), khz);
  delay(1000);
  //  irsend.sendSAMSUNG(OFFdata, 32);
  Serial.println("AC OFF "); Serial.print(OFFdata); Serial.print(" SENT");
  lastMode = "off";
  
  smartthing.send("switch off");
  

  delay(100);
}

void _ToggleLED() {

  irsend.sendRaw(ToggleLED, sizeof(ToggleLED) / sizeof(int), khz);
  delay(100);
  // irsend.sendSAMSUNG(ToggleLEDData, 32);
  smartthing.send("LED toggled");
  //  Serial.println("TOGGLE LED "); Serial.print(ToggleLEDData); Serial.print(" SENT");

  delay(100);
}

void SetTemp(uint32_t * val, uint32_t valData) {
  irsend.sendRaw(val, 99, khz);
  delay(3000);
  // irsend.sendSAMSUNG(valData, 32);

  //  delay(1000);
  //  irsend.sendSAMSUNG(REPEAT, 32);

  // get value as int
  int IntTemp = IntTempArray[IndexArrays];
  // convert int Temp to String to send to st
  String StringTemp = String(IntTemp);
  smartthing.send("setpoint " + StringTemp);
  // to do//  smartthing.send("heatingSetpoint " + StringTemp);

  Serial.println("New Temperature "); Serial.print(IntTemp); Serial.print("F");

  delay(100);
}


void Refresh() {


  if (lastMode == "TURBO") {
    smartthing.send("thermostatMode turbo");
  }
  else if (lastMode == "FAN") {
    smartthing.send("thermostatMode fanOn");
  }
  else if (lastMode == "AUTO") {
    smartthing.send("thermostatMode auto");
  }
  else if (lastMode == "HEAT") {
    smartthing.send("thermostatMode heat");
  }
  else if (lastMode == "COOL") {
    smartthing.send("thermostatMode cool");
  }
    else if (lastMode == "off") {
    smartthing.send("thermostatMode off");
  }


  Blink(5, 50);

  delay(100);

}



/////////// fan speed commands

void fanLow() {
  irsend.sendRaw(FANLOW, sizeof(FANLOW) / sizeof(int), khz);
  delay(1000);
  // irsend.sendSAMSUNG(FANLOWdata, 32);
  smartthing.send("FANSPEED FANLOW");
  isOn = true;
  lastFanMode = "FANLOW";
}

void fanMed() {
  irsend.sendRaw(FANMED, sizeof(FANMED) / sizeof(int), khz);
  delay(1000);
  // irsend.sendSAMSUNG(FANMEDdata, 32);
  isOn = true;
  lastFanMode = "FANMED";
  smartthing.send("FANSPEED FANMED");
}

void fanHigh() {
  irsend.sendRaw(FANHIGH, sizeof(FANHIGH) / sizeof(int), khz);
  delay(1000);
  // irsend.sendSAMSUNG(FANHIGHdata, 32);
  isOn = true;
  lastFanMode = "FANHIGH";
  smartthing.send("FANSPEED FANHIGH");
}

void cool() {
  //acON();
  irsend.sendRaw(COOL, sizeof(COOL) / sizeof(int), khz);
  delay(1000);
  // irsend.sendSAMSUNG(coolData, 32);
  Serial.println("COOL CMD"); Serial.print(coolData); Serial.print(" SENT");
  isOn = true;

  lastMode = "COOL";
  smartthing.send("thermostatMode cool");
}

void heat() {
  //acON();
  irsend.sendRaw(HEAT, sizeof(HEAT) / sizeof(int), khz);
  delay(1000);
  // irsend.sendSAMSUNG(heatData, 32);

  isOn = true;
  lastMode = "HEAT";
  smartthing.send("thermostatMode heat");
}

void Auto() {
  //acON();
  // irsend.sendRaw(AUTO, sizeof(AUTO) / sizeof(int), khz);
  delay(1000);
  irsend.sendSAMSUNG(autoData, 32); /// SHOULD BE SENT AS HEX, not int... To be tried again later!

  isOn = true;
  lastMode = "AUTO";
  smartthing.send("thermostatMode auto");
}

////////////////// commands modes
void fanOnly() {
  //acON();
  irsend.sendRaw(FAN, sizeof(FAN) / sizeof(int), khz);
  delay(1000);
  // irsend.sendSAMSUNG(fanData, 32);
  smartthing.send("thermostatMode fan");
  isOn = true;
  lastMode = "FAN";
}
void turbo() {
  irsend.sendRaw(TURBO, sizeof(TURBO) / sizeof(int), khz);
  delay(1000);
  smartthing.send("thermostatMode TURBO");
  isOn = true;
  lastMode = "TURBO";
}




void setLast() {

  setToLastFanMode();
  SetToLastMode();
  setToLastTemp();

}

void setToLastTemp() {

  if (lastRecordedTemp > 0) {
    IndexArrays = lastRecordedTemp;
    Serial.print("set to last recorded TEMPERATURE mode: "); Serial.println(lastRecordedTemp);
  }
  //  else {
  //    IndexArrays = 10; // default at 72F
  //    Serial.println("NO LAST RECORDED TEMPERATURE SETTING. Set to default: 72");
  //  }

  SetTemp(newTemp, newTempData);
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

void SetToLastMode() {

  if (lastMode == "TURBO") {
    turbo();
    Serial.println("set to last recorded mode: TURBO");
    smartthing.send("Remember TURBO");
  }
  else if (lastMode == "FAN") {
    fanOnly();
    Serial.println("set to last recorded mode: FAN ONLY");
  }
  else if (lastMode == "AUTO") {
    Auto();
    Serial.println("set to last recorded mode: AUTO");
  }
  else if (lastMode == "HEAT") {
    heat();
    Serial.println("set to last recorded mode: HEAT");
  }
  else if (lastMode == "COOL") {
    cool();
    Serial.println("set to last recorded mode: COOL");
  }
  else {
    Serial.println("No recorded mode, setting to default: AUTO");
    Auto();
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

//*****************************************************************************



void Reset() {
  smartthing.send("controller reset");

  ESP.restart();
}


void messageCallout(String message)
{
  // if debug is enabled print out the received message
  if (isDebugEnabled)
  {
    Serial.print("Received message: '");
    Serial.print(message);
    Serial.println("' ");
  }


  if (message.startsWith("setTemp"))
  {
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

    int n = New.toInt();  //convert String into a number
    Serial.println("String converted to integer");

    // search for the integer value as an index

    i = 0;
    s = 99;
    Serial.println("Indexation..");
    while ( IntTempArray[i] != n && i < s) {
      i++;
      Serial.println("");
      Serial.print("found: "); Serial.print(IntTempArray[i]);
    }


    Serial.print("Indexation done.. found index value: "); Serial.println(i);

    IndexArrays = i;
    newTemp = TempArray[IndexArrays - 1][99]; // now use this index to find the proper IR command to send
    newTempData = DataTempArray[IndexArrays - 1];

    Serial.print("newTemp is: "); Serial.println((uint32_t) newTemp);

    SetTemp(newTemp, newTempData);

  }

  // if message contents equals to 'on' then call on() function
  // else if message contents equals to 'off' then call off() function

  if (message.equals("switch on"))
  {
    if (lastMode != "" && lastRecordedTemp != 0) {
      Serial.println("setting last modes values");
      setLast(); // set the last recorded values
    }
    else {
      Serial.println("turning on");
      acON();
    }
  }
  else if (message.equals("switch off"))
  {
    acOFF();
  }
  else if (message.equals("switch refresh"))
  {
    Refresh();
  }
  else if (message.equals("Led toggle")) {
    _ToggleLED();
  }
  else if (message.equals("raiseTemp")) {
    IndexArrays++;
    if (IndexArrays > sizeof(TempArray)) {
      IndexArrays = sizeof(TempArray);
    }
    lastRecordedTemp = IndexArrays; // record this value for next time the device starts

    newTemp = TempArray[IndexArrays - 1][99];
    newTempData = DataTempArray[IndexArrays - 1];
    SetTemp(newTemp, newTempData);
  }
  else if (message.equals("lowerTemp")) {
    IndexArrays--;
    if (IndexArrays < 0) {
      IndexArrays = 0;
    }
    lastRecordedTemp = IndexArrays; // record this value for next time the device starts
    newTemp = TempArray[IndexArrays - 1][99];
    newTempData = DataTempArray[IndexArrays - 1];

    SetTemp(newTemp, newTempData);
    Serial.println("");
    Serial.print("SENT TEMP: "); Serial.print(IntTempArray[IndexArrays]);
  }
  else if (message.equals("cool")) {
    cool();
  }
  else if (message.equals("heat")) {
    heat();
  }
  else if (message.equals("auto")) {
    acON();
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

  delay(100);
}

