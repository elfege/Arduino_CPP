// works with ESP8266_WINDOW_BEDROOM_2.0_Stepper


unsigned long previousMillis = millis();
unsigned long previousMillisStop = millis();
boolean Stop = true;
int steps = 0;
int maxSteps = 10;

// Include the Arduino Stepper Library
#include <Stepper.h>

// Number of steps per output rotation
const int stepsPerRevolution = 1200;

// Create Instance of Stepper library
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);
#define INPUTA A0
#define INPUTB A1
#define StopPin  A2
#define StopPinOUTPUT 7

String dir = "";

void setup()
{

  pinMode(INPUTA, INPUT);
  pinMode(INPUTB, INPUT);
  pinMode(StopPinOUTPUT, OUTPUT);
  digitalWrite(StopPinOUTPUT, HIGH);

  // set the speed at 60 rpm:
  myStepper.setSpeed(40);
  // initialize the serial port:
  Serial.begin(9600);

  Serial.println("CALIBRATING");
  while (analogRead(StopPin) < 800)
  {
    dir = "rvs";
    // step one revolution in one direction:
    Serial.println("calibrating");
    myStepper.step(-stepsPerRevolution);
  }
  steps = 0;


}

void loop()
{
  if (analogRead(StopPin) > 1000)
  {
    STOP();
  }
  else
  {
    digitalWrite(StopPinOUTPUT, LOW);
  }
  if (millis() - previousMillis >= 5000)
  {
    Serial.print("INPUTA = "); Serial.println(analogRead(INPUTA));
    Serial.print("INPUTB = "); Serial.println(analogRead(INPUTB));
    Serial.print("StopPinOUTPUT = "); Serial.println(digitalRead(StopPinOUTPUT));
    Serial.print("StopPin = "); Serial.println(analogRead(StopPin));

    Serial.print("steps = "); Serial.println(steps);

    previousMillis = millis();
  }
  while (analogRead(INPUTA) > 500 && analogRead(INPUTB) < 500 && steps <= maxSteps)
  {
    // closing away from motor
    steps++;
    dir = "fwd";
    // step one revolution in one direction:
    Serial.println("clockwise");
    myStepper.step(stepsPerRevolution);

    if (analogRead(StopPin) > 1000)
    {
      STOP();
      break;
    }
    delay(1);
  }
  while (analogRead(INPUTB) > 500 && analogRead(INPUTA) < 500 )
  {
    // opening towards motor
    steps--;
    dir = "rvs";
    // step one revolution in the other direction:
    Serial.println("counterclockwise");
    myStepper.step(-stepsPerRevolution);
    if (analogRead(StopPin) > 1000)
    {
      STOP();
      break;
    }
    delay(1);
  }
}

void STOP()
{
  //Serial.println("STOP*******************");
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(StopPinOUTPUT, HIGH);

  if (dir == "rvs")
  {
    myStepper.step(200);
  }
  else if (dir == "fwd")
  {
    myStepper.step(-200);
  }

  dir = "stop";
  Stop = true;


}
