/////////////////////////////////////////////////////////////////

#include "ESPRotary.h";

/////////////////////////////////////////////////////////////////
#define LED 2
#define ROTARY_PIN1	25
#define ROTARY_PIN2	32

/////////////////////////////////////////////////////////////////

ESPRotary r = ESPRotary(ROTARY_PIN1, ROTARY_PIN2);

/////////////////////////////////////////////////////////////////

void setup() {
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
  
  Serial.begin(9600);
  delay(50);
  Serial.println("\n\nSimple Counter");
  pinMode(27, OUTPUT);
  digitalWrite(27, HIGH);
  
  r.setChangedHandler(rotate);
  r.setLeftRotationHandler(showDirection);
  r.setRightRotationHandler(showDirection);

  Serial.println("END OF SETUP");
   Blink(10, 50);
}

void loop() {
  r.loop(); 
  //Blink(1, 50);
}

/////////////////////////////////////////////////////////////////

void rotate(ESPRotary& r) {
   Serial.println(r.getPosition());
   Blink(1, 50);
}


void showDirection(ESPRotary& r) {
  Serial.println(r.directionToString(r.getDirection()));
  //Blink(2, 50);
}

/////////////////////////////////////////////////////////////////


void Blink(int times, int lapse) {
  int c = 0;
  while (c < times) {
    digitalWrite(LED, 1);
    delay(lapse);
    digitalWrite(LED, 0);
    delay(lapse);
    c++;
  }
}
