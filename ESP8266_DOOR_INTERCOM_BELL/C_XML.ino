void initXMLhttp() {
  _server.on("/", []() {
    term.println("HTTP / request START");
    _server.send_P(200, "text/html", MAIN_page);
    term.println("HTTP / request END");
  });

  _server.on("/getstates", []() {
    // term.println("getstates");
    getStates(false, "javascript");
  });

  _server.on("/debug", []() {
    debug = !debug;
    _server.send(200, "text/html", "DEBUG is " + String(debug ? "ACTIVE" : "OFF"));
  });

  _server.on("/reset", []() {
    Reset();
    _server.send(200, "text/html", "reset");
  });

  _server.on("/simulator", []() {
    test();
    _server.send(200, "text/html", "test");
  });

  _server.on("/talk", []() {
    talk();
    _server.send(200, "text/html", "talk");
  });
  _server.on("/door", []() {
    unlock_door();
    _server.send(200, "text/html", "open");
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
    _server.send(200, "text/html", "refresh");
  });
}


void getStates(bool smartthingReq, String origin) {

  // term.println("getStates req. from: "+origin);
  // term.println("smartthingReq: " + String(smartthingReq ? "YES" : "NO"));

  String toSend = "";
  String sep = ",";


  //for html page
  String a = "debug " + String(debug ? "ACTIVE" : "OFF");
  String b = "bell " + String(sensorValue >= threshold ? "ACTIVE" : "INACTIVE");
  String c = "door " + String(digitalRead(DOOR) == PRESS ? "OPENING" : "CLOSED");

  //for both html page and hub
  String d = "talk " + String(digitalRead(TALK) == PRESS ? "TALKING" : "TALK");

  toSend = a + sep + b + sep + c + sep + d + sep;
  // term.println("sent to client: "+toSend);
  _server.send(200, "text/html", toSend);

  //for smart hub only
  String door = "door " + String(digitalRead(DOOR) == PRESS ? "open" : "closed");
  String door_sw = "switch " + String(digitalRead(DOOR) == PRESS ? "on" : "off");
  String acc = "acceleration " + String(sensorValue >= threshold ? "active" : "inactive");

  b.toLowerCase();  // can't have upper case with hubs events
  d.toLowerCase();

  toSend = door + sep + door_sw + sep + b + sep + d + sep + acc + sep;


  bool hubSend = runsim || smartthingReq || millis() - lastRefresh > 600000;
  if (hubSend) {
    String c = "";
    for (int i = 0; i < toSend.length(); i++) {
      if (toSend.charAt(i) != ',') {
        c += toSend.charAt(i);
      } else {
        term.println("UPDATING HUB: " + c);
        send_event(c);
        c = "";
      }
    }
  }
}
