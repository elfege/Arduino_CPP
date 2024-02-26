
void cool()
{

  if (millis() - lastTimeCmdSent > dealytBtwCmds || currentMode != "cool")
  {
    isOff = false;
    lastTimeCmdSent = millis();
    lastOperation = 1; // saveToRTC() called in serverAndHubs()

    fanCirculate = false;
    lastModeRequest = millis();
    currentMode = "cool";
    term.println("cool()");

    if (outside_temperature >= 60 || outside_temperature == 0)
    {
      /* VERY POWER HUNGRY FEATURE!!! */
      irsend.sendRaw(PURECOOL, sizeof(PURECOOL) / sizeof(int), khz); // PURECOOL cmd instead of boosting value, the later being an "auto" cmd, which sucks most of the time.
      delay(DELAY_CMD);
      irsend.sendRaw(PURECOOL, sizeof(PURECOOL) / sizeof(int), khz); // PURECOOL cmd instead of boosting value, the later being an "auto" cmd, which sucks most of the time.
      delay(DELAY_CMD);
      irsend.sendRaw(PURECOOL, sizeof(PURECOOL) / sizeof(int), khz); // PURECOOL cmd instead of boosting value, the later being an "auto" cmd, which sucks most of the time.
      delay(DELAY_CMD);
      turbo();

      // indexArray = getIndex(lastSetPoint);
      // term.println("getting temp value from array of temperatures");
      // newTemp = TempArray[indexArray - 1][99];
      // term.println("sending SetTemp() command");
      // SetTemp(newTemp, "coolingSetpoint"); // don't send setpoint event if using boost setpoint
    }
    else
    {
      // if outside temperature is below 60, don't trigger the compressor, just run the fan instead.
      term.println("Outside temperature is below 60°F, running fan instead of cooling");
      irsend.sendRaw(FAN, sizeof(FAN) / sizeof(int), khz);
      delay(DELAY_CMD);
    }

    send_to_hub("thermostatMode cool"); // send a thermostatMode event
    send_to_hub("thermostatOperatingState cooling");
    send_to_hub("coolingSetpoint " + String(lastSetPoint)); // send a coolingSetpoint event
    send_to_hub("fanspeed auto");
  }
  else
  {
    term.println("Command already sent less than 5 minutes ago. Doing nothing to prevent cancel effect on unit");
  }
}
void heat()
{

  if (millis() - lastTimeCmdSent > dealytBtwCmds || currentMode != "heat")
  {
    isOff = false;
    lastTimeCmdSent = millis();

    lastOperation = 2; // saveToRTC() called in serverAndHubs()

    fanCirculate = false;
    lastModeRequest = millis();
    currentMode = "heat";
    term.println("heat()");

    if (outside_temperature < 60 || outside_temperature == 0)
    {
      /* VERY POWER HUNGRY FEATURE!!! */
      irsend.sendRaw(PUREHEAT, sizeof(PUREHEAT) / sizeof(int), khz); // PUREHEAT cmd instead of boosting value, the later being an "auto" cmd, which sucks most of the time.
      delay(DELAY_CMD);
      irsend.sendRaw(PUREHEAT, sizeof(PUREHEAT) / sizeof(int), khz); // PUREHEAT cmd instead of boosting value, the later being an "auto" cmd, which sucks most of the time.
      delay(DELAY_CMD);
      irsend.sendRaw(PUREHEAT, sizeof(PUREHEAT) / sizeof(int), khz); // PUREHEAT cmd instead of boosting value, the later being an "auto" cmd, which sucks most of the time.
      delay(DELAY_CMD);
      turbo();

      // indexArray = getIndex(lastSetPoint);
      // term.println("getting temp value from array of temperatures");
      // newTemp = TempArray[indexArray - 1][99];
      // term.println("sending SetTemp() command");
      // SetTemp(newTemp, "heatingSetpoint"); // don't send setpoint event if using boost setpoint
    }
    else
    {
      // if outside temperature is above 60, don't trigger the heat pump, just run the fan instead.
      term.println("Outside temperature is above 60°F, running fan instead of cooling");
      fanCirculate = true;
      irsend.sendRaw(FAN, sizeof(FAN) / sizeof(int), khz);
      delay(DELAY_CMD);
    }

    /*requiredSetpointForBoost = "85"; // this will force the AC to boost its heating operation

      term.print("lastSetPoint is: "); term.println(lastSetPoint);
      term.print("requiredSetpointForBoost is: "); term.println(requiredSetpointForBoost);
      //  term.println("getting index value from array of indexes");
      indexArray = getIndex(requiredSetpointForBoost);
      //indexArray = getIndex(lastSetPoint);
      term.println("getting temp value from array of temperatures");
      newTemp = TempArray[indexArray - 1][99];
      term.println("sending SetTemp() command");
      SetTemp(newTemp, "");// don't send setpoint event if using boost setpoint
      delay(DELAY_CMD);;
    */

    // toggleLED();
    send_to_hub("thermostatMode heat");
    send_to_hub("thermostatOperatingState heating");
    send_to_hub("heatingSetpoing " + String(lastSetPoint)); // send a heatingSetpoint event
    send_to_hub("fanspeed auto");
  }
  else
  {
    term.println("Command already sent less than 5 minutes ago. Doing nothing to prevent cancel effect on unit");
  }
  // NB: turbo request is set by the SetTemp() function.
}

void turbo()
{
  if (!turboIsOn)
  {
    fanCirculate = false;
    turboIsOn = true;
    term.println("sending turbo IR RAW cmd");
    irsend.sendRaw(TURBO, sizeof(TURBO) / sizeof(int), khz);
    delay(DELAY_CMD);
    send_to_hub("TurboCmd Sent");
    send_to_hub("fanspeed turbo");
  }
  else
  {
    term.println("turbo cmd already sent");
  }
}

void Auto()
{
  lastOperation = 3; // saveToRTC() called in serverAndHubs()

  fanCirculate = false;
  currentMode = "auto";
  lastActiveMode = currentMode; // used for when we turn the fan back to auto
  term.println("auto()");
  irsend.sendRaw(AUTO, sizeof(AUTO) / sizeof(int), khz);
  delay(DELAY_CMD);
  toggleLED();
  // irsend.sendSAMSUNG(autoData, 32); /// SHOULD BE SENT AS HEX, not int... To be tried again later!

  onrequested = true;
  requestTurbo = true; // reset turbo() cmd
  turboIsOn = false;
  requestTurboMillis = millis(); // so it is sent again wit

  send_to_hub("thermostatMode auto");
  send_to_hub("thermostatFanMode auto");
  send_to_hub("fanspeed auto");
}

void acON()
{
  fanCirculate = false;
  isOff = false;
  term.println("acON()");
  irsend.sendRaw(ON, sizeof(ON) / sizeof(int), khz);
  delay(DELAY_CMD);
  irsend.sendRaw(ON, sizeof(ON) / sizeof(int), khz);
  delay(DELAY_CMD);

  // toggleLED();

  onrequested = true;
  requestTurbo = true;           // reset turbo() cmd
  requestTurboMillis = millis(); // so it is sent again within seconds

  currentMode = "ON";

  send_to_hub("switch on");
  send_to_hub("thermostatFanMode auto");
}

void acOff()
{
  if (lastActiveMode != "off" || millis() - lastTimeCmdSent > dealytBtwCmds)
  {
    lastOperation = 0; // saveToRTC() called in serverAndHubs()
    
    lastTimeCmdSent = millis();

    currentMode = "off";
    send_to_hub("switch off");
    send_to_hub("thermostatMode off");
    turboIsOn = false;
    onrequested = false;
    ledToggled = false;
    isOff = true;

    if (millis() - offRequestMillis > fanDurationWhenOff)
    {
      
      lastActiveMode = "off";

      irsend.sendRaw(OFF, sizeof(OFF) / sizeof(int), khz); // works on C&H...
      delay(DELAY_CMD);
      irsend.sendRaw(OFF, sizeof(OFF) / sizeof(int), khz); // works on C&H...
      delay(DELAY_CMD);
      irsend.sendRaw(OFF, sizeof(OFF) / sizeof(int), khz); // works on C&H...
      delay(DELAY_CMD);
      irsend.sendRaw(OFF, sizeof(OFF) / sizeof(int), khz); // works on C&H...
      delay(DELAY_CMD);
    }
    else
    {
      irsend.sendRaw(FAN, sizeof(FAN) / sizeof(int), khz);
      delay(DELAY_CMD);
    }
  }
  else
  {
    term.println("OFF COMMAND ALREADY SENT - SKIPPING");
  }

  delay(100);
}

void off_override()
{
  lastOperation = 0; // saveToRTC() called in serverAndHubs()
  lastActiveMode = "off";

  currentMode = "off";
  send_to_hub("switch off");
  send_to_hub("thermostatMode off");
  turboIsOn = false;
  onrequested = false;
  ledToggled = false;
  isOff = true;
  irsend.sendRaw(OFF, sizeof(OFF) / sizeof(int), khz); // works on C&H...
  delay(DELAY_CMD);
  irsend.sendRaw(OFF, sizeof(OFF) / sizeof(int), khz); // works on C&H...
  delay(DELAY_CMD);
}

void toggleLED()
{
  /*term.println("executing led toggle");
    if (!ledToggled) // this condition will be true if and only if the last command (cool, heat, auto...) was preceded by an off() command
    //the LED is always back on by default when the unit starts again... which is totally stupid btw...
    {
    term.println("sending IR data");
    irsend.sendRaw(TOGGLELED, sizeof(TOGGLELED) / sizeof(int), khz);
    delay(DELAY_CMD);
    ledToggled = true;
    //send_to_hub("LED TOGGLED");
    }
    else
    {
    term.println("LED ALREADY TOGGLED");
    }*/
  term.println("LED TOGGLE IGNORED ON AC THIS DEVICE");
}
void toggleLEDbypass()
{
  term.println("simple led toggle, no condition");
  irsend.sendRaw(TOGGLELED, sizeof(TOGGLELED) / sizeof(int), khz); // there's no off/on, simple toggle
  delay(DELAY_CMD);
}

void SetTemp(uint32_t *val, String cmd)
{

  term.println("SENDING RAW IR DATA");
  irsend.sendRaw(val, 199, khz);
  delay(2000);

  // every new setpoint cmd cancels the turbo() operation due to the fact that we use raw cmd data

  lastModeRequest = millis();
  onrequested = true; // every setpoint request turns on the AC

  // DEPRECATED DOESN'T DO GOO DURING MID-SEASON AUTO WILL HEAT UP EVEN WHEN SET POINT IS 71
  // UPDATE: Works if used with boost setpoints
  requestTurbo = true; // reset turbo() cmd
  // turboIsOn = false; // reset only at acOff() or fanOn() (since fanOn() can be used instead of off by my hub's app)
  requestTurboMillis = millis(); // so the turbo() method is called within seconds

  // the device driver will need to know every setpoint in order for the app to correct if, for example, IR signal didn't go through
  send_to_hub(cmd + " " + lastSetPoint); // lastSetPoint is a global variable / it's the set point requested through ajax or Hub and recorded then
}

void Refresh()
{

  term.println("REFRESHING");
  term.println("This device has rebooted " + (String)reset_counter + " times");
  term.println("This device last recorded operation is " + (String)lastOperation);
  term.println("This device last set point is " + (String)lastSetPoint);
  term.println("fanCirculate => " + String(fanCirculate ? "TRUE" : "FALSE"));

  // lastTemperatureCheck = temperatureCheckTimerDelay;
  // getOutdoorTemperature();

  if (currentMode != "off")
  {
    String currMode = currentMode;
    currMode.toLowerCase(); // .toLowerCase() modifies the String in place, so we need apply it to a copy
    send_to_hub("thermostatMode " + currMode);
  }
  else
  {
    term.println("currentMode = off");
    turboIsOn = false;
    send_to_hub("thermostatMode " + String(fanCirculate ? "fancirculate" : "off"));
  }

  send_to_hub("thermostatSetpoint " + lastSetPoint);
  delay(200);
  String lastOpState = lastOperation == 0 ? "idle" : lastOperation == 1 ? "cooling"
                                                 : lastOperation == 2   ? "heating"
                                                 : lastOperation == 3   ? "auto"
                                                 : lastOperation == 4   ? "idle"
                                                 : lastOperation == 5   ? "fanCirculate"
                                                                        : "error";

  send_to_hub("thermostatOperatingState " + String(fanCirculate ? "fancirculate" : lastOpState));
  delay(200);

  fanMode = fanCirculate; // fanMode is an RTC commuted into boolean
  if (fanMode)
    fanOn(); // resend fan command in case, doesn't cost ressources.

  term.println("thermostatFanMode = " + String(fanMode ? "on" : "auto"));

  send_to_hub("thermostatFanMode " + String(fanMode ? "on" : "auto"));
  delay(200);
  String turboState = turboIsOn ? "turbo on" : "turbo off";
  delay(200);
  send_to_hub(turboState);
}

void refreshSetpoint()
{
  String space = " ";
  String thisCmd;

  if (currentMode == "heat")
  {
    thisCmd = "heatingSetpoint";
    send_to_hub(thisCmd + space + StringTemp);
  }
  else if (currentMode == "cool")
  {
    thisCmd = "collingSetpoint";
    send_to_hub(thisCmd + space + StringTemp);
  }
}

/////////// fan speed commands

void fanLow()
{
  term.println("Sending LOWFAN IR CMD");
  lastFanMode = "FANLOW";
  onrequested = true;

  irsend.sendRaw(FAN, sizeof(LOWFAN) / sizeof(int), khz);
  delay(DELAY_CMD);

  //  send_to_hub("fanspeed LOW");
}

void fanMed()
{
  term.println("Sending MEDFAN IR CMD");
  lastFanMode = "FANMED";
  onrequested = true;

  irsend.sendRaw(FAN, sizeof(MEDFAN) / sizeof(int), khz);
  delay(DELAY_CMD);

  //  send_to_hub("fanSpeed medium");
}

void fanHigh()
{
  term.println("Sending HIGHFAN IR CMD");
  lastFanMode = "FANHIGH";
  onrequested = true;

  irsend.sendRaw(FAN, sizeof(HIGHFAN) / sizeof(int), khz);
  delay(1000);

  //  send_to_hub("fanspeed high");
}

//////////////////  modes

void fanOn()
{
  fanCirculate = true;

  term.println("Turning on the fan");

  irsend.sendRaw(FAN, sizeof(FAN) / sizeof(int), khz);
  delay(DELAY_CMD);
  irsend.sendRaw(FAN, sizeof(FAN) / sizeof(int), khz);
  delay(DELAY_CMD);
  irsend.sendRaw(FAN, sizeof(FAN) / sizeof(int), khz);
  delay(DELAY_CMD);

  send_to_hub("thermostatMode " + String(currentMode == "off" ? "fancirculate" : currentMode));

  //  send_to_hub("thermostatOperatingState " + String(fanCirculate ? "fancirculate" : String(lastOperation))); // fanCirculate mode not implemented yet
  send_to_hub("thermostatFanMode on");
  delay(1000);
  String var = String("FAN is ON");
  send_to_hub(var); // must resend this so web ui's button won't show "thermostatFanMode on/auto" text or other weird states meant for smart hub
}

void fanAuto()
{

  if (currentMode == "off" && millis() - offRequestMillis < fanDurationWhenOff)
  {
    term.println("received command to turn off the fan but fan off mode is still active, doing nothing...");
    return;
  }
  fanCirculate = false;

  if (lastOperation == 1)
  {
    irsend.sendRaw(FANAUTOC, sizeof(FANAUTOC) / sizeof(int), khz);
    delay(DELAY_CMD);
  }
  else if (lastOperation == 2)
  {
    irsend.sendRaw(FANAUTOH, sizeof(FANAUTOH) / sizeof(int), khz);
    delay(DELAY_CMD);
  }
  else
  {
    irsend.sendRaw(FANAUTOH, sizeof(FANAUTOH) / sizeof(int), khz);
    delay(DELAY_CMD);
  }

  String currMode = currentMode;
  currMode.toLowerCase(); // .toLowerCase() modifies the String in place, so we need apply it to a copy
  send_to_hub("thermostatMode " + currMode);

  //  send_to_hub("thermostatOperatingState " + String(fanCirculate ? "fancirculate" : String(lastOperation))); fanCirculate mode not yet implemented
  send_to_hub("thermostatFanMode auto");
  delay(1000);
  String var = String("FAN is AUTO");
  send_to_hub(var);
}

void setLast()
{

  // setToLastFanMode();
  SetTocurrentMode();
  setToLastTemp();
}

void setToLastTemp()
{

  if (lastRecordedTemp > 0)
  {
    indexArray = lastRecordedTemp;
    term.print("set to last recorded TEMPERATURE mode: ");
    term.println(lastRecordedTemp);

    newTemp = TempArray[indexArray - 1][99]; // now use this index to find the proper IR command to send

    SetTemp(newTemp, cmd);
  }
  else
  {
    send_to_hub("NO LASTTEMP");
  }
}

void setToLastFanMode()
{
  if (lastFanMode == "FANLOW")
  {
    fanLow();
    term.println("set to last recorded FAN mode: LOW");
  }
  if (lastFanMode == "FANMED")
  {
    fanMed();
    term.println("set to last recorded FAN mode: MEDIUM");
  }
  if (lastFanMode == "FANHIGH")
  {
    fanHigh();
    term.println("set to last recorded FAN mode: HIGH");
  }
}

void SetTocurrentMode()
{

  if (currentMode == "auto")
  {
    Auto();
    term.println("set to last recorded mode: AUTO");
    send_to_hub("thermostatMode auto");
  }
  else if (currentMode == "heat")
  {
    heat();
    term.println("set to last recorded mode: HEAT");
    send_to_hub("thermostatMode heat");
  }
  else if (currentMode == "cool")
  {
    cool();
    term.println("set to last recorded mode: COOL");
    send_to_hub("thermostatMode cool");
  }
  else
  {
    term.println("No recorded mode, setting to default: heat");
    //    Auto();
    send_to_hub("thermostatMode heat");
  }
}

void send_to_hub(String var)
{
  _server.send(200, "text/html", var);
  term.println(var + " at " + String(TimeInfos()));
  smartthing.send(var); // send the current value to smartthings
}

void getMac()
{
  String ip = WiFi.localIP().toString();
  String var = "Mac: " + WiFi.macAddress() + " IP:" + ip + " SSID:" + String(WiFi.SSID());
  _server.send(200, "text/html", var);
}

void getOutdoorTemperature()
{
  if (millis() - lastTemperatureCheck >= temperatureCheckTimerDelay)
  {
    term.println("********************************************");
    term.println("          getOutdoorTemperature             ");
    term.println("********************************************");
    term.println(API_PATH);

    if (WiFi.status() == WL_CONNECTED)
    {
      WiFiClient client;
      HTTPClient http;

      String serverPath = API_PATH;

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
      // term.println("serverPath.c_str() => " + String(serverPath.c_str()));

      // Send HTTP GET request
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0)
      {
        term.print("HTTP Response code: ");
        // term.println(httpResponseCode);
        String payload = http.getString();
        // term.println(payload);

        // Print the entire HTTP response, including headers
        // http.writeToStream(&Serial);

        if (payload.length() > 0)
        {

          // Use the Streaming API to extract the temperature value from the JSON payload
          DynamicJsonDocument jsonBuffer(4000);
          DeserializationError error = deserializeJson(jsonBuffer, payload);
          if (!error)
          {
            JsonObject jsonObject = jsonBuffer.as<JsonObject>();
            JsonArray attributesArray = jsonObject["attributes"];
            for (JsonVariant attribute : attributesArray)
            {
              JsonObject attributeObject = attribute.as<JsonObject>();
              if (attributeObject["name"] == "temperature")
              {
                outside_temperature = attributeObject["currentValue"];
                term.println("outside temperature is: " + String(outside_temperature) + "°F");
                break; // Stop looping once we find the temperature value
              }
            }
          }
          else
          {
            term.println("---------------- ERROR ---------------" + String(error.c_str()));
            // print the available heap memory in bytes.
            term.println("Free heap memory: " + String(ESP.getFreeHeap()));
          }
        }
        else
        {
          term.println("---------------- ERROR ---------------Payload is empty");
        }
      }
      else
      {
        term.print("Error code: ");
        term.println(httpResponseCode);
      }

      // Free resources
      http.end();
    }
    else
    {
      term.println("WiFi Disconnected");
    }

    lastTemperatureCheck = millis();
  }
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

  // term.println("LOOP time = " + String(elapsed) + "ms");
  String result = "Up time: ";
  timerResult = "";

  if (dispalyDays >= 1)
  {
    result = result + dispalyDays + " d";
    timerResult = dispalyDays + " d";
    // term.print(result);
  }
  if (displayHours >= 1)
  {
    result = result + displayHours + " h ";
    timerResult = displayHours + " h";
    // term.print(result);
  }
  if (displayMinutes >= 1)
  {
    result = result + displayMinutes + " m ";
    timerResult = displayMinutes + " m";
    // term.print(result);
  }
  if (displaySeconds >= 1)
  {
    result = result + displaySeconds + " s ";
    timerResult = displaySeconds + " s";
    // term.print(result);
  }

  result += " " + lastBootDate; // add the boot date stamp
  return result;
}

//*****************************************************************************
// Local Functions  | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
//                  V V V V V V V V V V V V V V V V V V V V V V V V V V V V V V
//*****************************************************************************
void Blink(int times, int lapse)
{
  int c = 0;
  while (c < times)
  {
    digitalWrite(LED, 1);
    delay(lapse);
    digitalWrite(LED, 0);
    delay(lapse);
    c++;
  }
}

//*****************************************************************************

void Reset()
{

  send_to_hub("reset reset");
  unsigned long stt = millis();
  while (millis() - stt < 1000) // give it time to send the debug message
  {
    serverAndHub();
  }
  ESP.reset();
}

void messageCallout(String message)
{
  term.print("Received message: '");
  term.print(message);
  term.println("' ");

  if (message.startsWith("setHeatingSetpoint") || message.startsWith("setCoolSetpoint") || message.startsWith("level"))
  {
    if (message.startsWith("setHeatingSetpoint"))
    {
      cmd = "heatingSetpoint";
    }
    else if (message.startsWith("setCoolSetpoint"))
    {
      cmd = "coolingSetpoint";
    }
    term.println("");
    term.print("received SetPoint command: ");
    term.println(message);

    String thisMessage = message;
    char this_char[thisMessage.length() + 1];
    thisMessage.toCharArray(this_char, sizeof(this_char));

    String New = "";

    int i = 0;
    int s = message.length() + 1;
    term.println("extracting digits");
    for (s > 0; i < s; i++)
    {
      if (isDigit(thisMessage[i]))
      {
        New += thisMessage[i];
        // term.print("New = "); term.println(New);
      }
    }
    term.print("New SetPoint is: ");
    term.println(New);

    lastSetPoint = New; // remember this as last requested set point for future reference
    lastSetPointRTC = lastSetPoint.toInt();
    lastSetpointRequested = lastSetPoint;

    indexArray = getIndex(New);
    // term.print("Indexation done.. found index value: "); term.println(indexArray);
    newTemp = TempArray[indexArray - 1][99]; // now use this index to find the proper IR command to send

    term.print("newTemp is: ");
    term.println((uint32_t)newTemp);

    SetTemp(newTemp, cmd);
    String var = cmd + " " + String(New); // "New" is a String while newTemp is a charArray
    send_to_hub(var);                     // parse event
  }
  else if (message.equals("on"))
  {
    /*  ////send_to_hub("---2");
      if (currentMode != "" && lastRecordedTemp != 0) {
        term.println("setting last modes values");
        setLast(); // set the last recorded values
      }
      else {*/
    term.println("on");
    acON();
    //}
  }
  else if (message.equals("off"))
  {
    acOff();
    if (currentMode != "OFF")
    {
      offRequestMillis = millis(); // allow fan to circulate for X minutes after turning off to prevent mold formation
    }
  }
  else if (message.equals("refresh"))
  {
    Refresh();
  }
  else if (message.equals("Ledtoggle"))
  {

    toggleLED();
  }
  else if (message.equals("raiseHeatingSetpoint"))
  {
    ////send_to_hub("---3");
    indexArray++;
    if (indexArray > sizeof(TempArray))
    {
      indexArray = sizeof(TempArray);
    }
    newTemp = TempArray[indexArray - 1][99];
    SetTemp(newTemp, "heatingSetpoint");
  }
  else if (message.equals("lowerHeatingSetpoint"))
  {
    ////send_to_hub("---4");
    indexArray--;
    if (indexArray < 0)
    {
      indexArray = 0;
    }

    newTemp = TempArray[indexArray - 1][99];

    SetTemp(newTemp, "heatingSetpoint");
  }
  else if (message.equals("raiseCoolSetpoint") || message.equals("raiseSetpoint"))
  {
    // send_to_hub("---5");
    indexArray++;
    if (indexArray > sizeof(TempArray))
    {
      indexArray = sizeof(TempArray);
    }

    newTemp = TempArray[indexArray - 1][99];

    SetTemp(newTemp, "coolingSetpoint");
  }
  else if (message.equals("lowerCoolSetpoint") || message.equals("lowerSetpoint"))
  {
    // send_to_hub("---6");
    indexArray--;
    send_to_hub("indexing cmd");
    if (indexArray < 0)
    {
      indexArray = 0;
    }

    newTemp = TempArray[indexArray - 1][99];

    SetTemp(newTemp, "coolingSetpoint");
  }
  else if (message.equals("cool"))
  {
    cool();
  }
  else if (message.equals("heat"))
  {
    heat();
  }
  else if (message.equals("auto"))
  {
    // acON();
    Auto();
  }
  else if (message.equals("fanOn"))
  {
    fanOn();
  }
  else if (message.equals("fanAuto"))
  { // turn off fan only operation
    fanAuto();
  }
  else if (message.equals("fanlow"))
  {
    fanLow();
  }
  else if (message.equals("fanmed"))
  {
    fanMed();
  }
  else if (message.equals("fanhigh"))
  {
    fanHigh();
  }
  else if (message.equals("turbo"))
  {
    turbo();
  }
  else if (message.equals("reset"))
  {
    Reset();
  }

  else if (message.equals("refresh"))
  {
    Refresh();
  }

  delay(100);
}

int getIndex(String val)
{
  int n = val.toInt(); // convert String into a number
  // term.println("String converted to integer");
  //  search for the integer value as an index

  int i = 0;
  int s = 99;
  term.println("Indexation..");
  while (IntTempArray[i] != n && i < s)
  {
    i++;
    // term.println("");
    // term.print("found: "); term.println(IntTempArray[i]);
  }
  return i;
}

boolean isOn()
{
  bool result = false;
  if (analogRead(Asensor) > 800 || onrequested)
  {
    result = true;
  }
  return result;
}

void glow()
{
  int s = 256; // ESP8266 uses 10-bit resolution for PWM generation / 1024 is max. 256 allows for faster glow

  if (glw >= s)
  {
    goUp = false;
  }
  else if (glw < 1)
  {
    goUp = true;
  }

  if (goUp && glw < s)
  {
    glw++;
    analogWrite(LED, glw);
    // term.print("GLOW "); term.println(i);
  }
  else if (!goUp && glw > 0)
  {
    glw--;
    analogWrite(LED, glw);
    // term.print("GLOW "); term.println(i);
  }
  //  delay(5); // delay must be set in the scope where it's called from
}

void getTemp() // get the current target temperature to display on webpage
{
  int IntTemp = IntTempArray[indexArray];
  String var = String(IntTemp);
  var = "level " + String(IntTemp);
  send_to_hub(var);
  ;
}
void updateNtpTime()
{

  timeClient.update();
  unsigned long Start = millis();
  while (!timeClient.updated() && millis() - Start < 2000)
  {
    delay(1);
    serverAndHub();
  }
  if (!timeClient.updated())
  {
    if (ntpFailures < 2)
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
      term.println("******TIME SUCCESSFULY UPDATED FROM NTP SERVER AFTER " + String(ntpFailures) + " FAILED ATTEMPTS******");
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
