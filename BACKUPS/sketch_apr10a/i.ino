
void initXMLhttp() {
  _server.on("/", []()
  {
    _server.send_P(200, "text/html", MAIN_page);

  });

  _server.on("/ON", kobraOn);
  _server.on("/OFF", kobraOff);

  _server.on("/rvs", rvs);
  _server.on("/fwd", fwd);
  _server.on("/right", right);
  _server.on("/right", right);
 // _server.on("/stop", stop);
  
  

  _server.on("/logs", []() {
    logs = !logs;
  } );
  _server.on("/cancelstopall", []() {
    STOPALL = !STOPALL;
    String state = STOPALL ? "STOPALL ON" : "STOPALL OFF";
    _server.send(200, "text/html", state);
  });
  _server.on("/stopall", []() {
    STOPALL = !STOPALL;
    String state = STOPALL ? "STOPALL ON" : "STOPALL OFF";
    _server.send(200, "text/html", state);
  });




  _server.on("/reset", Reset);
  


  // speed range from slider
  int i = 0;
  int s = 255; // max speed value
  for (1; i < s; i++) {
    //    Serial.println(i);
    String iString = String(i);
    String str1 = "/speedRange";
    String str2 = str1 + "=" + iString;
    _server.on(str2, [i, iString, HttpReq]() {
      Serial.print("NEW SPEED: "); Serial.println(i);

      HttpReq = true; // tell mvt function that we have a new value requested from the user for next cmd
      // set the global variable
      SPEED = i;
      // now store this value so next fwd or rvs request will apply it
      rqSp = SPEED;

      _server.send(200, "text/html", "confirmed SPEED req: " + iString);
    });
  }

}
