
void initXMLhttp() {
  server.on("/", []()
  {
    server.send_P(200, "text/html", MAIN_page);

  });
  server.on("/charge", getChargeState);// toggles
  server.on("/getchargestate", getChargeStateOnly); // doesn't toggle, just refreshes.
  server.on("/chargeoff", []()
  {
    Serial.println("Received charge OFF cmd from http");
    chargeoff();
  });
  server.on("/chargeon", []()
  {
    Serial.println("Received charge ON cmd from http");
    chargeon();
  });
  server.on("/camon", kobraOn);
  server.on("/camoff", kobraOff);
  server.on("/rvs", rvs);
  server.on("/fwd", fwd);
  server.on("/right", right);
  server.on("/left", left);
  server.on("/stop", MotorsStop);
  server.on("/trackback", tracskbackward);
  server.on("/trackfwd", tracskforward);
  server.on("/getvolts", getvolts);
  server.on("/logs", []() {
    logs = !logs;
  } );
  server.on("/reset", Reset);



  // speed range from slider
  int i = 0;
  int s = 255; // max speed value
  for (1; i < s; i++) {
    //    Serial.println(i);
    String iString = String(i);
    String str1 = "/speedRange";
    String str2 = str1 + "=" + iString;
    server.on(str2, [i, iString, HttpReq]() {
      Serial.print("NEW SPEED: "); Serial.println(i);

      HttpReq = true; // tell mvt function that we have a new value requested from the user for next cmd
      // set the global variable
      SPEED = i;
      // now store this value so next fwd or rvs request will apply it
      rqSp = SPEED;

      server.send(200, "text/html", "confirmed SPEED req: " + iString);
    });
  }

  i = 0;
  s = 180; // max servo position
  String str1 = "/towerrange";
  String str2 = "/armrange";
  String str3 = "/forearmrange";
  String str4 = "/wristrange";
  String str5 = "/clamprange";
  for (1; i < s; i++) {
    String iStringTower = String(map(i, 0, 180, 180, 0));
    String iString = String(i);
    String str21 = str1 + "=" + iStringTower;
    String str22 = str2 + "=" + iString;
    String str23 = str3 + "=" + iString;
    String str24 = str4 + "=" + iString;
    String str25 = str5 + "=" + iString;

    server.on(str21, [i, iStringTower]() {
      Serial.print("NEW TOWER POSITION: "); Serial.println(i);
      towerposition(i);
    });
    server.on(str22, [i, iString]() {
      Serial.print("NEW ARM POSITION: "); Serial.println(i);
      armposition(i);
    });
    server.on(str23, [i, iString]() {
      Serial.print("NEW FOREARM POSITION: "); Serial.println(i);
      forearmposition(i);
    });
    server.on(str24, [i, iString]() {
      Serial.print("NEW WRIST POSITION: "); Serial.println(i);
      wristposition(i);
    });
    server.on(str25, [i, iString]() {
      Serial.print("NEW CLAMP POSITION: "); Serial.println(i);
      clampposition(i);
    });
  }
}
