
void getFwd() {
  fwd(SPEED, 0);

  _server.send(200, "text/plain", MvtState);
}

void getRvs() {
  rvs(SPEED, 0);

  String state = MvtState;
  _server.send(200, "text/plain", state);
}

void getLeft() {
  left(SPEED);

  String state = MvtState;
  _server.send(200, "text/plain", state);
}


void getRight() {
  right(SPEED);

  String state = MvtState;
  _server.send(200, "text/plain", state);
}

void getStop() {
  STOP();
  //  STOPALL = true;

  String state = MvtState;
  _server.send(200, "text/plain", state);
}

void toggleCharge() {
  digitalWrite(CHARGERelay, !digitalRead(CHARGERelay));
}


void getChargeState() {
  toggleCharge();
  String state = digitalRead(CHARGERelay) ? "NOT CHARGING" : "CHARGING";
  _server.send(200, "text/plain", state);

}




// update values without toggling anything (onload);

void updateChargeState() {
  String state = digitalRead(CHARGERelay) ? "NOT CHARGING" : "CHARGING";
  _server.send(200, "text/plain", state);
}



void CancelSpeedRequest() {
  HttpReq = false; // tell mvt function to get back to taking values from program's commands
}



void executeFWD() {
  if (httpdist == 0) {
    httpdist = 10;
  }
  term.print("EXECUTING NEW DISTANCE: "); term.print(httpdist);
  term.print(" AT SPEED: "); term.println(SPEED);
  fwd(SPEED, httpdist);

  String state = "FORWARD over " + String(httpdist) + "cm with a speed parameter of: " + SPEED;
  _server.send(200, "text/plain", cmd);
}
void executeRVS() {
  if (httpdist == 0) {
    httpdist = 10;
  }
  term.print("EXECUTING NEW DISTANCE: "); term.print(httpdist);
  term.print(" AT SPEED: "); term.println(SPEED);
  String state = "REVERSE over " + String(httpdist) + "cm with a speed parameter of: " + SPEED;
  _server.send(200, "text/plain", cmd);

  rvs(SPEED, httpdist);

}

void initXMLhttp() {
  _server.on("/", []()
  {
    term.println("NEW CLIENT");
    _server.send_P(200, "text/html", MAIN_page);

  });

   
  _server.on("/charge", getChargeState);
  _server.on("/updatecharge", updateChargeState);
  _server.on("/getvolts", getvolts);

  _server.on("/ON", meboOn);
  _server.on("/OFF", meboOff);


  _server.on("/fwd", getFwd);
  _server.on("/rvs", getRvs);
  _server.on("/left", getLeft);
  _server.on("/right", getRight);
  _server.on("/stop", getStop);
  _server.on("/logs", []() {
    logs = !logs;
    logDone = true;
  } );
  _server.on("/fullstop", simpleSTOP);

  _server.on("/ArmUp", armUp);
  _server.on("/ArmDown", armDown);
  _server.on("/armupstep", armUpStep);
  _server.on("/armdownstep", armDownStep);
  _server.on("/armcenter", armCenter);


  _server.on("/ForeArmUp", foreArmUp);
  _server.on("/ForeArmDown", foreArmDown);
   _server.on("/forearmupstep", foreArmUpStep);
  _server.on("/forearmdowntep", foreArmDownStep);
  _server.on("/stoparm", stopArm);
  _server.on("/CloseClamp", closeClamp);
  _server.on("/OpenClamp", openClamp);

  _server.on("/wristleft", wristLeft);
  _server.on("/wristright", wristRight);
  _server.on("/wristmiddle", wristMiddle);

  _server.on("/wristleftstep", wristleftstep);
  _server.on("/wristrightstep", wristrightstep);

  _server.on("/followlinefwd", FollowLineFWD);
  _server.on("/followlinervs",FollowLineRVS);

  _server.on("/dock", dock);

  _server.on("/executefwd", executeFWD);
  _server.on("/executervs", executeRVS);
  
  _server.on("/reset", Reset);
  _server.on("/cancelSprq", CancelSpeedRequest);

  _server.on("/selfdrive", []() {
    roam();
  });

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

  _server.on("/ignoreobs", [ignoreobs]() {
    ignoreobs = !ignoreobs;// toggle value
    String state = ignoreobs ? "Obstacles Management INACTIVE" : "Obstacles Management ACTIVE"  ;
    _server.send(200, "text/html", state);
  });

  // speed range from slider
  int i = 0;
  int s = 255; // max speed value
  for (1; i < s; i++) {
    //    term.println(i);
    String iString = String(i);
    String str1 = "/speedRange";
    String str2 = str1 + "=" + iString;
    _server.on(str2, [i, iString, HttpReq]() {
      term.print("NEW SPEED: "); term.println(i);

      HttpReq = true; // tell mvt function that we have a new value requested from the user for next cmd
      // set the global variable
      SPEED = i;
      // now store this value so next fwd or rvs request will apply it
      rqSp = SPEED;

      _server.send(200, "text/html", "confirmed SPEED req: " + iString);
    });
  }

  //distance from slider
  i = 0;
  s = 201; // max distance value
  for (1; i < s; i++) {
    //    term.println(i);
    String iString = String(i);
    String str1 = "/distanceRange";
    String str2 = str1 + "=" + iString;
    _server.on(str2, [i, iString]() {
      term.print("RECEIVED NEW DISTANCE: "); term.println(i);

      HttpReq = true; // tell mvt function that we have a new value requested from the user for next cmd
      HttpDistReq = true; // tell mvt function to take httpdist as a new parameter
      // set the global variable
      httpdist = i;
      _server.send(200, "text/html", "confirmed DISTANCE req: " + iString);
    });
  }


}
