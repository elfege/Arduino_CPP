
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
void updateCamState() {
  String state = digitalRead(CamAndArmsRelay) ? "TURN OFF CAMERA" : "TURN ON CAMERA";
  _server.send(200, "text/plain", state);
}
void updateChargeState() {
  String state = digitalRead(CHARGERelay) ? "NOT CHARGING" : "CHARGING";
  _server.send(200, "text/plain", state);
}

void toggleRelayCam()
{
  digitalWrite(CamAndArmsRelay, !digitalRead(CamAndArmsRelay));

}
void getCAMstate()
{
  toggleRelayCam();
  String state = digitalRead(CamAndArmsRelay) ? "TURN OFF CAMERA" : "TURN ON CAMERA";
  _server.send(200, "text/plain", state);
}


void CancelSpeedRequest() {
  HttpReq = false; // tell mvt function to get back to taking values from program's commands
}

void GetStopArm() {
  cmd = "+stoparm-";
  int t = 0;
  String received = "";
  UartSend(cmd);
  _server.send(200, "text/plain", cmd);

}

void executeFWD() {
  if (httpdist == 0) {
    httpdist = 10;
  }
  Serial.print("EXECUTING NEW DISTANCE: "); Serial.print(httpdist);
  Serial.print(" AT SPEED: "); Serial.println(SPEED);
  fwd(SPEED, httpdist);

  String state = "FORWARD over " + String(httpdist) + "cm with a speed parameter of: " + SPEED;
  _server.send(200, "text/plain", cmd);



}
void executeRVS() {
  if (httpdist == 0) {
    httpdist = 10;
  }
  Serial.print("EXECUTING NEW DISTANCE: "); Serial.print(httpdist);
  Serial.print(" AT SPEED: "); Serial.println(SPEED);
  String state = "REVERSE over " + String(httpdist) + "cm with a speed parameter of: " + SPEED;
  _server.send(200, "text/plain", cmd);

  rvs(SPEED, httpdist);

}

void initXMLhttp() {
  _server.on("/", []()
  {
    _server.send_P(200, "text/html", MAIN_page);

  });

  _server.on("/video", []()
  {
    _server.send_P(200, "text/html", video_page);

  });

  _server.on("/camstate", getCAMstate);
  _server.on("/charge", getChargeState);
  _server.on("/updatecam", updateCamState);
  _server.on("/updatecharge", updateChargeState);
  _server.on("/getvolts", getvolts);


  _server.on("/fwd", getFwd);
  _server.on("/rvs", getRvs);
  _server.on("/left", getLeft);
  _server.on("/right", getRight);
  _server.on("/stop", getStop);
  _server.on("/logs", [](){
    logs = !logs;
  } );
  _server.on("/fullstop", fullStop);

  _server.on("/ArmUp", armUp);
  _server.on("/ArmDown", armDown);
  _server.on("/armupstep", armUpStep);
  _server.on("/armdownstep", armDownStep);
  _server.on("/armcenter", armCenter);


  _server.on("/ForeArmUp", foreArmUp);
  _server.on("/ForeArmDown", foreArmDown);
  _server.on("/stoparm", GetStopArm);
  _server.on("/CloseClamp", closeClamp);
  _server.on("/OpenClamp", openClamp);

  _server.on("/wristleft", wristLeft);
  _server.on("/wristright", wristRight);
  _server.on("/wristmiddle", wristMiddle);

  _server.on("/wristleftstep", wristleftstep);
  _server.on("/wristrightstep", wristrightstep);

  _server.on("/followlinefwd", FollowLineFWD);
  _server.on("/followlinervs", FollowLineRVS);
  _server.on("/dock", dock);

  _server.on("/executefwd", executeFWD);
  _server.on("/executervs", executeRVS);

  _server.on("/selfdrive", []() {

    selfdrive();
    

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

  _server.on("/ignoreobs", []() {
    ignoreobs = !ignoreobs;
    String state = ignoreobs ? "Obstacles Management ACTIVE" : "Obstacles Management INACTIVE";
    _server.send(200, "text/html", state);
  });


  _server.on("/reset", Reset);
  _server.on("/cancelSprq", CancelSpeedRequest);


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

  //distance from slider
  i = 0;
  s = 200; // max distance value
  for (1; i < s; i++) {
    //    Serial.println(i);
    String iString = String(i);
    String str1 = "/distanceRange";
    String str2 = str1 + "=" + iString;
    _server.on(str2, [i, iString]() {
      Serial.print("RECEIVED NEW DISTANCE: "); Serial.println(i);

      HttpReq = true; // tell mvt function that we have a new value requested from the user for next cmd
      HttpDistReq = true; // tell mvt function to take httpdist as a new parameter
      // set the global variable
      httpdist = i;
      _server.send(200, "text/html", "confirmed DISTANCE req: " + iString);
    });
  }


}
