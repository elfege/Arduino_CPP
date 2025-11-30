#include <math.h>


void kobraOn() {
  Serial.println("POWER ON");

  digitalWrite(POWER, 1);
  String var = "switch on";
  //  st::Everything::sendSmartString(var);

}
void kobraOff() {

  Serial.println("POWER OFF");

 digitalWrite(POWER, 0);
  String var = "switch off";
  // st::Everything::sendSmartString(var);

}

void charge() {
  // battery charging relay
  digitalWrite(CHARGERelay, 0);
  //refresh();
}
void stopcharge() {
  // battery charging relay
  Serial.println("STOPPING CHARGE PROCESS");
  digitalWrite(CHARGERelay, 1);
  //refresh();
}

void Reset() {
  Serial.println("*****************************************************************************");
  Serial.println("                               RESTARTING ESP                                ");
  Serial.println("*****************************************************************************");
  //Blink(10, 50);
  esp_restart();
}
