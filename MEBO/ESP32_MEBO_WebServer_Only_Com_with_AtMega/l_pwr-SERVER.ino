#include <math.h>


void poweron() {
  Serial.println("POWER ON");
  digitalWrite(RELAY1, 1);
  String var = "MEBO camon";
  
}
void poweroff() {

  Serial.println("POWER OFF");
  digitalWrite(RELAY1, 0);
  String var = "MEBO camoff";
  
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

  digitalWrite(RST, HIGH);
  delay(100);
  Serial.println("HARDWARE RESET FAILED...");
  ESP.restart();
}



