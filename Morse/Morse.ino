#include <Morse.h>

Morse morse(13);
int delval = 50;

void setup()
{
  
  
}

void loop()
{
  
  morse.dot(delval); morse.dot(delval); morse.dot(delval);
  morse.dash(delval); morse.dash(delval); morse.dash(delval);
  morse.dot(delval); morse.dot(delval); morse.dot(delval);
  delay(3000);
}
