void initXMLhttp() {
  _server.on("/", []() {
    _server.send_P(200, "text/html", MAIN_page);
  });

  _server.on("/getstates", []() {
    // if(debug) term.println("getstates");
    getStates(false);
  });

  _server.on("/debug", []() {
    debug = !debug;
    _server.send(200, "text/html", "DEBUG is " + String(debug ? "DEBUG ACTIVE" : "DEBUG DISABLED"));
  });

  _server.on("/reset", []() {
    Reset();
  });



  _server.on("/onoff", []() {

    if (digitalRead(POWER) == 1) {
      off();
    }
    else if (digitalRead(POWER) == 0) {
        on();
    }
    delay(10);
    String var = "onoff " + String(digitalRead(POWER) ? "TURN OFF" : "TURN ON");
    _server.send(200, "text/html", var);
  });

  _server.on("/ON", on);
  _server.on("/OFF", off);



  // cmds sent from MEBO's MAIN ESP32 CONTROLLER
  _server.on("/L", off);
  _server.on("/H", on);


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


  String a = "debug " + String(debug ? "Disable Debug" : "Enable Debug");
  String b = "onoff " + String(digitalRead(POWER) ? "TURN OFF" : "TURN ON");


  toSend = a + sep + b;  // + sep + c + sep + d + sep + e; //+ f; // no 'sep' at the end of the string sent to axios
  _server.send(200, "text/html", toSend);

  //for smart hub only
  String sw = "switch " + String(digitalRead(POWER) == HIGH ? "on" : "off");

  toSend = sw + sep;

  bool hubSend = smartthingReq || millis() - lastRefresh > 600000;
  if (hubSend) {
    String c = "";
    for (int i = 0; i < toSend.length(); i++) {
      if (toSend.charAt(i) != ',') {
        c += toSend.charAt(i);
      } else {
        if (debug) term.println("UPDATING HUB WITH THESE VALUES: " + c);
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