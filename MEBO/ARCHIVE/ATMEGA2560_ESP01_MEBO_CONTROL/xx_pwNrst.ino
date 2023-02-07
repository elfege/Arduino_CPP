#include <math.h>


void poweron() {
  Serial.println("POWER ON");
  digitalWrite(RELAY1, 1);
  String var = "MEBO camon";
  st::Everything::sendSmartString(var);
}
void poweroff() {

  Serial.println("POWER OFF");
  digitalWrite(RELAY1, 0);
  String var = "MEBO camoff";
  st::Everything::sendSmartString(var);
}

void charge() {
  // battery charging relay
  digitalWrite(RELAY2, 0);
}
void stopcharge() {
  // battery charging relay
  digitalWrite(RELAY2, 1);
}
 
void Reset() {
  //Blink(5, 500);
  digitalWrite(RESET_PIN, HIGH);
}



