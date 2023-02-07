// just reading the encoder, nothing else.

//defining Pins names for the code

int pwm = 13; int forward = 10; int reverse = 11;

int countencoder = 0;
int MaxCountValue = 640;

int encoderValue_A = 0; int encoderValue_B = 0; void count(void);


void setup()

{

  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), countA, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), countB, FALLING);
  
}

void loop() {


  // if there is data to read, read it
  if (Serial.available() > 0) {

    // read the incoming data from the ezb
    int incomingByte = Serial.read();

    // command 'a' means transmit the current values and reset the counters

    char received = 97;
    if (incomingByte == received) {

      // send the values
      Serial.write(encoderValue_A);
      //Serial.println(encoderValue_A);
      Serial.write(encoderValue_B);
      //Serial.println(encoderValue_B);

      //delay(300);
      // reset the values
     
      encoderValue_A = 0;
      encoderValue_B = 0;
      countencoder = 0;
     

    }
  }
}

void countA()

{
  encoderValue_A++;
}

void countB()

{
  encoderValue_B++;
}




