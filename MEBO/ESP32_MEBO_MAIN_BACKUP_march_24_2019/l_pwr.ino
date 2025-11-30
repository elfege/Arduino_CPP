#include <math.h>


void meboOn() {
  Serial.println("POWER ON");

 IPAddress SERVER(192, 168, 10, 105);
  WiFiClient client;
  if (client.connect(SERVER, 80)) {
    client.println("GET /H HTTP/1.0");
  }
  String var = "switch on";
  //  st::Everything::sendSmartString(var);

}
void meboOff() {

  Serial.println("POWER OFF");

  // Initialize the client library
  IPAddress SERVER(192, 168, 10, 105);
  WiFiClient client;
  if (client.connect(SERVER, 80)) {
    client.println("GET /L HTTP/1.0");
  }
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
  Serial.println("HARDWARE RESET...");
  Blink(10, 50);

  digitalWrite(RST, HIGH);
  delay(1000);
  Serial.println("");
  Serial.println("");
  Serial.println("HARDWARE RESET FAILED!!!");
  Blink(5, 500);
  delay(1000);
  Serial.println("");
  Serial.println("");
  Serial.println("SOFTWARE RESET");

  ESP.restart();
}
