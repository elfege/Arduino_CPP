//*************************************************************************************************
//NodeMCU v1.0 ESP8266-12e Pin Definitions (makes it much easier as these match the board markings)
//*************************************************************************************************
//#define LED_BUILTIN 16
//#define BUILTIN_LED 16
//
//#define D0 16  //no internal pullup resistor
//#define D1  5
//#define D2  4
//#define D3  0  //must not be pulled low during power on/reset, toggles value during boot
//#define D4  2  //must not be pulled low during power on/reset, toggles value during boot
//#define D5 14
//#define D6 12
//#define D7 13
//#define D8 15  //must not be pulled high during power on/reset
//*****************************************************************************

#define LED D4

// Include the Arduino Stepper Library
#include <Stepper.h>

const int stepsPerRevolution = 1200; // change this to fit the number of steps per revolution

// initialize the stepper library on pins D5 to D8
Stepper myStepper(stepsPerRevolution, D5, D6, D7, D2);

int stepCount = 0; // number of steps the motor has taken


void setup()
{
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(LED, OUTPUT);
  Blink(50, 10);
  // set the speed at 60 rpm:
  myStepper.setSpeed(40);
  // initialize the serial port:
  Serial.begin(115200);


}

void loop() {
  //  unsigned long Start = millis();
  //  Serial.println("clockwise");
  //  while (millis() - Start < 5000)
  //  {
  //    myStepper.step(-1);// step one step:
  //    stepCount = stepcount > 0 ? stepcount - 1 : stepcount ;
  //    delay(10);
  //  }
  //  Serial.println("counterclockwise");
  //  Start = millis();
  //  while (millis() - Start < 5000)
  //  {
  //  myStepper.step(1);// step one step:
  //  stepCount++;
  //  delay(100);
  //  }

  // step one revolution in one direction:
  digitalWrite(LED, 1);
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  digitalWrite(LED, 0);
  delay(500);


  // step one revolution in the other direction:
  digitalWrite(LED, 1);
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  digitalWrite(LED, 0);
  delay(500);

}

void Blink(int lapse, int times)
{
  int c = 0;
  while (c < times)
  {
    digitalWrite(LED, 1);
    delay(lapse);
    digitalWrite(LED, 0);
    delay(lapse);
    c++;
  }
}
