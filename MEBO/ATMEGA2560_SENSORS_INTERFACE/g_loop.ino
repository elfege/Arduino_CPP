
void loop() {
  tasks();

//  Serial.println("!digitalRead(Arm_Center)"+ String(!digitalRead(Arm_Center)));
}

void tasks() {
  UartReceive();
//  if(millis() - lastWtdCall >= 3000)
//  {
//    watchdog();
//    lastWtdCall = millis();
//  }
  
  if (millis() - logsMillis > 500 && logs)
  {
    if (millis() - logsStartedMillis > 1000 * 60 * 3) // stop debug after 3 minutes
    {
      logs = false;
      Serial.println("DEBUG TIMEOUT");
      return;
    }
    debug();
  }
  if (fusRVAL() < 5 && !ForeArmIsUp()) {
    //Serial.println("Forearm less than 5 cm from obstacle/ground");
    digitalWrite(ForeArm1, 0);
    digitalWrite(ForeArm2, 1);
    Start = millis();
    while (millis() - Start < 200 && !stoparm)
    {
      tasks();
    }
    foreArmStop();
  }
}
void watchdog(){
  
  UartSend("&w*");
  if(millis() - wtdResponseMillis > 10000)
  {
    Serial.println("ESP HASN'T RESPONDED IN MORE THAN 10 SECONDS, RESETING");
    delay(500);
    digitalWrite(RST, HIGH);
  }
  else
  {
    Serial.println("ESP RESPONDED TO WTD REQUEST");
    wtdResponseMillis = millis();
  }
}
void debug() {

  //  Serial.print("volts = "); Serial.println(getvolts());
  Serial.print("BUMP = "); Serial.println(BUMP());
  Serial.print("BUMP RIGHT= "); Serial.println(BUMPR());
  Serial.print("BUMP LEFT = "); Serial.println(BUMPL());
  Serial.print("digitalRead(BL) = "); Serial.println(digitalRead(BL));
  Serial.print("digitalRead(BR) = "); Serial.println(digitalRead(BR));
  Serial.print("fusup = "); Serial.println(fusup());
  Serial.print("frontIR = "); Serial.println(frontIR());
  Serial.print("OBS = "); Serial.println(OBS());
  Serial.print("sideOBS = "); Serial.println(sideOBS());
  Serial.print("rightOBS = "); Serial.println(rightOBS());
  Serial.print("leftOBS = "); Serial.println(leftOBS());
  Serial.print("rearOBS = "); Serial.println(rearOBS());
  Serial.print("rearOBS = "); Serial.println(RearFrontOK());
  Serial.print("RearLeftIR = "); Serial.println(RearLeftIR());
  Serial.print("RearRightIR = "); Serial.println(RearRightIR());
  Serial.print("fusupVAL = "); Serial.println(fusupVAL());
  Serial.print("fusupVAL_L = "); Serial.println(fusupVAL_L());
  Serial.print("fusupVAL_R = "); Serial.println(fusupVAL_R());
  Serial.print("fusRVAL = "); Serial.println(fusRVAL());
  Serial.print("fusLVAL = "); Serial.println(fusLVAL());
  Serial.print("rightIRVal = "); Serial.println(rightIRVal());
  Serial.print("leftIRVal = "); Serial.println(leftIRVal());
  Serial.print("RLIRVal = "); Serial.println(RLIRVal());
  Serial.print("RRIRVal = "); Serial.println(RRIRVal());
  Serial.print("RLIRVal = "); Serial.println(RLIRVal());
  Serial.print("BUMP: "); Serial.println(BUMP());


  Serial.println("");
  Serial.println("");
  Serial.println("");



}


void Reset() {
  digitalWrite(RST, HIGH);
}
