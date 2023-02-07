//defining Pins names for the code

int pwm = 13; int forward = 12; int reverse = 11;


int encorderValue = 0; void count(void);

void setup()

{

  Serial.begin(9600);

  pinMode(2, INPUT);

  attachInterrupt(digitalPinToInterrupt(2), count, FALLING);

  encorderValue = 0;

}

void loop()

{

  digitalWrite(pwm, 255);

  digitalWrite(forward, 0);

  analogWrite(reverse, 140);

  Serial.print("Starting\n");

  delay(100); Serial.print("Encoder Value="); Serial.println(encorderValue);
/*
  while (1)

  {



    // to print encoder value on the screen Serial.print("Encoder Value="); Serial.println(encorderValue); //Setting value of encoder

    //defining the while statement condition if(encorderValue<5000)

    break; // loop will break as soon as encoder value reaches 5000 or above digitalWrite(forward,1);

    digitalWrite(reverse, 0);

    analogWrite(pwm, 255);

  }

  digitalWrite(forward, 1);

  digitalWrite(reverse, 1);

  analogWrite(pwm, 255);
*/
}

void count()

{

  encorderValue++;

}
