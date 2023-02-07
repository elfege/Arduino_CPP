
int servoDelay = 30;

void towerposition(int val)
{ 
  server.send(200, "text/html", "towerposition");
  pos = tower.read();
  Serial.println("tower " + String(val));
  for (; pos <= val; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    tower.write(pos);    // tell servo to go to position in variable 'pos'
    delay(servoDelay);
  }
  for (; pos >= val; pos -= 1) { // goes from 180 degrees to 0 degrees
    tower.write(pos);    // tell servo to go to position in variable 'pos'
    delay(servoDelay);
  }
}

void armposition(int val)
{
  server.send(200, "text/html", "armposition");
  pos = arm.read();
  Serial.println("arm " + String(val));
  for (; pos <= val; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    arm.write(pos);    // tell servo to go to position in variable 'pos'
    delay(servoDelay);
  }
  for (; pos >= val; pos -= 1) { // goes from 180 degrees to 0 degrees
    arm.write(pos);    // tell servo to go to position in variable 'pos'
    delay(servoDelay);
  }
}

void forearmposition(int val)
{
  server.send(200, "text/html", "forearmposition");
  pos = forearm.read();
  Serial.println("forearm " + String(val));
  for (; pos <= val; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    forearm.write(pos);    // tell servo to go to position in variable 'pos'
    delay(servoDelay);
  }
  for (; pos >= val; pos -= 1) { // goes from 180 degrees to 0 degrees
    forearm.write(pos);    // tell servo to go to position in variable 'pos'
    delay(servoDelay);
  }
}

void wristposition(int val)
{
  server.send(200, "text/html", "wristposition");
  pos = wrist.read();
  Serial.println("wrist " + String(val));
  for (; pos <= val; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    wrist.write(pos);    // tell servo to go to position in variable 'pos'
    delay(servoDelay);
  }
  for (; pos >= val; pos -= 1) { // goes from 180 degrees to 0 degrees
    wrist.write(pos);    // tell servo to go to position in variable 'pos'
    delay(servoDelay);
  }
}

void clampposition(int val)
{
  server.send(200, "text/html", "clampposition");
  servoDelay = 10;
  pos = clamp.read();
  Serial.println("clamp " + String(val));
  for (; pos <= val && pos <= 120; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    clamp.write(pos);    // tell servo to go to position in variable 'pos'
    delay(servoDelay);
  }
  for (; pos >= val && pos >= 35; pos -= 1) { // goes from 180 degrees to 0 degrees
    clamp.write(pos);    // tell servo to go to position in variable 'pos'
    delay(servoDelay);
  }
}

void servosdockposition()
{
  server.send(200, "text/html", "servosdockposition");
  int forearmaligned = 131;
  int armretracted = 60; // don't forget displayed value is inverted map of 0,180 in js for this servo (lower value upper the arm)
  int towercenter = 87; // don't forget displayed value is inverted map of 0,180 in js for this servo
  int clampretracted = 70; // don't forget displayed value is inverted map of 0,180 in js for this servo
  int wristcentered = 77;

  clampposition(clampretracted);
  armposition(armretracted);
  forearmposition(forearmaligned);
  towerposition(towercenter);
  wristposition(wristcentered);
  clampposition(clampretracted);

}
