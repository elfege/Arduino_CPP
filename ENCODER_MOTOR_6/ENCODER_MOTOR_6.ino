// just reading the encoder, nothing else.

int L = 0;
int R = 0;
//int encoderValue_B1 = 0;
//int encoderValue_B2 = 0;

#define LED 2

void setup() {

  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  pinMode(32, INPUT);
  // pinMode(25, INPUT);
  pinMode(25, INPUT); // low amp 3.3v source
  //digitalWrite(27, HIGH);

  //  pinMode(4, INPUT);
  //  pinMode(5, INPUT);

  attachInterrupt(digitalPinToInterrupt(32), countA1, FALLING);
  attachInterrupt(digitalPinToInterrupt(25), countA2, FALLING);
  //attachInterrupt(digitalPinToInterrupt(4), countB1, FALLING);
  //attachInterrupt(digitalPinToInterrupt(5), countB2, FALLING);
  Serial.println("setup executed");
  Blink(10, 1000);
}

void loop() {

  //  // if there is data to read, read it
  //  if (Serial.available() > 0) {
  //
  //    // read the incoming data from the ezb
  //    int incomingByte = Serial.read();
  //
  //    // command 'a' means transmit the current values and reset the counters
  //    if (incomingByte == 'a') {
  //
  //      // send the values
  //      Serial.write(L);
  //      Serial.write(R);
  //      //Serial.write(encoderValue_B1);
  //      //Serial.write(encoderValue_B2);
  //
  //      // reset the values
  //      L = 0;
  //      R = 0;
  //      //encoderValue_B1 = 0;
  //      //encoderValue_B2 = 0;
  //    }
  //    Serial.flush();
  //  }
  //if (R >= 64) R = 0;
  //if (L >= 64) L = 0;
 }

void countA1() {
  
  L++;
  Serial.println(L);
  
}
/*
  void countB1() {

  encoderValue_B1++;
  }
*/

void countA2() {
  
  R++;
  Serial.println(R);
}
/*
  void countB2() {

  encoderValue_B2++;
  }
*/

void Blink(int times, int lapse) {
  int c = 0;
  while (c < times) {
    digitalWrite(LED, 1);
    delay(lapse);
    digitalWrite(LED, 0);
    delay(lapse);
    c++;
  }
}
