#include <SPI.h>

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int const pulse = A10;
int startpos;
long previous = 0;
long total = 0;
float revols;
float distInmm;
int steps;
bool done = false;
int forward = 86;
int reverse = 96;
int stopit = 93;
int sincelast;
float pi = 3.141593653589793;
float radius = 9.6; // radius in mm
// 1 revolution = 2*pi*R = 60.31 mm
float circ = 2.0 * pi * radius;


void setup() {
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  pinMode(pulse, INPUT);
  Serial.println("");
  Serial.print("SETUP DONE");

}

void loop() {

  distInmm = 120.62; // requested distance in mm
  // how many revolutions is that?
  revols = distInmm / circ; // just for debug purpose
  Serial.println("");
  Serial.print("Requested Revolutions = "); Serial.print(revols);
  ServoRun();
  total = 0;
  previous = 0;
  delay(2000);

}






