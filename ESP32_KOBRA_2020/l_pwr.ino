#include <math.h>

void motorspw() {

  String debug = digitalRead(MotorsPw) ? "TURNING MOTORS OFF" : "TURNING MOTORS ON";
  Serial.println(debug);
  digitalWrite(MotorsPw, !digitalRead(MotorsPw));
  delay(1);
  motorspwStatus();
}

void motorspwStatus() {
  String state = digitalRead(MotorsPw) ? "MOTORS ARE ON" : "MOTORS ARE OFF";
  server.send(200, "text/plain", state);
}


void chargeon() {
  // battery charging relay
  digitalWrite(CHARGER, 0);
  // sync with power controller
  String state = "";
  if (digitalRead(CHARGER)) {
    state = "KOBRA IS NOT CHARGING";
    docking = true;// mark it as true to prevent switch toggle in float getVolts()
    //so we can see actual current and maybe dock
  }
  else
  {
    state = "KOBRA IS CHARGING";
    docking = false; // allow current status check
  }

  server.send(200, "text/plain", state);
}
void chargeoff() {
  // battery charging relay
  digitalWrite(CHARGER, 1);// reversed logic on this relay
  // sync with power controller
  String state = digitalRead(CHARGER) ? "KOBRA IS NOT CHARGING" : "KOBRA IS CHARGING";
  server.send(200, "text/plain", state);
}

void getChargeState()
{
  if (!docking) {
    toggleCharge();
  }
  String state = digitalRead(CHARGER) ? "KOBRA IS NOT CHARGING" : "KOBRA IS CHARGING";
  server.send(200, "text/plain", state);
}
void getChargeStateOnly()
{
  String state = digitalRead(CHARGER) ? "KOBRA IS NOT CHARGING" : "KOBRA IS CHARGING";
  server.send(200, "text/plain", state);
}
void toggleCharge()
{
  digitalRead(CHARGER) ? chargeon() : chargeoff();
}



float getvolts() {

  boolean WasturnedOn = false;
  float power;
  if (digitalRead(CHARGER) && !docking)
    // reversed logic on this relay
    // !docking is to prevent false positives in the boolean "haspower()"
  {
    digitalWrite(CHARGER, 0);
    WasturnedOn = true;
    // set a delay to leave enough time to read the value
  }
  unsigned long currentMStart = millis();
  while (millis() - currentMStart < 100) { // in any case, do a 100 millis() read for better accuracy
    power = analogRead(PW);
    loop(); // allow other requests to work in the mean time
  }

  float volts =  (power * 11.4 ) / (2600);            //power * (15 / 600.0);
  String state = String(volts);
  //  Serial.print("Battery Level: " + state); Serial.println(" Volts");
  //  Serial.println("Analog Read PW = " + String(power));

  server.send(200, "text/plain", state); // update webpage value


  if (!digitalRead(CHARGER) && WasturnedOn && volts > 10)
  {
    digitalWrite(CHARGER, 1); // turn off
  }

  return volts;
}

float getvoltsonly() { // no switching
  float power = analogRead(PW);
  float volts =  (power * 11.4 ) / (2600);            //power * (15 / 600.0);
  String state = String(volts);

  server.send(200, "text/plain", state); // update webpage value

  return volts;
}
