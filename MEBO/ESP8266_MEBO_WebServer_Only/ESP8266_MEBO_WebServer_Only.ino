//Receiver Code


char str[4];

void setup() {
  Serial.begin(115200); // serial print out / debug / & flash
  //Serial1.begin(115200);

}

void loop() {
  int i = 0;

  
  if (Serial.available()) {
    delay(100); //allows all serial sent to be received together
    while (Serial.available() && i < 4) {
      str[i++] = Serial.read();
    }
    str[i++] = '\0';
  }

  

  if (i > 0) {
    Serial.println("");
    Serial.println(str);
  }


}
