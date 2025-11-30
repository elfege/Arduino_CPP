
#define FIR               A3

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115000);
pinMode(FIR, INPUT);
}

void loop() {
 Serial.println(""); 
  Serial.print("FIR obstacle returns : ");Serial.println(FIR_obs());
  Serial.println(""); 
  Serial.print("FIR sensor value is : ");Serial.println(analogRead(FIR));
  delay(500);

}

boolean FIR_obs() {
  int val = analogRead(FIR);

  if (val > 1800) {
    return true;
  }
  else {
    return false;
  }
}
