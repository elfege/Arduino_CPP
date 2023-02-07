/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

#define RECV_PIN 2

#define LEDPIN 10

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(115000);
   pinMode(LEDPIN, OUTPUT);
   int i = 0;
  for (i = 0; i < 10; i++) {
    digitalWrite(LEDPIN, 1);
    delay(50);
    digitalWrite(LEDPIN, 0);
    delay(50);
  }
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
    digitalWrite(LEDPIN, 1);
    delay(50);
    digitalWrite(LEDPIN, 0);
    delay(50);
  }
  delay(500);
}
