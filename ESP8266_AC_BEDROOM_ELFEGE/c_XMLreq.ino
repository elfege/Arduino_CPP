
void initXMLhttp()
{
  _server.on("/", []()
  {
    term.println("loading page...");
    _server.send_P(200, "text/html", MAIN_page);
  });
  _server.on("/index2", []()
  {
    _server.send_P(200, "text/html", simplifiedPage);
  });
  _server.on("/getstate", []()
  {
    getStates();
  });
  _server.on("/getClock", []()
  {
    String clockInfo = timeClient.getFormattedTime();
    _server.send(200, "text/html", clockInfo);
  });

  _server.on("/cool-w-fan", []()
  {
    term.println("fanInsteadOfCoolRTC ==> b4 ==>" + String(fanInsteadOfCoolRTC));
    term.println("fanInsteadOfCool ==> b4 ==>" + String(fanInsteadOfCool));
    fanInsteadOfCool = !fanInsteadOfCool;
    fanInsteadOfCoolRTC = fanInsteadOfCool;
    term.println("fanInsteadOfCoolRTC ==> after ==>" + String(fanInsteadOfCoolRTC));
    term.println("fanInsteadOfCool ==> after ==>" + String(fanInsteadOfCoolRTC));

    String str = "COOL-w-FAN " + String(fanInsteadOfCool ? "enabled" : "disabled");
    _server.send(200, "text/html", str);
  });
  

  _server.on("/cool", cool);
  _server.on("/heat", heat);

  _server.on("/fan", []()
  {
    fanOn();
    term.println("fanCirculate returns " + String(fanCirculate ? "TRUE" : "FALSE"));
    _server.send(200, "text/html", String(fanCirculate ? "FAN is ON" : "FAN is AUTO"));
  });

  _server.on("/auto", Auto);
  _server.on("/off", []() {
    if (millis() - offRequestMillis > fanDurationWhenOff)
    {
      if(currentMode != "OFF"){
    offRequestMillis = millis(); // allow fan to circulate for X minutes after turning off to prevent mold formation
    }
      acOff;
    }
    else {
      term.println("OFF REQUEST IGNORED. FAN ONLY INSTEAD");
      irsend.sendRaw(FAN, sizeof(FAN) / sizeof(int), khz);
      delay(DELAY_CMD);
    }
  });
  _server.on("/off_ovrd", off_override);
  _server.on("/ledtoggle", toggleLED);
  _server.on("/ledtogglebypass", toggleLEDbypass);


  _server.on("/logs", []()
  {
    logs = !logs;
  });
  _server.on("/stopAll", []()
  {
    _server.send_P(200, "text/html", WifiDisconnectedPage);
    StopAll = true; // irreversible needs manual reset or succesfull OTA upload
    stopAllRequest = millis(); // so make it a temporary 1 minute request
    tempRequest = true;
  });

  _server.on("/refresh", Refresh);
  _server.on("/getMac", getMac);
  
  _server.on("/getOutsideTemp", [](){
    String temp = String(outside_temperature);
    _server.send(200, "text/html", temp);
  });

  _server.on("/getTimeInfo", []()
  {
    String data = TimeInfos();
    _server.send(200, "text/html", data);

  });

  _server.on("/temp", getTemp);


  _server.on("/turbo", []()
  {
    turboIsOn = false;
    turbo();

  });

  _server.on("/reset", Reset);

  // temp range from slider
  int i = 0;
  int s = 85; // max sp value
  for (1; i <= s; i++) {
    //  term.println(i);
    String iString = String(i);

    String str1 = "/tempRange";
    String str2 = str1 + "=" + iString;
    _server.on(str2, [i, iString]() {
      term.print("NEW TEMPERATURE: "); term.println(i);
      // update the global variables
      lastSetPoint = iString;
      lastSetPointRTC = lastSetPoint.toInt();
      lastSetpointRequested = lastSetPoint;
      indexArray = getIndex(iString);
      term.println("indexArray = " + String(indexArray));
      newTemp = TempArray[indexArray - 1][99]; // now use this index to find the proper IR command to send
      term.println("newTemp = " + iString);
      SetTemp(newTemp, cmd);
      send_to_hub("thermostatSetpoint " + iString);
    });
  }

}


void getStates() {
  String sep = ",";

  String a = "state " + currentMode;
  String b = "FAN " + String(fanCirculate ? "fan: on" : "fan: auto");
  String c = "temp " + String(lastSetPoint);
  String d = "tempRange " + String(lastSetPoint);

  String data = a + sep + b + sep + c + sep + d;
  _server.send(200, "text/html", data);

}
