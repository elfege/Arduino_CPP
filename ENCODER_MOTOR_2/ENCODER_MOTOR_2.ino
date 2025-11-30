//defining Pins names for the code

int pwm = 13; int forward = 10; int reverse = 11;

int MaxCountValue = 1000;

int encoderValue = 0; void count(void);

void setup()

{

  Serial.begin(9600);
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), count, FALLING);
  encoderValue = 0;
}

void loop()
{
  if (encoderValue < MaxCountValue)
  {
    digitalWrite(pwm, 225);

    digitalWrite(reverse, 1);
    digitalWrite(forward, 0);

    analogWrite(reverse, 50);
    analogWrite(forward, 0);
  }

  else
  {

    delay(500);
    digitalWrite(pwm, 255);

    digitalWrite(reverse, 0);
    digitalWrite(forward, 1);

    analogWrite(forward, 50);
    digitalWrite(reverse, 0);
  }
  delay(100);
  Serial.print("Encoder Value=");
  Serial.println(encoderValue);

  int MaxCountValueDouble = MaxCountValue * 2;
  
  if (encoderValue >= MaxCountValueDouble )
  {
    Serial.println("reverse");
    encoderValue = 0;
  }
}

void count()

{

  encoderValue++;


}


