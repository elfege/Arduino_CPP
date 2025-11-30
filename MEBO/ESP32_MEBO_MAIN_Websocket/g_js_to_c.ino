
void UpdateSpeed() {

  //  rqSp = parseInt(document.getElementById("speed").value);

  SPEED = rqSp;
  Serial.print("SPEED UPDATED TO: "); Serial.println(SPEED);

  fwd(SPEED, 0);

}

void getFwd() {
  fwd(SPEED, 0);

  String state = MvtState;
  WebSocketsServer.send(200, "text/plain", state);
}

void getRvs() {
  rvs(SPEED, 0);

  String state = MvtState;
  WebSocketsServer.send(200, "text/plain", state);
}

void getLeft() {
  left(SPEED);

  String state = MvtState;
  WebSocketsServer.send(200, "text/plain", state);
}


void getRight() {
  right(SPEED);

  String state = MvtState;
  WebSocketsServer.send(200, "text/plain", state);
}

void getStop() {
  STOP();
  STOPALL = true;

  String state = MvtState;
  WebSocketsServer.send(200, "text/plain", state);
}


void toggleLED()
{
  digitalWrite(MEBORelay, !digitalRead(MEBORelay));
}

void getLEDState()
{
  toggleLED();
  String led_state = digitalRead(MEBORelay) ? "ON" : "OFF";
  WebSocketsServer.send(200, "text/plain", led_state);
}

int getSpeed(int speedVal) {
 
}

void CancelSpeedRequest() {
  HttpReq = false; // tell mvt function to get back to taking values from program's commands
}

void initJava() {
  WebSocketsServer.on("/", []()
  {
    WebSocketsServer.send_P(200, "text/html", MAIN_page);
  });


  WebSocketsServer.on("/ledstate", getLEDState);
  WebSocketsServer.on("/fwd", getFwd);

  WebSocketsServer.on("/rvs", getRvs);
  WebSocketsServer.on("/left", getLeft);
  WebSocketsServer.on("/right", getRight);
  WebSocketsServer.on("/stop", getStop);
  WebSocketsServer.on("/ArmUp", armUp);
  WebSocketsServer.on("/ArmDown", armDown);
  WebSocketsServer.on("/ForeArmUp", foreArmUp);
  WebSocketsServer.on("/ForeArmDown", foreArmDown);
  WebSocketsServer.on("/CloseClamp", closeClamp);
  WebSocketsServer.on("/OpenClamp", openClamp);

  int i = 0;
  int s = 200; // max speed value

  for (1; i < s; i++) {
//    Serial.println(i);
    String iString = String(i);
    String str1 = "/speedRange";
    String str2 = str1 + "=" + iString;
    WebSocketsServer.on(str2, [i]() {
      //Serial.print("GETTING NEW SPEED: "); Serial.println(i);

      HttpReq = true; // tell mvt function that we have a new value requested from the user for next cmd
      // set the global variable
      SPEED = i;
      // now store this value so next fwd or rvs request will apply it
      rqSp = SPEED;
     
      //delay(100);
    });
  }
  WebSocketsServer.on("/cancelSprq", CancelSpeedRequest);



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
