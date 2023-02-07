// just reading the encoder, nothing else.

int encoderValue_A1 = 0;
int encoderValue_A2 = 0;
int encoderValue_B1 = 0;
int encoderValue_B2 = 0;

void setup() {

  Serial.begin(9600);

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);

  attachInterrupt(digitalPinToInterrupt(2), countA1, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), countA2, FALLING);
  attachInterrupt(digitalPinToInterrupt(4), countB1, FALLING);
  attachInterrupt(digitalPinToInterrupt(5), countB2, FALLING);
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
      Serial.write(encoderValue_B1);
      Serial.write(encoderValue_B2);

      // reset the values
      encoderValue_A1 = 0;
      encoderValue_A2 = 0;
      encoderValue_B1 = 0;
      encoderValue_B2 = 0;
    }
  }
}

void countA1() {

  encoderValue_A1++;
}

void countB1() {

  encoderValue_B1++;
}

void countA2() {

  encoderValue_A2++;
}

void countB2() {

  encoderValue_B2++;
}
