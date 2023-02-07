void initXMLhttp() {
  _server.on("/", []() {
    _server.send_P(200, "text/html", MAIN_page);
  });

  _server.on("/getstates", []() {
    // term.println("getstates");
    getStates(false);
  });

  _server.on("/debug", []() {
    debug = !debug;
    _server.send(200, "text/html", "DEBUG is " + String(debug ? "ACTIVE" : "OFF"));
  });

  _server.on("/reset", []() {
    Reset();
  });

  _server.on("/simulator", []() {
    test();
  });

  _server.on("/talk", []() {
   talk();
  });
  _server.on("/open", []() {
    open();
  });

  _server.on("/getip", []() {
    String localIp = WiFi.localIP().toString();
    if (debug) term.println("_server.send() => " + localIp);
    _server.send(200, "text/html", localIp);
  });
  _server.on("/getmac", []() {
    String var = WiFi.macAddress();
    _server.send(200, "text/html", var);
  });
  _server.on("/refresh", []() {
    Refresh();
  });
}


void getStates(bool smartthingReq) {
  String toSend = "";
  String sep = ",";


  //for html page
  String a = "debug " + String(debug ? "ACTIVE" : "OFF");
  String b = "bell " + String(voltage >= Val ? "ACTIVE" : "INACTIVE");
  String c = "open " + String(digitalRead(DOOR) == PRESS ? "OPENING" : "DOOR");

  //for both html page and hub
  String d = "talk " + String(digitalRead(TALK) == PRESS ? "TALKING" : "TALK");

  toSend = a + sep + b + sep + c + sep + d + sep;
  _server.send(200, "text/html", toSend);

  //for smart hub only
  String door = "door " + String(digitalRead(DOOR) == PRESS ? "open" : "closed");
  String door_sw = "switch " + String(digitalRead(DOOR) == PRESS ? "on" : "off");
  String acc = "acceleration " + String(voltage >= Val ? "active" : "inactive");

  b.toLowerCase(); // can't have upper case with hubs events
  d.toLowerCase();  
  
  toSend = door + sep + door_sw + sep + b + sep + d + sep + acc + sep;

  bool hubSend = smartthingReq || millis() - lastRefresh > 600000;
  if (hubSend) {
    String c = "";
    for (int i = 0; i < toSend.length(); i++) {
      if (toSend.charAt(i) != ',') {
        c += toSend.charAt(i);
      } else {
        term.println("UPDATING HUB WITH THESE VALUES: "+c);
        sendToHub(c);
        c = "";
      }
    }
  }
}

void sendToHub(String var) {
  smartthing.send(var);
  lastRefresh = millis();
}