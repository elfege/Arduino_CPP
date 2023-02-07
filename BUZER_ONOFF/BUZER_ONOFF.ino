int ledPin1 = 2;  // pushbutton connected to digital pin 7
int ledPin2 = 4;

int sensorPin = 8;
int sensorValue = 0;  // variable to store the read value

void setup() {
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(8, INPUT);
  // pinMode(13, INPUT);
  pinMode(A5, INPUT);
  // digitalWrite(A5, LOW);
  digitalWrite(8, LOW);  // turn off pullup resistors
    // digitalWrite(13, LOW); // turn off pullup resistors
}


void loop() {

  sensorValue = digitalRead(sensorPin);  // read the value from the sensor

  if (sensorValue != 0)  // Receiving a call from interphone - it's ringing.
  {

    digitalWrite(ledPin1, LOW);  // pressing the talk button
    delay(500);
    digitalWrite(ledPin1, LOW);
    delay(200);
    digitalWrite(ledPin2, LOW);  // open door
    // stop the program for <sensorValue> milliseconds:
    delay(sensorValue);
  }
  sensorValue = digitalRead(sensorPin);
  if (sensorValue == 0) {

    delay(sensorValue);
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
  }
  delay(200);
}