
void UpdateSpeed() {

  //  rqSp = parseInt(document.getElementById("speed").value);

  SPEED = rqSp;
  Serial.print("SPEED UPDATED TO: "); Serial.println(SPEED);
  
  fwd(SPEED, 0);

}

void getFwd() {
  fwd(SPEED, 0);
  
  String state = MvtState;
  _server.send(200, "text/plain", state);
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
  STOPALL = true;

  String state = MvtState;
  _server.send(200, "text/plain", state);
}


void toggleLED()
{
  digitalWrite(CamAndArmsRelay, !digitalRead(CamAndArmsRelay));
  
}

void getCAMstate()
{
  toggleLED();
  String led_state = digitalRead(CamAndArmsRelay) ? "TURN OFF" : "TURN ON";
  _server.send(200, "text/plain", led_state);
}

int getSpeed(int speedVal) {

}

void CancelSpeedRequest() {
  HttpReq = false; // tell mvt function to get back to taking values from program's commands
}

void initJava() {
  _server.on("/", []()
  {
    _server.send_P(200, "text/html", MAIN_page);

  });

//_server.on("/powerOff", powerOff); // to auxiliary power ip... 
  _server.on("/ledstate", getCAMstate);


  
  _server.on("/fwd", getFwd);
  _server.on("/rvs", getRvs);
  _server.on("/left", getLeft);
  _server.on("/right", getRight);
  _server.on("/stop", getStop);
  _server.on("/ArmUp", armUp);
  _server.on("/ArmDown", armDown);
  _server.on("/ForeArmUp", foreArmUp);
  _server.on("/ForeArmDown", foreArmDown);
  _server.on("/CloseClamp", closeClamp);
  _server.on("/OpenClamp",openClamp);
  /*[]() {
    // note time cmd was sent
    if (millis() - previousCmdTime > 10) {
      previousCmdTime = millis();
      openClamp();
    }
    else {
      Serial.println("cmd ignored");
    }
  });*/
  
  _server.on("/reset", Reset);
  _server.on("/cancelSprq", CancelSpeedRequest);


  int i = 0;
  int s = 200; // max speed value

  for (1; i < s; i++) {
    //    Serial.println(i);
    String iString = String(i);
    String str1 = "/speedRange";
    String str2 = str1 + "=" + iString;
    _server.on(str2, [i]() {
      //Serial.print("GETTING NEW SPEED: "); Serial.println(i);

      HttpReq = true; // tell mvt function that we have a new value requested from the user for next cmd
      // set the global variable
      SPEED = i;
      // now store this value so next fwd or rvs request will apply it
      rqSp = SPEED;

      //delay(100);
    });
  }

}

void WiFiResponseMgt() {



}



//Serial.print("GETTING NEW SPEED: ");Serial.println(speedVal);
//  String thisVal = speedVal;
//  char this_char[thisVal.length() + 1];
//  thisVal.toCharArray(this_char, sizeof(this_char));
//
//  HttpReq = true; // tell mvt function that we have a new value requested from the user for next cmd
//  // set the global variable
//  SPEED = atoi(this_char);
//  // now store this value so next fwd or rvs request will apply it
//  rqSp = SPEED;
//  delay(100);
