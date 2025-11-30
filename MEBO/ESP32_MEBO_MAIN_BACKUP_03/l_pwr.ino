#include <math.h>


void meboOn() {
  Serial.println("POWER ON");
  digitalWrite(CamAndArmsRelay, 1);
  String var = "MEBO camon";
  st::Everything::sendSmartString(var);

}
void meboOff() {

  Serial.println("POWER OFF");
  digitalWrite(CamAndArmsRelay, 0);
  String var = "MEBO camoff";
  st::Everything::sendSmartString(var);

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
  Serial.println("HARDWARE RESET...");
  Blink(10, 50);

  digitalWrite(RST, HIGH);
  delay(1000);
  Serial.println("");
  Serial.println("");
  Serial.println("HARDWARE RESET FAILED!!!");
   Blink(5, 1500);
  delay(1000);
  Serial.println("");
  Serial.println("");
  Serial.println("SOFTWARE RESET");

  ESP.restart();
}
