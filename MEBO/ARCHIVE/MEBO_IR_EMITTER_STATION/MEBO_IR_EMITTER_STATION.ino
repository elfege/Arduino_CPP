#include <IRremote.h>
unsigned int raw[100] = {4428, 4376, 580, 1600, 580, 540, 552, 1608, 580, 516, 548, 560, 560, 520, 580, 536, 556, 1604, 576, 1612};
IRsend irsend;

#define LEDPIN 13

void setup()
{
  Serial.begin(115000);
  pinMode(3, OUTPUT);
  pinMode(LEDPIN, OUTPUT);

  digitalWrite(3, 1);
  int i = 0;
  for (i = 0; i < 10; i++) {
    digitalWrite(LEDPIN, 1);
    delay(50);
    digitalWrite(LEDPIN, 0);
    delay(50);
  }
}

void loop() {

  irsend.sendSony(0xa90, 12);

  delay(100);
}
