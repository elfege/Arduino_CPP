
void loop() {
// Serial.println(ClampIsOpen());

  UartReceive();
 
//    Serial.print("volts = "); Serial.println(getvolts());
//    delay(500);
  //st::Everything::run();


  //  Serial.println("----------------------BOOLEANS:--------------------------");
  //  Serial.println("");
  //
  //  Serial.print("BUMP = "); Serial.println(BUMP());
  //  Serial.print("BUMP = "); Serial.println(BUMP());
  //  Serial.print("BUMP RIGHT= "); Serial.println(BUMPR());
  //  Serial.print("BUMP LEFT = "); Serial.println(BUMPL());
  //  Serial.print("digitalRead(BL) = "); Serial.println(digitalRead(BL));
  //  Serial.print("digitalRead(BR) = "); Serial.println(digitalRead(BR));
  //
  //
  //  Serial.print("fusup = "); Serial.println(fusup());
  //  Serial.print("frontIR = "); Serial.println(frontIR());
  //
  //
  //  Serial.print("OBS = "); Serial.println(OBS());
  //  Serial.print("sideOBS = "); Serial.println(sideOBS());
  //  Serial.print("rightOBS = "); Serial.println(rightOBS());
  //  Serial.print("leftOBS = "); Serial.println(leftOBS());
  //  Serial.print("rearOBS = "); Serial.println(rearOBS());
  //  Serial.print("rearOBS = "); Serial.println(RearFrontOK());
  //
  //
  //
  //  Serial.print("RearLeftIR = "); Serial.println(RearLeftIR());
  //  Serial.print("RearRightIR = "); Serial.println(RearRightIR());
  //
  //
  //  Serial.println("----------------------VALUES:--------------------------");
  //  Serial.println("");
  //  Serial.print("frontIRVal = "); Serial.println(frontIRVal());
  //  Serial.print("fusupVAL = "); Serial.println(fusupVAL());
  //  Serial.print("fusupVAL_L = "); Serial.println(fusupVAL_L());
  //  Serial.print("fusupVAL_R = "); Serial.println(fusupVAL_R());
  //  Serial.print("fusRVAL = "); Serial.println(fusRVAL());
  //  Serial.print("fusLVAL = "); Serial.println(fusLVAL());
  //  Serial.print("rightIRVal = "); Serial.println(rightIRVal());
  //  Serial.print("leftIRVal = "); Serial.println(leftIRVal());
  //  Serial.print("RLIRVal = "); Serial.println(RLIRVal());
  //  Serial.print("RRIRVal = "); Serial.println(RRIRVal());
  //  Serial.print("RLIRVal = "); Serial.println(RLIRVal());
  //  Serial.print("BUMP: "); Serial.println(BUMP());
  //
  //
  //  Serial.println("");
  //  Serial.println("");
  //  Serial.println("");

  //  Serial.print("OBS: "); Serial.println(OBS());
  //  Serial.print("fusupVAL: "); Serial.println(fusupVAL());
  //  Serial.print("BUMP: "); Serial.println(BUMP());
  //  Serial.print("BUMPER PIN: "); Serial.println(digitalRead(BL));
  //
  //  Serial.print("fusup = "); Serial.println(fusup());
  //
  //  Serial.print("frontIR = "); Serial.println(frontIR());
  //  Serial.print("frontIRstr = "); Serial.println(frontIRstr());
  //  Serial.print("analogRead(FIR) = "); Serial.println(analogRead(FIR));
  //
  //  Serial.print("analogRead(RRIR) = "); Serial.println(analogRead(RRIR));
  //  Serial.print("RearRightIRstr() = "); Serial.println(RearRightIRstr());
  //
  //  Serial.print("analogRead(RLIR) = "); Serial.println(analogRead(RLIR));
  //  Serial.print("RearLeftIRstr() = "); Serial.println(RearLeftIRstr());
  //
  //
  //  Serial.print("analogRead(RLIR) = "); Serial.println(analogRead(RLIR));
  //
  //    Serial.print("leftIRVal = "); Serial.println(leftIRVal());
  //    Serial.print("rightIRVal = "); Serial.println(rightIRVal());


  //  foreArmDown();
  //  foreArmUp();
  //  armUp();
  //  armDown();

  //  logs();


}

void logs() {
  Serial.print("Arm is Down: "); Serial.println(ArmIsDown());
  Serial.print("Arm is Up: "); Serial.println(ArmIsUp());
  Serial.println("-----------------------------");
  Serial.print("ForeArm is Down: "); Serial.println(ForeArmIsDown());
  Serial.print("ForeArm is Up: "); Serial.println(ForeArmIsUp());
  Serial.println("-----------------------------");
  Serial.println("-----------------------------");

  delay(1500);
}


void Reset() {
  digitalWrite(RST, HIGH);
}
