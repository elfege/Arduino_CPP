

void towerposition(int val)
{
  pos = tower.read();
  Serial.println("tower "+String(val));
  for (; pos <= val; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    tower.write(pos);    // tell servo to go to position in variable 'pos'
    loop();
  }
 for (; pos >= val; pos -= 1) { // goes from 180 degrees to 0 degrees
    tower.write(pos);    // tell servo to go to position in variable 'pos'
    loop();
  }
}

void armposition(int val)
{
  pos = arm.read();
  Serial.println("arm "+String(val));
  for (; pos <= val; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    arm.write(pos);    // tell servo to go to position in variable 'pos'
    loop();
  }
 for (; pos >= val; pos -= 1) { // goes from 180 degrees to 0 degrees
    arm.write(pos);    // tell servo to go to position in variable 'pos'
    loop();
  }
}

void forearmposition(int val)
{
  pos = forearm.read();
  Serial.println("forearm "+String(val));
  for (; pos <= val; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    forearm.write(pos);    // tell servo to go to position in variable 'pos'
    loop();
  }
 for (; pos >= val; pos -= 1) { // goes from 180 degrees to 0 degrees
    forearm.write(pos);    // tell servo to go to position in variable 'pos'
    loop();
  }
}

void wristposition(int val)
{
  pos = wrist.read();
  Serial.println("wrist "+String(val));
  for (; pos <= val; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    wrist.write(pos);    // tell servo to go to position in variable 'pos'
    loop();
  }
 for (; pos >= val; pos -= 1) { // goes from 180 degrees to 0 degrees
    wrist.write(pos);    // tell servo to go to position in variable 'pos'
    loop();
  }
}

void clampposition(int val)
{
  pos = clamp.read();
  Serial.println("clamp "+String(val));
  for (; pos <= val; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    clamp.write(pos);    // tell servo to go to position in variable 'pos'
    loop();
  }
 for (; pos >= val; pos -= 1) { // goes from 180 degrees to 0 degrees
    clamp.write(pos);    // tell servo to go to position in variable 'pos'
    loop();
  }
}
