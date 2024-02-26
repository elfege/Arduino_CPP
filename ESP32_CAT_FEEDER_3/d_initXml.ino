void initXMLhttp()
{

  _server.on("/", []()
  {
    term.println("NEW CLIENT");
    _server.send_P(200, "text/html", MAIN_page);
  });

  _server.on("/getstates", []()
  {
    // term.println("getstates");
    getStates(false);
  });

 _server.on("/pause", []()
  {
    paused = !paused;
    _server.send(200, "text/plain", String(paused ? "RESUME" : "PAUSE"));  
  });
   _server.on("/off", []()
  {
    Reset();
    _server.send(200, "text/plain", "RESET!");  // we don't want to change the buttons' text
  });
   _server.on("/on", []()
  {
    feed();
    _server.send(200, "text/plain", "FEED");  // we don't want to change the buttons' text
  });
  _server.on("/feed", []()
  {
    feed();
    _server.send(200, "text/plain", "FEED");  // we don't want to change the buttons' text
  });
  _server.on("/load", []()
  {
    load();
    _server.send(200, "text/plain", "LOAD A CAN");  // we don't want to change the buttons' text
  });
  _server.on("/push", []()
  {
    push();
    _server.send(200, "text/plain", "PUSHING");  // we don't want to change the buttons' text
  });
  _server.on("/pull", []()
  {
    mainArmPull();
    _server.send(200, "text/plain", "PULLING");  // we don't want to change the buttons' text
  });
  _server.on("/smallarm", []()
  {
    if (smallisout)
    {
      smallPush();
    }
    else
    {
      smallPull();
    }
    _server.send(200, "text/plain", String(smallisout ? "SMALL ARM IS OUT" : "SMALL ARM IS IN"));
  });
  _server.on("/smallpush", []()
  {
    smallPush();
  });
  _server.on("/smallpull", []()
  {
    smallPull();
  });
  _server.on("/stop", []()
  {
    stop();
    _server.send(200, "text/plain", "STOP");
  });
  _server.on("/logs", []()
  {
    Logs = !Logs;
    String response = Logs ? "DEBUG ENABLED" : "DEBUG DISABLED";
    _server.send(200, "text/plain", response);
  });
  _server.on("/open", []()
  {
    _server.send(200, "text/plain", "OPEN A CAN");
    openCan();
  });
   _server.on("/openfree", []()
  {
    _server.send(200, "text/plain", "OPEN A CAN");
    open_free();
  });

   _server.on("/getmac", []()
  {
    String str = WiFi.macAddress();
    _server.send(200, "text/plain", str);
  });

  _server.on("/lock", []()
  {
    if (!canLocked()) {
      lock();
      _server.send(200, "text/plain", "LOCKING");
    } else {
      unlock();
      _server.send(200, "text/plain", "UNLOCKING");
    }
  });

  _server.on("/OTA", [](){
    _server.send(200, "text/plain", "OTA WAIT");
    readyOTA = true;
    readyOTAmillis = millis();
  });

  _server.on("/awaitOTA", [](){
    // http response to be sent from mainHandler() after 60 seconds.  
  });
  
  _server.on("/force_unlock", []()
  {
    unlock(); 
    delay(300);

      _server.send(200, "text/plain", "UNLOCKED");

  });
  _server.on("/force_lock", []()
  {
    lock(); 
    delay(300);

      _server.send(200, "text/plain", "UNLOCKED");

  });

  _server.on("/verticalup", []()
  {
    verticalPull();
    _server.send(200, "text/plain", "PULL UP");
  });
  _server.on("/verticaldown", []()
  {
    verticalPush();
    _server.send(200, "text/plain", "PUSH DOWN");
  });
  _server.on("/verticalstepup", []()
  {
    verticalUpStep(1, 10);
    _server.send(200, "text/plain", "STEP UP");
  });
  _server.on("/verticalstepdown", []()
  {
    verticalDownStep(1, 10);
    _server.send(200, "text/plain", "STEP DOWN");
  });
  _server.on("/extract", []()
  {
    extract();
  });
  _server.on("/reset", []()
  {
    Reset();
  });
  _server.on("/clean", []()
  {
    cleanOpener();
  });

  _server.on("/freecanlid", []() 
  {
    freeCanLid();
  });

  _server.on("/getip", []()
  {
    String localIp = WiFi.localIP().toString();
    if (Logs) term.println("_server.send() => " + localIp);
    _server.send(200, "text/html", localIp);
  });

  _server.on("/cosinuspush", []()
  {
    cosinusPush();
    _server.send(200, "text/plain", "ANGLE PUSH");
  });
  _server.on("/cosinuspull", []()
  {
    cosinusPull();
    _server.send(200, "text/plain", "ANGLE PULL");
  });

  _server.on("/cosinusplus", []()
  {
    cosinusPushStep(1, 10);
  });
  _server.on("/cosinusminus", []()
  {
    cosinusPullStep(1, 10);
  });

  _server.on("/recordcosmax", []()
  {
    recording = !recording;
    _server.send(200, "text/html", String(recording ? "STOP" : "RECORD"));

    if (recording) {
      cosinusPull();  // pull all the way
      unsigned long Start = millis();
      while (!stopped && Start - millis() < 4000)  // needed time to execute
      {
        yield();
      }
      // then push and record the start time (within push method)
      cosinusPush();
    } else {
      cosMax = millis() - cosinusMillis;  // record the elapsed time
      cosinusStop();
      term.println("NEW COSINUS MAX OPERATION TIME: " + String(cosMax) + " ms");
    }
  });

  _server.on("/toggleIntervals", []()
  {
    scriptactive = !scriptactive;
    _server.send(200, "text/html", "script " + String(scriptactive ? "STOP SCRIPT" : "RESUME SCRIPT"));
  });

  int s = 100; // max speed value sliderCosinus
  for (int i = 0; i <= s; i++)
  {
    //  term.println(i);
    String iString = String(i);
    String str1 = "/sliderCosinus";
    String str2 = str1 + "=" + iString;

    _server.on(str2, [i, iString]()
    {

      speedValCosinus = map(i, 0, 100, 0, 255);
      term.print("NEW SPEED: ");
      term.println(i);
      term.print("NEW SPEED (bits): ");
      term.println(speedValCosinus);

      ledcWrite(pwmChannelCos, speedValCosinus);
      if (Logs) term.println("COSINUS SPEED set to " + String(speedValCosinus) + "%");

      _server.send(200, "text/html", iString);
    });
  }

  // speed value sliderOpener
  for (int i = 0; i <= s; i++)
  {
    //  term.println(i);
    String iString = String(i);
    String str1 = "/sliderOpener";
    String str2 = str1 + "=" + iString;

    _server.on(str2, [i, iString]()
    {

      speedValOpener = map(i, 0, 100, 0, maxSpeedOpener);
      term.print("NEW SPEED: ");
      term.println(i);
      term.print("NEW SPEED (bits): ");
      term.println(speedValOpener);

      ledcWrite(pwmChannelCanOpener, speedValOpener);
      if (Logs) term.println("OPENER SPEED set to " + String(speedValOpener) + "%");

      _server.send(200, "text/html", iString);
    });
  }
}

void getStates(bool smartthingReq)
{
  String canposition = canDown() ? "CAN READY" : canInPosition() ? "CAN IN POSITION"
                       : "NO CAN";
  String deformation = deformed() ? "Defomation" : "No deformation";
  // String canLeft = oneCanLeft() ? "ONLY ONE CAN LEFT" : empty() ? "FEEDER IS EMPTY!"
  //                  : full()    ? "FULL"
  //                  : "ERROR";
  String canLeft = empty() ? "EMPTY" : "";
  String feedStatus = feeding ? "FEEDING..." : "FEED";
  String pullingStatus = pulling ? "PULLING" : "PULL";
  String pushingStatus = pushing ? "PUSHING" : "PUSH";
  String extractingStatus = extracting ? "EXTRACTING" : "EXTRACT";
  String lockStatus = canLocked() ? "UNLOCK" : "LOCK";

  String toSend = "";
  String a = "fillstate " + canLeft;
  String b = "canstate " + canposition;
  String c = "push " + pushingStatus;
  String d = "pull " + pullingStatus;
  String e = "extract " + extractingStatus;
  String f = "lock " + lockStatus;
  String g = "cosinus " + String(cosinuspushing ? "ANGLE PULL" : "ANGLE PUSH");
  String h = "feed " + feedStatus;
  String i = "deformation " + deformation;
  String k = "irval1 IR_1=N/A"; //String(analogRead(IR1_pile));
  String l = "irval2 IR_2=" + String(analogRead(IR2_pile));
  String m = "load " + String(loading ? "LOADING" : "LOAD A CAN");
  String n = "slider " + String(map(speedValOpener, 0, 255, 0, 100));
  String o = "feed " + String(feeding ? "FEEDING" : "FEED");
  String p = "smallarm" + String(smallisout ? "SMALL ARM IS OUT" : "SMALL ARM IS IN");
  String q = "verticalstate " + String(goingdown ? "CAN OPENER IS DOWN" : "CAN OPENER IS UP");
  String r = "sliderValueOpener " + String(map(speedValOpener, 0, 255, 0, 100));
  String s = "sliderOpener " + String(map(speedValOpener, 0, 255, 0, 100));
  String t = "sliderValueCosinus " + String(map(speedValCosinus, 0, 255, 0, 100));
  String u = "sliderCosinus " + String(map(speedValCosinus, 0, 255, 0, 100));

  String sep = ",";

  toSend = a + sep + b + sep + c + sep + d + sep + e + sep + f + sep + g + sep + h + sep + i + sep + k + sep + l + sep + m + sep + n + sep + o + sep + p + sep + q + sep + r + sep + s + sep + t + sep + u;
  _server.send(200, "text/html", toSend);

  // we can't send so many events to hubitat's hub... because then it's not happy... at all. So we send those essential ones, for now.
  // if it's been more than an hour, refresh all values
  // if device is currently feeding, refresh all values 
  // otherwise, refresh only a select amount of values
  String toSendToHub = feeding || refreshrequest || millis() - lastRefresh > 3600000 ? toSend : a + sep + b + sep + f + sep + h + sep + o;
  refreshrequest = false;

  bool hubSend = smartthingReq || millis() - lastRefresh > 600000;
  if (hubSend)
  {
    String c = "";
    for (int i = 0; i < toSendToHub.length(); i++)
    {
      if (toSendToHub.charAt(i) != ',')
      {
        c += toSendToHub.charAt(i);
      }
      else
      {
        sendToHub(c);
        c = "";
      }
    }
  }
}

void sendToHub(String var)
{
  st::Everything::sendSmartString(var);
  lastRefresh = millis();
}
