// just reading the encoder, nothing else.

int encoderValue_A1 = 0;
int encoderValue_A2 = 0;
int encoderValue_B1 = 0;
int encoderValue_B2 = 0;

int maxi = 20;

void setup() {

  Serial.print("start");
  Serial.begin(9600);

  pinMode(2, INPUT);
  pinMode(3, INPUT);


  attachInterrupt(digitalPinToInterrupt(2), countA1, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), countA2, FALLING);

  // remember : R3 UNO has only pins 2 and 3 that are interrupt capable.

}

void loop() {

  // if there is data to read, read it
  if (Serial.available() > 0) {

    // read the incoming data from the ezb
    int incomingByte = Serial.read();

    // command 'a' means transmit the current values and reset the counters
    if (incomingByte == 'a') {

      // send the values
      Serial.write(encoderValue_A1);
      Serial.write(encoderValue_A2);

      // reset the values once = 255

      //if (encoderValue_A1 >= 255) {
        encoderValue_A1 = 0;
     // }
      //if (encoderValue_A2 >= 255) {
        encoderValue_A2 = 0;
      //}
    }
  }
}

void countA1() {

  encoderValue_A1++;
}

void countA2() {

  encoderValue_A2++;
}

