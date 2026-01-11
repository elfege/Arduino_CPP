/*
  Morse.cpp - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Morse.h"

Morse::Morse(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void Morse::dot(int delval)
{
  digitalWrite(_pin, HIGH);
  delay(delval);
  digitalWrite(_pin, LOW);
  delay(delval);  
}

void Morse::dash(int delval)
{
  digitalWrite(_pin, HIGH);
  delay(delval);
  digitalWrite(_pin, LOW);
  delay(delval);
}