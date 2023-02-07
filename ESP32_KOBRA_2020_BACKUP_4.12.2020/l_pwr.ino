#include <math.h>

void kobraOn() {
  Serial.println("POWERCamAndMotors ON");

  digitalWrite(POWERCamAndMotors, 1);
  String var = "switch on";

}
void kobraOff() {

  Serial.println("POWERCamAndMotors OFF");

  digitalWrite(POWERCamAndMotors, 0);
  String var = "switch off";

}

void chargeon() {
  // battery charging relay
  digitalWrite(CHARGER, 0);
  // sync with power controller

}
void chargeoff() {
  // battery charging relay
  digitalWrite(CHARGER, 1);// reversed logic on this relay
  // sync with power controller
 
}

void getChargeState()
{
  toggleCharge();
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

  boolean WasTurnedOff = false;
  if (digitalRead(CHARGER))// reversed logic on this relay

  {
    digitalWrite(CHARGER, 0);
    WasTurnedOff = true;

    // set a delay to leave enough time to read the value
    unsigned long currentMStart = millis();
    while (millis() - currentMStart < 10) {
      loop(); // allow other requests to work in the mean time
    }
  }
  float power = analogRead(PW);
  float volts = (power * 12.5 ) / (2047);            //power * (15 / 600.0);

// map(power, 0.0, 2047.0, 0.0, 12.50);
  //mapfloat(received.toFloat(), 0.0, 1023.0, 0.0, 21.0);
  //Serial.print(volts); Serial.println(" Volts");

  String state = String(volts);
  Serial.println("Analog Read PW = " + String(power));
  server.send(200, "text/plain", state);

  if (!digitalRead(CHARGER) && WasTurnedOff)
  {
    digitalWrite(CHARGER, 1); // turn off
  }

  return volts;
}
